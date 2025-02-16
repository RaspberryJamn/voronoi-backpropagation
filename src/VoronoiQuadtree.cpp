#include "VoronoiQuadTree.h"
#include <algorithm>
#include <iostream>

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
    this->search.nearby_candidates.clear();

    this->search.recent_seed = nullptr;
    this->search.recent_x = 0;
    this->search.recent_y = 0;
    this->search.sort_recent_x = 0;
    this->search.sort_recent_y = 0;
    this->search.band_width = 0;
    this->search.sort_band_width = 0;
    this->search.current_bounding_mag = 0;
    this->search.current_box_radius = 0;
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
    this->search.nearby_candidates.clear();

    this->search.recent_x = 0;
    this->search.recent_y = 0;
    this->search.sort_recent_x = 0;
    this->search.sort_recent_y = 0;
    this->search.band_width = 0;
    this->search.sort_band_width = 0;
    this->search.current_bounding_mag = 0;
    this->search.current_box_radius = 0;
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
//        current_node->CalculateSortingPos();
        current_node->sort.x = (int)current_node->x;
        current_node->sort.y = (int)current_node->y;
        this->search.sort_recent_x = current_node->sort.x;
        this->search.sort_recent_y = current_node->sort.y;
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

//    node->CalculateSortingPos(); // this is where the new node's sort position is actually ascertained
    node->sort.x = (int)node->x;
    node->sort.y = (int)node->y;
    this->search.sort_recent_x = node->sort.x;
    this->search.sort_recent_y = node->sort.y;
    this->AddToChildren(node, this->root);
}
void VoronoiQuadTree::AddToChildren(VoronoiNode* node, VQuadTree* branch) {

    if (this->SplitValid(branch)) { // after the split would happen, and on that point it had been allowed to do so

        this->AddToChildrenSplit(node, branch);
        branch->total_children++;

    } else { // we're either below critical mass or above it and weren't allowed to split

        branch->node_children.push_back(node);
        branch->total_children++;
//        node->SetBounds(branch->min_x, branch->min_y, branch->max_x, branch->max_y); // node knows its shape
        node->sort.x_min = branch->min_x;
        node->sort.y_min = branch->min_y;
        node->sort.x_max = branch->max_x;
        node->sort.y_max = branch->max_y;

        if (this->SplitValid(branch)) { // or more specifically, does it _now_ qualify for splitting

            VQuadTree::CalculateHalfXY(branch);

            branch->tree_children[0] = new VQuadTree(branch->min_x , branch->min_y , branch->half_x, branch->half_y, branch->depth+1);
            branch->tree_children[1] = new VQuadTree(branch->half_x, branch->min_y , branch->max_x , branch->half_y, branch->depth+1);
            branch->tree_children[2] = new VQuadTree(branch->min_x , branch->half_y, branch->half_x, branch->max_y , branch->depth+1);
            branch->tree_children[3] = new VQuadTree(branch->half_x, branch->half_y, branch->max_x , branch->max_y , branch->depth+1);

            std::for_each(branch->node_children.begin(), branch->node_children.end(), [&](VoronoiNode* current_node) {
                VoronoiNode* repositioned_node = current_node; // get the node
                this->search.sort_recent_x = repositioned_node->sort.x; // and its position (NOT recalculated)
                this->search.sort_recent_y = repositioned_node->sort.y;
                this->AddToChildrenSplit(repositioned_node, branch); // the nodes in each slot find their new home
            });

            branch->node_children.clear(); // at the end, everything in the list has been deleted, so the head of the list is cleared
        }
    }
}
void VoronoiQuadTree::AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch) {
    if (this->search.sort_recent_y < branch->half_y) {
        if (this->search.sort_recent_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[0]);
        } else {
            this->AddToChildren(node, branch->tree_children[1]);
        }
    } else {
        if (this->search.sort_recent_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[2]);
        } else {
            this->AddToChildren(node, branch->tree_children[3]);
        }
    }
}

