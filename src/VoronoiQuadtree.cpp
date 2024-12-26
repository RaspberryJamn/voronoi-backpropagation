#include "VoronoiQuadTree.h"
#include <algorithm>

VoronoiQuadTree::VoronoiQuadTree() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->root = new VQuadTree(this->x, this->y, this->x+this->w, this->y+this->h, 0);

    this->all_child_nodes.clear();
    this->total_child_count = 0;

    // GetNearby {
    this->nearby_candidates.clear();

    this->recent_x = 0;
    this->recent_y = 0;
    this->sort_recent_x = 0;
    this->sort_recent_y = 0;
    this->band_width = 0;
    this->sort_band_width = 0;
    this->current_bounding_mag = 0;
    this->current_box_radius = 0;
    // }
}

VoronoiQuadTree::~VoronoiQuadTree() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->DeleteTree(this->root);
    this->root = nullptr;

    this->all_child_nodes.clear();
    this->total_child_count = 0;

    // GetNearby {
    this->nearby_candidates.clear();

    this->recent_x = 0;
    this->recent_y = 0;
    this->sort_recent_x = 0;
    this->sort_recent_y = 0;
    this->band_width = 0;
    this->sort_band_width = 0;
    this->current_bounding_mag = 0;
    this->current_box_radius = 0;
    // }
}

void VoronoiQuadTree::RespecTree(int x, int y, int w, int h, int max_depth, int critical_mass) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->max_depth = max_depth;
    this->critical_mass = critical_mass;
    this->total_child_count = this->total_child_count;

    this->DeleteTree(this->root); // simply delete the entirety of the tree, no need to remove every node before readding it later
    this->root = nullptr;
    this->root = new VQuadTree(this->x, this->y, this->x+this->w, this->y+this->h, 0);

    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
        current_node->CalculateSortingPos();
        this->sort_recent_x = current_node->GetSortingPosX();
        this->sort_recent_y = current_node->GetSortingPosY();
        this->AddToChildren(current_node, this->root);
    });
}

void VoronoiQuadTree::DeleteTree(VQuadTree* branch) {
    if (branch->node_children.empty()) {
        if (branch->tree_children[0] != nullptr) {
            this->DeleteTree(branch->tree_children[0]); branch->tree_children[0] = nullptr;
            this->DeleteTree(branch->tree_children[1]); branch->tree_children[1] = nullptr;
            this->DeleteTree(branch->tree_children[2]); branch->tree_children[2] = nullptr;
            this->DeleteTree(branch->tree_children[3]); branch->tree_children[3] = nullptr;
        }
    } else {
        branch->node_children.clear();
    }
    delete branch;
}

bool VoronoiQuadTree::SplitValid(VQuadTree* branch) {
    return (branch->total_children > this->critical_mass) && (branch->depth < this->max_depth);
}
void VoronoiQuadTree::AddNode(VoronoiNode* node) {
    this->all_child_nodes.push_back(node);
    this->total_child_count++;

    node->CalculateSortingPos(); // this is where the new node's sort position is actually ascertained
    this->sort_recent_x = node->GetSortingPosX();
    this->sort_recent_y = node->GetSortingPosY();
    this->AddToChildren(node, this->root);
}
void VoronoiQuadTree::AddToChildren(VoronoiNode* node, VQuadTree* branch) {

    if (this->SplitValid(branch)) { // after the split would happen, and on that point it had been allowed to do so

        this->AddToChildrenSplit(node, branch);
        branch->total_children++;

    } else { // we're either below critical mass or above it and weren't allowed to split

        branch->node_children.push_back(node);
        branch->total_children++;
        node->SetBounds(branch->min_x, branch->min_y, branch->max_x, branch->max_y); // node knows its shape

        if (this->SplitValid(branch)) { // or more specifically, does it _now_ qualify for splitting

            VQuadTree::CalculateHalfXY(branch);

            branch->tree_children[0] = new VQuadTree(branch->min_x , branch->min_y , branch->half_x, branch->half_y, branch->depth+1);
            branch->tree_children[1] = new VQuadTree(branch->half_x, branch->min_y , branch->max_x , branch->half_y, branch->depth+1);
            branch->tree_children[2] = new VQuadTree(branch->min_x , branch->half_y, branch->half_x, branch->max_y , branch->depth+1);
            branch->tree_children[3] = new VQuadTree(branch->half_x, branch->half_y, branch->max_x , branch->max_y , branch->depth+1);

            std::for_each(branch->node_children.begin(), branch->node_children.end(), [&](VoronoiNode* current_node) {
                VoronoiNode* repositioned_node = current_node; // get the node
                this->sort_recent_x = repositioned_node->GetSortingPosX(); // and its position (NOT recalculated)
                this->sort_recent_y = repositioned_node->GetSortingPosY();
                this->AddToChildrenSplit(repositioned_node, branch); // the nodes in each slot find their new home
            });

            branch->node_children.clear(); // at the end, everything in the list has been deleted, so the head of the list is cleared
        }
    }
}
void VoronoiQuadTree::AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch) {
    if (this->sort_recent_y < branch->half_y) {
        if (this->sort_recent_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[0]);
        } else {
            this->AddToChildren(node, branch->tree_children[1]);
        }
    } else {
        if (this->sort_recent_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[2]);
        } else {
            this->AddToChildren(node, branch->tree_children[3]);
        }
    }
}

