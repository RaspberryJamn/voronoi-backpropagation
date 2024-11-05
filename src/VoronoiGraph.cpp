#include "VoronoiGraph.h"
#include <cmath>
#include <iostream>

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

    this->DeleteTree(this->root);
    this->root = new VQuadTree(this->x+this->w/2, this->y+this->h/2, 0);

    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;



    NodeLinkedList* current_slot = this->all_child_nodes;
    while (current_slot != nullptr) {
        VoronoiNode* node = current_slot->node;
        node->GetSortingDist();
        this->recent_sort_x = node->GetSortingPosX();
        this->recent_sort_y = node->GetSortingPosY();
        this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
        current_slot = current_slot->next;
    }
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    NodeLinkedList::Append(node, &this->all_child_nodes);
    node->CalculateSortingPos();
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
}
void VoronoiGraph::AddToChildren(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y) {
    if (branch->total_children <= this->critical_mass) {
        NodeLinkedList::Append(node, &branch->node_children);
        branch->total_children++;

        if ((branch->total_children > this->critical_mass) && (branch->depth <= this->max_depth)) {

            branch->tree_children[0] = new VQuadTree((min_x+branch->half_x)/2, (min_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[1] = new VQuadTree((max_x+branch->half_x)/2, (min_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[2] = new VQuadTree((min_x+branch->half_x)/2, (max_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[3] = new VQuadTree((max_x+branch->half_x)/2, (max_y+branch->half_y)/2, branch->depth+1);

            NodeLinkedList* current_slot = branch->node_children;
            while (current_slot != nullptr) {
                this->AddToChildrenSplit(node, branch, min_x, min_y, max_x, max_y);
                current_slot = current_slot->next;
            }
        }

    } else {
        this->AddToChildrenSplit(node, branch, min_x, min_y, max_x, max_y);
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
    NodeLinkedList::Remove(node, &this->all_child_nodes);
    node->CalculateSortingPos();
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->RemoveFromChildren(node, this->root);
}
void VoronoiGraph::RemoveFromChildren(VoronoiNode* node, VQuadTree* branch) {
    if (branch->total_children <= this->critical_mass) {
        NodeLinkedList* current_slot = branch->node_children;
        while (current_slot != nullptr) {
            if (current_slot->node == node) { // scanned onto the node in question
                NodeLinkedList* first_slot = branch->node_children; // get reference to list[0]
                current_slot->node = first_slot->node; // data of list[0] overwrites the current slot, list[0].node has one reference of redundancy
                branch->node_children = first_slot->next; // list[0] becomes otherwise unreferenced, list[0].node is back to only one valid reference
                delete first_slot;
                branch->total_children--;
                return; // we found it, all good
            }
            current_slot = current_slot->next;
        }
    } else {
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
    this->sort_band_width = (int)(std::ceil(this->band_width));
    this->gain = gain;

    if (seed != nullptr) {
        seed->CalculateDist(x, y, gain);
        seed->CalculateSortingDist();
        this->current_bounding_mag = seed->GetDist() + this->band_width;
        this->current_box_radius = seed->GetSortingDist() + this->sort_band_width;
    } else {
        VoronoiNode* arbitrary_seed = this->all_child_nodes->node;
        arbitrary_seed->CalculateDist(x, y, gain);
        arbitrary_seed->CalculateSortingDist();
        this->current_bounding_mag = arbitrary_seed->GetDist() + band_width;
        this->current_box_radius = arbitrary_seed->GetSortingDist() + sort_band_width;
    }


    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;

        this->BuildNearbyList(this->root);

    NodeLinkedList* candidate_list = this->nearby_candidates;
    this->nearby_candidates = nullptr;

    if (candidate_list == nullptr) {
        std::cout << "erm" << std::endl;
    }

    NodeLinkedList* nearest_slot = nullptr;
    double nearest_dist = candidate_list->node->GetDist();

    while (candidate_list != nullptr) { // consume the head, commandeer everything that passes the most recent standards into this->nearby_candidates
        NodeLinkedList* next_entry = candidate_list->next;
        if (candidate_list->node->GetDist() >= this->current_bounding_mag) { // passes the check, may have significant m value
            candidate_list->next = this->nearby_candidates;
            this->nearby_candidates = candidate_list;
            if (candidate_list->node->GetDist() < nearest_dist) {
                nearest_slot = candidate_list;
            }
        } else { // fails the check, will not have significant m value
            delete candidate_list;
        }
        candidate_list = next_entry;
    }
    if (nearest_slot != nullptr) { // puts the nearest node at the top
        VoronoiNode* copy_node = this->nearby_candidates->node;
        this->nearby_candidates->node = nearest_slot->node;
        nearest_slot->node = copy_node;
    }
    return this->nearby_candidates;
}
void VoronoiGraph::BuildNearbyList(VQuadTree* branch) {
    if (branch->total_children <= this->critical_mass) {

        VoronoiNode* new_nearest = nullptr;

        NodeLinkedList* current_list_entry = branch->node_children;
        while (current_list_entry != nullptr) {
            VoronoiNode* current_node = current_list_entry->node;
            current_node->CalculateDist(this->recent_x, this->recent_x, this->gain);
            double relative_dist = this->current_bounding_mag-current_node->GetDist(); // from this node to the outer circle
            if (relative_dist > 0) { // beats the outer circle
                NodeLinkedList* new_entry = new NodeLinkedList();
                new_entry->node = current_node;
                new_entry->next = this->nearby_candidates;
                this->nearby_candidates = new_entry;
                if (relative_dist >= this->band_width) { // beats the middle circle
                    this->current_bounding_mag = current_node->GetDist()+this->band_width; // now the outer circle is this dist plus out by band width
                    new_nearest = current_node;
                }
            }
            current_list_entry = current_list_entry->next;
        }
        if (new_nearest != nullptr) {
            new_nearest->CalculateSortingDist();
            this->current_box_radius = new_nearest->GetSortingDist() + this->sort_band_width;
        }

    } else {
        size_t search_1;
        size_t search_2;
        size_t search_3;
        size_t search_4;
        bool search_x_then_y;
        int dx = this->recent_sort_x-branch->half_x; // from vertically dividing line to sample
        int dy = this->recent_sort_y-branch->half_y; // from horizontally dividing line to sample
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
            if ( dy < dx ) {
                search_2 = 1;
                search_3 = 2;
                search_x_then_y = true;
            } else {
                search_2 = 2;
                search_3 = 1;
                search_x_then_y = false;
            }
        } else {
            if ( dy < -dx ) {
                search_2 = 0;
                search_3 = 3;
                search_x_then_y = false;
            } else {
                search_2 = 3;
                search_3 = 0;
                search_x_then_y = true;
            }
        }

        this->BuildNearbyList(branch->tree_children[search_1]);

        if (search_x_then_y) { // search x first
            if ((this->current_box_radius < dx) || (this->current_box_radius < -dx)) { // center line further than bounding box
                return;
            }
        } else { // search y first
            if ((this->current_box_radius < dy) || (this->current_box_radius < -dy)) { // horizon line further than bounding box
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_2]);

        if (search_x_then_y) { // search y second
            if ((this->current_box_radius < dy) || (this->current_box_radius < -dy)) { // horizon line further than bounding box
                return;
            }
        } else { // search x second
            if ((this->current_box_radius < dx) || (this->current_box_radius < -dx)) { // center line further than bounding box
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_3]);

        if ((dx*dx+dy*dy) > this->current_bounding_mag) { // center point further than bounding circle
            return;
        }
        this->BuildNearbyList(branch->tree_children[search_4]);
    }
}