void VoronoiQuadTree::RemoveNode(VoronoiNode* node) {
    this->all_child_nodes.erase(find(this->all_child_nodes.begin(), this->all_child_nodes.end(), node));
    this->total_child_count--;
    this->search.sort_recent_x = node->sort.x;
    this->search.sort_recent_y = node->sort.y;
    this->RemoveFromBranch(node, this->root);
}

void VoronoiQuadTree::RemoveFromBranch(VoronoiNode* node, VQuadTree* branch) {
    if (this->SplitValid(branch)) { // branched
        if (this->search.sort_recent_y < branch->half_y) {
            if (this->search.sort_recent_x < branch->half_x) {
                this->RemoveFromBranch(node, branch->tree_children[0]);
            } else {
                this->RemoveFromBranch(node, branch->tree_children[1]);
            }
        } else {
            if (this->search.sort_recent_x < branch->half_x) {
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
        int x_copy = current_node->sort.x; // store a copy of the old sorting position before the most recent move
        int y_copy = current_node->sort.y;

        if (current_node->x <  this->x) { current_node->x = this->x;}
        if (current_node->x >= this->x+this->w) { current_node->x = this->x+this->w-1;}
        if (current_node->y <  this->y) { current_node->y = this->y;}
        if (current_node->y >= this->y+this->h) { current_node->y = this->y+this->h-1;}

        current_node->sort.x = (int)current_node->x;// you moved, therefore you should have an updated sorting position
        current_node->sort.y = (int)current_node->y;
        if ((current_node->sort.x < current_node->sort.x_min) ||
            (current_node->sort.x >=current_node->sort.x_max) ||
            (current_node->sort.y < current_node->sort.y_min) ||
            (current_node->sort.y >=current_node->sort.y_max)) { // now, have you left the box you were in just a moment ago?
            this->search.sort_recent_x = x_copy;
            this->search.sort_recent_y = y_copy;
            this->RemoveFromBranch(current_node, this->root); // use the old for removing it
            this->search.sort_recent_x = current_node->sort.x;// and the new for adding it
            this->search.sort_recent_y = current_node->sort.y;
            this->AddToChildren(current_node, this->root);
        }
    });
}

std::vector<VoronoiNode*> VoronoiQuadTree::GetNearby(double x, double y, double band_width, double gain) {
    this->SetGain(gain);
    this->SetBandWidth(band_width);
    return this->GetNearby(x, y);
}
std::vector<VoronoiNode*> VoronoiQuadTree::GetNearby(double x, double y) {
    this->search.recent_x = x;
    this->search.recent_y = y;
    this->search.sort_recent_x = (int)this->search.recent_x;
    this->search.sort_recent_y = (int)this->search.recent_y;

    if (this->search.recent_seed == nullptr) {
        this->search.recent_seed = this->all_child_nodes.front();
    }
    this->search.recent_seed->CalculateMag(this->search.recent_x, this->search.recent_y, this->search.gain); // work done here
    this->search.recent_seed->CalculateBoxRadius(this->search.recent_x, this->search.recent_y); // work done here
    this->search.current_bounding_mag = this->search.recent_seed->sort.mag + this->search.band_width;
    this->search.current_box_radius = this->search.recent_seed->sort.box_radius + this->search.sort_band_width;

    this->search.nearby_candidates.clear();

        this->BuildNearbyList(this->root); // this is where the job is mostly done

//    SDL_assert(!this->search.nearby_candidates.empty());
    if (this->search.nearby_candidates.empty()) {
        std::cout << "assert failed: !this->search.nearby_candidates.empty() in VoronoiQuadTree::GetNearby" << std::endl;
        this->PrintTree();
    }

    size_t nearest_slot = 0; // we're gonna be searching for the nearest node to swap to the front
    size_t current_slot = 0;
    double nearest_dist = this->search.nearby_candidates.front()->sort.mag;

    for (std::vector<VoronoiNode*>::iterator it = this->search.nearby_candidates.begin(); it != this->search.nearby_candidates.end();) {

        double current_distance = (*it)->sort.mag;
        if (current_distance <= this->search.current_bounding_mag) { // scanned node passes the check, may have significant m value

            if (current_distance < nearest_dist) {
                nearest_slot = current_slot;
                nearest_dist = current_distance;
            }
            current_slot++;
            ++it;

        } else { // node fails the check, will definitely not have significant m value
            it = this->search.nearby_candidates.erase(it);
        }
    }

    if (nearest_slot != 0) { // puts the nearest node at the top
        VoronoiNode* temp_first = this->search.nearby_candidates.front();
        this->search.nearby_candidates.front() = this->search.nearby_candidates.at(nearest_slot);
        this->search.nearby_candidates.at(nearest_slot) = temp_first;
    }
    std::vector<VoronoiNode*> result_list = {};
    this->search.nearby_candidates.swap(result_list);
    return result_list;
}
void VoronoiQuadTree::BuildNearbyList(VQuadTree* branch) { // takes the running info on the best distance upper bound and adds the nodes descendant to this branch to the list of potential candidates
    if (!this->SplitValid(branch)) { // not split, this is a leaf node

        VoronoiNode* new_nearest = nullptr;
        std::for_each(branch->node_children.begin(), branch->node_children.end(), [&](VoronoiNode* current_node) {

            current_node->CalculateMag(this->search.recent_x, this->search.recent_y, this->search.gain);
            double relative_dist = this->search.current_bounding_mag-current_node->sort.mag; // from this node to the outer circle
            if (relative_dist > 0) { // beats the outer circle
                this->search.nearby_candidates.push_back(current_node);
                if (relative_dist >= this->search.band_width) { // beats the middle circle
                    this->search.current_bounding_mag = current_node->sort.mag+this->search.band_width; // now the outer circle is this dist plus out by band width
                    new_nearest = current_node;
                }
            }
        });

        if (new_nearest != nullptr) { // if a candidate in this branch beat the standing record
            new_nearest->CalculateBoxRadius(this->search.recent_x, this->search.recent_y); // do the work to find the box radius

            this->search.current_box_radius = new_nearest->sort.box_radius + this->search.sort_band_width; // update the search

            this->search.current_bounding_mag = new_nearest->sort.mag+this->search.band_width;
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
        int dx = this->search.sort_recent_x-branch->half_x; // and now this is the vector position of the sample center, with the branch as the origin
        int dy = this->search.sort_recent_y-branch->half_y;
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
            if ((dx > this->search.current_box_radius) || (dx < -this->search.current_box_radius)) {
                return;
            }
        } else { // search y first
            if ((dy > this->search.current_box_radius) || (dy < -this->search.current_box_radius)) {
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_2]);

        if (search_x_then_y) { // search y second
            if ((dy > this->search.current_box_radius) || (dy < -this->search.current_box_radius)) {
                return;
            }
        } else { // search x second
            if ((dx > this->search.current_box_radius) || (dx < -this->search.current_box_radius)) {
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_3]);

        if ((dx*dx+dy*dy)*this->search.gain > this->search.current_bounding_mag) { // center point mag further out than the bounding outer circle mag (bounding mag has gain in it)
            return;
        }
        this->BuildNearbyList(branch->tree_children[search_4]);
    }
}
void VoronoiQuadTree::SetGain(double gain) {
    this->search.gain = gain;
    this->SetBandWidth(this->search.band_width);
}
void VoronoiQuadTree::SetBandWidth(double band_width) {
    this->search.band_width = band_width;
    this->search.sort_band_width = (int)(std::ceil(this->search.band_width/this->search.gain));
}
double VoronoiQuadTree::GetGain() {
    return this->search.gain;
}
std::vector<VoronoiNode*> VoronoiQuadTree::GetAllNodes() {
    return this->all_child_nodes;
}
int VoronoiQuadTree::GetNodeCount() {
    return this->total_child_count;
}