void VoronoiQuadTree::RemoveNode(VoronoiNode* node) {
    this->all_child_nodes.erase(find(this->all_child_nodes.begin(), this->all_child_nodes.end(), node));
    this->total_child_count--;
    this->sort_recent_x = node->GetSortingPosX();
    this->sort_recent_y = node->GetSortingPosY();
    this->RemoveFromBranch(node, this->root);
}

void VoronoiQuadTree::RemoveFromBranch(VoronoiNode* node, VQuadTree* branch) {
    if (this->SplitValid(branch)) { // branched
        if (this->sort_recent_y < branch->half_y) {
            if (this->sort_recent_x < branch->half_x) {
                this->RemoveFromBranch(node, branch->tree_children[0]);
            } else {
                this->RemoveFromBranch(node, branch->tree_children[1]);
            }
        } else {
            if (this->sort_recent_x < branch->half_x) {
                this->RemoveFromBranch(node, branch->tree_children[2]);
            } else {
                this->RemoveFromBranch(node, branch->tree_children[3]);
            }
        }
        branch->total_children--;
        if (!this->SplitValid(branch)) {// should the trees remerge?
            this->ConsolidateChildLists(branch);
        }
        return;
    } else { // leaf
        branch->node_children.erase(find(branch->node_children.begin(), branch->node_children.end(), node));
        branch->total_children--;
    }
}

void VoronoiQuadTree::ConsolidateChildLists(VQuadTree* branch) {
    std::vector<VoronoiNode*> concatenated = {};
    for (int i = 0; i < 4; i++) {
        std::for_each(branch->tree_children[i]->node_children.begin(), branch->tree_children[i]->node_children.end(), [&](VoronoiNode* current_node) {
            concatenated.push_back(current_node);
        });
        delete branch->tree_children[i];
        branch->tree_children[i] = nullptr;
    }
    branch->node_children.swap(concatenated);
}

void VoronoiQuadTree::UpdateNodePositions() {
//    this->RespecTree(this->x, this->y, this->w, this->h, this->max_depth, this->critical_mass);
    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
        int x_copy = current_node->GetSortingPosX(); // store a copy of the old sorting position before the most recent move
        int y_copy = current_node->GetSortingPosY();

        current_node->Clamp(this->x, this->y, this->x+this->w, this->y+this->h);

        current_node->CalculateSortingPos(); // you moved, therefore you should have an updated sorting position
        if (!(current_node->IsBounded(current_node->GetSortingPosX(), current_node->GetSortingPosY()))) { // now, have you left the box you were in just a moment ago?
            this->sort_recent_x = x_copy;
            this->sort_recent_y = y_copy;
            this->RemoveFromBranch(current_node, this->root); // use the old for removing it
            this->sort_recent_x = current_node->GetSortingPosX();// and the new for adding it
            this->sort_recent_y = current_node->GetSortingPosY();
            this->AddToChildren(current_node, this->root);
        }
    });
}

