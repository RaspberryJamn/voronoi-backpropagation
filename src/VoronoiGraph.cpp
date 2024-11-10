#include "VoronoiGraph.h"
#include <cmath>
#include <iostream>

void PrintIndents(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
}
void VQuadTree::Print(VQuadTree* tree, int indent) {
    if (tree == nullptr) {
        std::cout << "invalid tree, null pointer" << std::endl;
        return;
    }
    std::cout << "{" << std::endl;
    PrintIndents(indent+1); std::cout << "total_children: " << tree->total_children << "," << std::endl;
    PrintIndents(indent+1); std::cout << "depth: " << tree->depth << "," << std::endl;
    PrintIndents(indent+1); std::cout << "half_x: " << tree->half_x << "," << std::endl;
    PrintIndents(indent+1); std::cout << "half_y: " << tree->half_y << "," << std::endl;
    PrintIndents(indent+1); NodeLinkedList::Print("node_children: ", tree->node_children, indent+1);
    if (tree->tree_children[0] == nullptr) {
        PrintIndents(indent+1); std::cout << "tree_children: []" << std::endl;
    } else {
        PrintIndents(indent+1); std::cout << "tree_children: [" << std::endl;
        PrintIndents(indent+2); std::cout << "[0]: "; VQuadTree::Print(tree->tree_children[0], indent+2);
        PrintIndents(indent+2); std::cout << "[1]: "; VQuadTree::Print(tree->tree_children[1], indent+2);
        PrintIndents(indent+2); std::cout << "[2]: "; VQuadTree::Print(tree->tree_children[2], indent+2);
        PrintIndents(indent+2); std::cout << "[3]: "; VQuadTree::Print(tree->tree_children[3], indent+2);
        PrintIndents(indent+1); std::cout << "]" << std::endl;
    }
    PrintIndents(indent); std::cout<< "}" << std::endl;
}

void VoronoiGraph::PrintTree() {
    VQuadTree::Print(this->root, 0);
}

VoronoiGraph::VoronoiGraph() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->root = new VQuadTree(this->x+this->w/2, this->y+this->h/2, 0);

    this->all_child_nodes = nullptr;
    this->total_child_count = 0;

    this->gain = 0;

    this->nearby_candidates = nullptr;

    this->recent_x = 0;
    this->recent_y = 0;
    this->recent_sort_x = 0;
    this->recent_sort_y = 0;
    this->band_width = 0;
    this->sort_band_width = 0;
    this->current_bounding_mag = 0;
    this->current_box_radius = 0;
}

VoronoiGraph::~VoronoiGraph() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->DeleteTree(this->root);
    this->root = nullptr;

    NodeLinkedList::DeleteNodes(this->all_child_nodes);
    this->all_child_nodes = nullptr;
    this->total_child_count = 0;

    this->gain = 0;

    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;

    this->recent_x = 0;
    this->recent_y = 0;
    this->recent_sort_x = 0;
    this->recent_sort_y = 0;
    this->band_width = 0;
    this->sort_band_width = 0;
    this->current_bounding_mag = 0;
    this->current_box_radius = 0;
}

void VoronoiGraph::DeleteTree(VQuadTree* branch) {
    if (branch->node_children == nullptr) {
        if (branch->tree_children[0] != nullptr) {
            this->DeleteTree(branch->tree_children[0]);
            this->DeleteTree(branch->tree_children[1]);
            this->DeleteTree(branch->tree_children[2]);
            this->DeleteTree(branch->tree_children[3]);
        }
    } else {
        NodeLinkedList::DeleteList(branch->node_children);
    }
    delete branch;
}

void VoronoiGraph::RespecTree(int x, int y, int w, int h, int max_depth, int critical_mass) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->max_depth = max_depth;
    this->critical_mass = critical_mass;

    this->DeleteTree(this->root); // simply delete the entirety of the tree, no need to remove every node before readding it later
    this->root = new VQuadTree(this->x+this->w/2, this->y+this->h/2, 0);

//    NodeLinkedList::DeleteList(this->nearby_candidates);
//    this->nearby_candidates = nullptr;

    NodeLinkedList* current_slot = this->all_child_nodes; // start with the first slot of all nodes
    while (current_slot != nullptr) { // and for each node's slot
        VoronoiNode* node = current_slot->node; // get the node
        this->recent_sort_x = node->GetSortingPosX(); // and its position
        this->recent_sort_y = node->GetSortingPosY();
        this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h); // and place it into the new tree
        current_slot = current_slot->next;
    }
}

