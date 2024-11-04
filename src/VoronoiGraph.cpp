#include "VoronoiGraph.h"

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

    NodeLinkedListNSP::DeleteNodes(this->all_child_nodes);
    this->all_child_nodes = nullptr;
    this->total_child_count = 0;

    this->gain = 0;

    NodeLinkedListNSP::DeleteList(this->nearby_candidates);
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
        this->DeleteTree(branch->tree_children[0]);
        this->DeleteTree(branch->tree_children[1]);
        this->DeleteTree(branch->tree_children[2]);
        this->DeleteTree(branch->tree_children[3]);
    } else {
        NodeLinkedListNSP::DeleteList(branch->node_children);
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

    this->DeleteTree(this->root); // all but half x and y is in a ready position
//    this->root = nullptr;

    this->root = new VQuadTree();
    this->root->half_x = 0;
    this->root->half_y = 0;
    this->root->node_children = nullptr;
    this->root->total_children = 0;
    this->root->depth = 0;
    this->root->tree_children[0] = nullptr;
    this->root->tree_children[1] = nullptr;
    this->root->tree_children[2] = nullptr;
    this->root->tree_children[3] = nullptr;
    this->root->half_x = this->x+this->w/2;
    this->root->half_y = this->y+this->h/2;

    NodeLinkedListNSP::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;

    NodeLinkedList* current_slot = this->all_child_nodes;
    while (current_slot != nullptr) {
        this->AddNode(current_slot->node);
        current_slot = current_slot->next;
    }
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    NodeLinkedListNSP::Append(node, &this->all_child_nodes);
    node->GetSortingDist();
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->AddToChildren(node, this->root);
}

void VoronoiGraph::AddToChildren(VoronoiNode* node, VQuadTree* branch) {
    if (branch->total_children <= this->critical_mass) {
        NodeLinkedListNSP::Append(node, &branch->node_children);
        branch->total_children++;

        if ((branch->total_children > this->critical_mass) && (branch->depth)) {

            NodeLinkedList* current_slot = branch->node_children;
            while (current_slot != nullptr) {
                this->AddNode(current_slot->node);
                current_slot = current_slot->next;
            }
        }

    }
}