std::vector<VoronoiNode*> VoronoiQuadTree::GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed) {
    this->SetGain(gain);
    this->SetBandWidth(band_width);
    return this->GetNearby(x, y, seed);
}
std::vector<VoronoiNode*> VoronoiQuadTree::GetNearby(double x, double y, VoronoiNode* seed) {
    this->recent_x = x;
    this->recent_y = y;
    this->sort_recent_x = (int)this->recent_x;
    this->sort_recent_y = (int)this->recent_y;

    if (seed != nullptr) {
        seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = seed->GetDist() + this->band_width;
        this->current_box_radius = seed->GetSortingDist() + this->sort_band_width;
    } else {
        VoronoiNode* arbitrary_seed = this->all_child_nodes.front();
        arbitrary_seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        arbitrary_seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = arbitrary_seed->GetDist() + this->band_width;
        this->current_box_radius = arbitrary_seed->GetSortingDist() + this->sort_band_width;
    }

    this->nearby_candidates.clear();

        this->BuildNearbyList(this->root); // this is where the job is mostly done

    SDL_assert(!this->nearby_candidates.empty());

    size_t nearest_slot = 0; // we're gonna be searching for the nearest node to swap to the front
    size_t current_slot = 0;
    double nearest_dist = this->nearby_candidates.front()->GetDist();

    for (std::vector<VoronoiNode*>::iterator it = this->nearby_candidates.begin(); it != this->nearby_candidates.end();) {

        double current_distance = (*it)->GetDist();
        if (current_distance <= this->current_bounding_mag) { // scanned node passes the check, may have significant m value

            if (current_distance < nearest_dist) {
                nearest_slot = current_slot;
                nearest_dist = current_distance;
            }
            current_slot++;
            ++it;

        } else { // node fails the check, will definitely not have significant m value
            it = this->nearby_candidates.erase(it);
        }
    }

    if (nearest_slot != 0) { // puts the nearest node at the top
        VoronoiNode* temp_first = this->nearby_candidates.front();
        this->nearby_candidates.front() = this->nearby_candidates.at(nearest_slot);
        this->nearby_candidates.at(nearest_slot) = temp_first;
    }
    std::vector<VoronoiNode*> result_list = {};
    this->nearby_candidates.swap(result_list);
    return result_list;
}
void VoronoiQuadTree::BuildNearbyList(VQuadTree* branch) { // takes the running info on the best distance upper bound and adds the nodes descendant to this branch to the list of potential candidates
    if (!this->SplitValid(branch)) { // not split, this is a leaf node

        VoronoiNode* new_nearest = nullptr;
        std::for_each(branch->node_children.begin(), branch->node_children.end(), [&](VoronoiNode* current_node) {

            current_node->CalculateDist(this->recent_x, this->recent_y, this->gain);
            double relative_dist = this->current_bounding_mag-current_node->GetDist(); // from this node to the outer circle
            if (relative_dist > 0) { // beats the outer circle
                this->nearby_candidates.push_back(current_node);
                if (relative_dist >= this->band_width) { // beats the middle circle
                    this->current_bounding_mag = current_node->GetDist()+this->band_width; // now the outer circle is this dist plus out by band width
                    new_nearest = current_node;
                }
            }
        });

        if (new_nearest != nullptr) { // if a candidate in this branch beat the standing record
            new_nearest->CalculateSortingDist(this->recent_x, this->recent_y); // do the work to find the box radius
            this->current_box_radius = new_nearest->GetSortingDist() + this->sort_band_width; // update the search

            this->current_bounding_mag = new_nearest->GetDist()+this->band_width;
        }

    } else { // split, doesnt itself have child nodes but it's boughs may
/*
 *          0|1
 *          -+- the quadrants
 *          2|3
 *
 *            \ 0123 | 1032 /
 *              \    |    /
 *          0213  \  |  /  1302
 *          ---------+--------- the order in which they should be searched, as indexed by the position of the sample center
 *          2031  /  |  \  3120
 *              /    |    \
 *            / 2301 | 3210 \
 */
        size_t search_1;
        size_t search_2;
        size_t search_3;
        size_t search_4;
        bool search_x_then_y;
        int dx = this->sort_recent_x-branch->half_x; // and now this is the vector position of the sample center, with the branch as the origin
        int dy = this->sort_recent_y-branch->half_y;
        if (dx < 0) {
            if (dy < 0) {
                search_1 = 0;
                search_4 = 3;
            } else {
                search_1 = 2;
                search_4 = 1;
            }
        } else {
            if (dy < 0) {
                search_1 = 1;
                search_4 = 2;
            } else {
                search_1 = 3;
                search_4 = 0;
            }
        }
        if ((search_1 == 0) || (search_1 == 3)) {
            if ( dy < dx ) { // yes towards up left
                search_2 = 1;
                search_3 = 2;
            } else { // towards down right
                search_2 = 2;
                search_3 = 1;
            }
        } else {
            if ( dy < -dx ) { // yes towards up right
                search_2 = 0;
                search_3 = 3;
            } else { // towards down left
                search_2 = 3;
                search_3 = 0;
            }
        }
        search_x_then_y = ((search_2 < search_3) == (dy < 0)); // activates in an hourglass shape

        this->BuildNearbyList(branch->tree_children[search_1]);

        if (search_x_then_y) { // search x first
            // dx is the vector from midpoint to sample
            // (dx is positive, actually so positive its tail on the left cant possibly fit in the box) || (dx is negative, even more negative than the box can accomodate, the tail sticks out to the right)
            if ((dx > this->current_box_radius) || (dx < -this->current_box_radius)) {
                return;
            }
        } else { // search y first
            if ((dy > this->current_box_radius) || (dy < -this->current_box_radius)) {
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_2]);

        if (search_x_then_y) { // search y second
            if ((dy > this->current_box_radius) || (dy < -this->current_box_radius)) {
                return;
            }
        } else { // search x second
            if ((dx > this->current_box_radius) || (dx < -this->current_box_radius)) {
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_3]);

        if ((dx*dx+dy*dy)*this->gain > this->current_bounding_mag) { // center point mag further out than the bounding outer circle mag (bounding mag has gain in it)
            return;
        }
        this->BuildNearbyList(branch->tree_children[search_4]);
    }
}
void VoronoiQuadTree::SetGain(double gain) {
    this->gain = gain;
    this->SetBandWidth(this->band_width);
}
void VoronoiQuadTree::SetBandWidth(double band_width) {
    this->band_width = band_width;
    this->sort_band_width = (int)(std::ceil(this->band_width/this->gain));
}
double VoronoiQuadTree::GetGain() {
    return this->gain;
}
std::vector<VoronoiNode*> VoronoiQuadTree::GetAllNodes() {
    return this->all_child_nodes;
}