bool VoronoiGraph::SplitValid(VQuadTree* branch) {
    return (branch->total_children > this->critical_mass) && (branch->depth < this->max_depth);
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    NodeLinkedList::Append(node, &this->all_child_nodes);
    node->SetResidence(this->all_child_nodes);

    node->CalculateSortingPos(); // this is where the new node's sort position is actually ascertained
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
}
void VoronoiGraph::AddToChildren(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y) {

    if (this->SplitValid(branch)) { // after the split would happen, and on that point it had been allowed to do so

        this->AddToChildrenSplit(node, branch, min_x, min_y, max_x, max_y);
        branch->total_children++;

    } else { // we're either below critical mass or above it and weren't allowed to split

        NodeLinkedList::Append(node, &branch->node_children); // node given a slot
        node->SetTreeSlot(branch->node_children); // node knows its reference
        node->SetBounds(min_x, min_y, max_x, max_y); // node knows its shape

        branch->total_children++;

        if (this->SplitValid(branch)) { // or more specifically, does it _now_ qualify for splitting

            branch->tree_children[0] = new VQuadTree((min_x+branch->half_x)/2, (min_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[1] = new VQuadTree((max_x+branch->half_x)/2, (min_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[2] = new VQuadTree((min_x+branch->half_x)/2, (max_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[3] = new VQuadTree((max_x+branch->half_x)/2, (max_y+branch->half_y)/2, branch->depth+1);

            // this branch's half x and y could theoretically be undecided up until now (where it's children would be undecided too)

            NodeLinkedList* current_slot = branch->node_children; // start with the first tree child slot
            while (current_slot != nullptr) { // and go through all of them
                NodeLinkedList* next_slot = current_slot->next;
                VoronoiNode* repositioned_node = current_slot->node; // get the node
                this->recent_sort_x = repositioned_node->GetSortingPosX(); // and its position (NOT recalculated)
                this->recent_sort_y = repositioned_node->GetSortingPosY();
                this->AddToChildrenSplit(repositioned_node, branch, min_x, min_y, max_x, max_y); // the nodes in each slot find their new home
                delete current_slot; // and their old slot is deleted
                current_slot = next_slot;
            }
            branch->node_children = nullptr; // at the end, everything in the list has been deleted, so the head of the list is cleared
        }
    }
}
void VoronoiGraph::AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y) {
    if (this->recent_sort_y < branch->half_y) {
        if (this->recent_sort_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[0], min_x, min_y, branch->half_x, branch->half_y);
        } else {
            this->AddToChildren(node, branch->tree_children[1], branch->half_x, min_y, max_x, branch->half_y);
        }
    } else {
        if (this->recent_sort_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[2], min_x, branch->half_y, branch->half_x, max_y);
        } else {
            this->AddToChildren(node, branch->tree_children[3], branch->half_x, branch->half_y, max_x, max_y);
        }
    }
}

void VoronoiGraph::RemoveNode(VoronoiNode* node) {
    NodeLinkedList::RemoveResidence(node, &this->all_child_nodes); // here is where the node is officially located, now gone
    // the node's sort position should not be recalculated now as that would mess with the function's ability to actually find the node
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->RemoveFromChildren(node, this->root); // this, in effect, is really just about consolidating branches no longer above critical mass
}
void VoronoiGraph::RemoveFromChildren(VoronoiNode* node, VQuadTree* branch) {
    if (!this->SplitValid(branch)) { // this branch is below critical mass or is above but was disallowed splitting

        NodeLinkedList::RemoveTreeLocation(node, &branch->node_children); // if I had a NLL::Contains() function I'd put an assert for that to be true because it REALLY shoudnt be false, but I dont soo
        branch->total_children--; // if below critical mass, will stay below critical mass. if above, well if it couldnt split before it wouldnt need to merge now

    } else { // above critical mass and was allowed to split, aka does have tree children and does not have node children
        if (this->recent_sort_y < branch->half_y) {
            if (this->recent_sort_x < branch->half_x) {
                this->RemoveFromChildren(node, branch->tree_children[0]);
            } else {
                this->RemoveFromChildren(node, branch->tree_children[1]);
            }
        } else {
            if (this->recent_sort_x < branch->half_x) {
                this->RemoveFromChildren(node, branch->tree_children[2]);
            } else {
                this->RemoveFromChildren(node, branch->tree_children[3]);
            }
        }
        branch->total_children--;
        if (branch->total_children <= this->critical_mass) { // (any child has critical mass) -> (parent has critical mass), therefore (parent does not have critical mass) -> (no child has critical mass)
            this->ConsolidateChildLists(branch);
        }
    }
}
void VoronoiGraph::ConsolidateChildLists(VQuadTree* branch) {
    NodeLinkedList* growing_start = branch->tree_children[0]->node_children;
    delete branch->tree_children[0];
    branch->tree_children[0] = nullptr;

    for (int i = 1; i <= 3; i++) {
        NodeLinkedList* prepend_start = branch->tree_children[i]->node_children;
        delete branch->tree_children[i];
        branch->tree_children[i] = nullptr;

        if (prepend_start != nullptr) {
            NodeLinkedList* sliding_slot = prepend_start;
            NodeLinkedList* next_slot = sliding_slot->next;
            while (next_slot != nullptr) {
                sliding_slot = next_slot;
                next_slot = sliding_slot->next;
            } // now sliding_slot has the end of the newcomer list
            sliding_slot->next = growing_start;
            growing_start = prepend_start;
        }
    }

    branch->node_children = growing_start;
}

void VoronoiGraph::UpdateNodePositions() {
    NodeLinkedList* current_slot = this->all_child_nodes;
    while (current_slot != nullptr) {
        VoronoiNode* node = current_slot->node;
        node->CalculateSortingPos();
        if (!node->IsBounded()) {
            this->recent_sort_x = node->GetSortingPosX();
            this->recent_sort_y = node->GetSortingPosY();
            this->RemoveFromChildren(node, this->root);
            this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
        }
        current_slot = current_slot->next;
    }
}

NodeLinkedList* VoronoiGraph::GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed) {
    this->recent_x = x;
    this->recent_y = y;
    this->recent_sort_x = (int)this->recent_x;
    this->recent_sort_y = (int)this->recent_y;
    this->band_width = band_width;
    this->gain = gain;
    this->sort_band_width = (int)(std::ceil(this->band_width/this->gain));

    if (seed != nullptr) {
        seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = seed->GetDist() + this->band_width;
        this->current_box_radius = seed->GetSortingDist() + this->sort_band_width;
    } else {
        VoronoiNode* arbitrary_seed = this->all_child_nodes->node;
        arbitrary_seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        arbitrary_seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = arbitrary_seed->GetDist() + band_width;
        this->current_box_radius = arbitrary_seed->GetSortingDist() + sort_band_width;
    }

    this->current_bounding_mag = 1000000000000;
    this->current_box_radius = 1000000000;

    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;

        this->BuildNearbyList(this->root); // this is where the job is mostly done

    if (this->nearby_candidates == nullptr) { // something has gone horribly wrong
        std::cout << "erm" << std::endl;
        return nullptr;
    }

    return this->nearby_candidates;

//    NodeLinkedList* scanning_candidate = this->nearby_candidates;
//    NodeLinkedList* result_list = nullptr; // start with an empty output list
//
//    NodeLinkedList* nearest_slot = nullptr; // we're gonna be searching for the nearest node to swap to the front
//    double nearest_dist = this->nearby_candidates->node->GetDist();
//
//    while (scanning_candidate != nullptr) {
//
//        NodeLinkedList* next_entry = scanning_candidate->next;
//        double scanned_distance = scanning_candidate->node->GetDist();
//
//        if (scanned_distance <= this->current_bounding_mag) { // scanned node passes the check, may have significant m value
//
//            scanning_candidate->next = result_list; // this candidate slot gets prepened onto result_list
//            result_list = scanning_candidate;
//
//            if (scanned_distance < nearest_dist) {
//                nearest_slot = scanning_candidate;
//                nearest_dist = scanned_distance;
//            }
//
//        } else { // node fails the check, will definitely not have significant m value
//            delete scanning_candidate;
//        }
//
//        scanning_candidate = next_entry;
//    }
//
//    if (nearest_slot != nullptr) { // puts the nearest node at the top
//        VoronoiNode* first_node = result_list->node;
//        result_list->node = nearest_slot->node; // list[0] gets the nearest node
//        nearest_slot->node = first_node; // list[nearest] gets the first node
//    }
//    return result_list;
}
void VoronoiGraph::BuildNearbyList(VQuadTree* branch) { // takes the running info on the best distance upper bound and adds the nodes descendant to this branch to the list of potential candidates
    if (!this->SplitValid(branch)) { // not split, this is a leaf node

        VoronoiNode* new_nearest = nullptr;

        NodeLinkedList* current_list_entry = branch->node_children; // iterate over all node children with magnitude testing only
        while (current_list_entry != nullptr) {
            VoronoiNode* current_node = current_list_entry->node; // for every node a child of this branch
            current_node->CalculateDist(this->recent_x, this->recent_y, this->gain);
            double relative_dist = this->current_bounding_mag-current_node->GetDist(); // from this node to the outer circle
            if (relative_dist > 0) { // beats the outer circle
                NodeLinkedList* new_candidate_entry = new NodeLinkedList();
                new_candidate_entry->node = current_node;
                new_candidate_entry->next = this->nearby_candidates; // prepend the new candidate
                this->nearby_candidates = new_candidate_entry;
                if (relative_dist >= this->band_width) { // beats the middle circle
                    this->current_bounding_mag = current_node->GetDist()+this->band_width; // now the outer circle is this dist plus out by band width
                    new_nearest = current_node;
                }
            }
            current_list_entry = current_list_entry->next;
        }
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
        int dx = this->recent_sort_x-branch->half_x; // and now this is the vector position of the sample center, with the branch as the origin
        int dy = this->recent_sort_y-branch->half_y;
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

