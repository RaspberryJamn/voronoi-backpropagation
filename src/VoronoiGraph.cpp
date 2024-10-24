#include "VoronoiGraph.h"

VoronoiGraph::VoronoiGraph(int x, int y, int w, int h, int dont_split_if_less_than_dim, int do_split_if_above_count, VoronoiGraph* parent) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->min_dim = dont_split_if_less_than_dim;
    this->max_count = do_split_if_above_count;

    this->half_x = this->x + this->w/2;
    this->half_y = this->y + this->h/2;

    this->node_children = nullptr;
    this->total_children = 0;

    this->parent_tree = parent;

    this->tree_children[0] = nullptr;
    this->tree_children[1] = nullptr;
    this->tree_children[2] = nullptr;
    this->tree_children[3] = nullptr;
}

VoronoiGraph::~VoronoiGraph() {
    if (this->total_children <= this->max_count) {
        this->DeleteAllNodes();
    } else {
        delete this->tree_children[0];
        delete this->tree_children[1];
        delete this->tree_children[2];
        delete this->tree_children[3];
        this->tree_children[0] = nullptr;
        this->tree_children[1] = nullptr;
        this->tree_children[2] = nullptr;
        this->tree_children[3] = nullptr;
        this->total_children = 0;
    }
    this->parent_tree = nullptr;
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->min_dim = 0;
    this->max_count = 0;
    this->half_x = 0;
    this->half_y = 0;

}
void VoronoiGraph::DeleteAllNodes() {
    NodeLinkedList* current_list_entry = this->node_children;
    while (current_list_entry != nullptr) {
        NodeLinkedList* next_list_entry = current_list_entry->next;
        delete current_list_entry->node;
        delete current_list_entry;
        current_list_entry = next_list_entry;
    }
    this->node_children = nullptr;
    this->total_children = 0;
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    if (this->total_children <= this->max_count) {
        NodeLinkedList* new_first = new NodeLinkedList();
        new_first->next = this->node_children;
        new_first->node = node;
        new_first->node->SetParent(this, this->x, this->x+this->w, this->y, this->y+this->h);

        this->node_children = new_first;
        this->total_children++;

        if (this->w >= this->min_dim) {
            if (this->total_children > this->max_count) {
                this->tree_children[0] = new VoronoiGraph(this->x     , this->y     , this->half_x-this->x          , this->half_y-this->y          , this->min_dim, this->max_count, this);
                this->tree_children[1] = new VoronoiGraph(this->half_x, this->y     , this->w-(this->half_x-this->x), this->half_y-this->y          , this->min_dim, this->max_count, this);
                this->tree_children[2] = new VoronoiGraph(this->x     , this->half_y, this->half_x-this->x          , this->h-(this->half_y-this->y), this->min_dim, this->max_count, this);
                this->tree_children[3] = new VoronoiGraph(this->half_x, this->half_y, this->w-(this->half_x-this->x), this->h-(this->half_y-this->y), this->min_dim, this->max_count, this);

                NodeLinkedList* current_list_entry = this->node_children;
                while (current_list_entry != nullptr) {
                    NodeLinkedList* next_list_entry = current_list_entry->next;
                    this->InsertToChildren(current_list_entry->node);
                    delete current_list_entry;
                    current_list_entry = next_list_entry;
                }
                this->node_children = nullptr;
            }
        }
    } else {
        this->InsertToChildren(node);
        this->total_children++;
    }
}
void VoronoiGraph::InsertToChildren(VoronoiNode* node) {
    if (node->GetSortingPosY() < this->half_y) {
        if (node->GetSortingPosX() < this->half_x) {
            this->tree_children[0]->AddNode(node);
        } else {
            this->tree_children[1]->AddNode(node);
        }
    } else {
        if (node->GetSortingPosX() < this->half_x) {
            this->tree_children[2]->AddNode(node);
        } else {
            this->tree_children[3]->AddNode(node);
        }
    }
}

void VoronoiGraph::RemoveNode(VoronoiNode* node) {
    VoronoiGraph* current_container = node->GetParent();
    if (current_container == nullptr) { // does not have a parent
        return;
    }
    NodeLinkedList* current_list_entry = current_container->GetChildren();
    while (current_list_entry != nullptr) {
        if (current_list_entry->node == node) { // scanned onto the node in question
            NodeLinkedList* first_slot = current_container->GetChildren(); // get reference to list[0]
            current_list_entry->node = first_slot->node; // data of list[0] overwrites the current slot, list[0].node has one reference of redundancy
            current_container->DropFirstListNode(); // list[0] becomes otherwise unreferenced, list[0].node is back to only one valid reference
            delete first_slot;
            break;
        }
        current_list_entry = current_list_entry->next;
    }
    if (current_list_entry == nullptr) { // node not contained within its parent branch??
        return;
    }

    current_container = current_container->GetParent();
    while (current_container != nullptr) { // go up until you find the root tree
        current_container->DecrementChildCount();
        // (any child has critical mass) -> (parent has critical mass), therefore (parent does not have critical mass) -> (no child has critical mass)
        if (current_container->GetChildCount() <= current_container->GetChildSplitThreshold()) {
            current_container->ConsolidateChildLists();
        }
        current_container = current_container->GetParent();
    }
    return;
}
void VoronoiGraph::ConsolidateChildLists() {
    NodeLinkedList* growing_start = this->tree_children[0]->OrphanChildList();
    delete this->tree_children[0];
    this->tree_children[0] = nullptr;

    for (int i = 1; i <= 3; i++) {
        NodeLinkedList* prepend_start = this->tree_children[i]->OrphanChildList();
        delete this->tree_children[i];
        this->tree_children[i] = nullptr;

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

    this->node_children = growing_start;
}
void VoronoiGraph::DropFirstListNode() {
    this->node_children = this->node_children->next;
    this->total_children--;
}
void VoronoiGraph::DecrementChildCount() {
    this->total_children--;
}
int VoronoiGraph::GetChildCount() {
    return this->total_children;
}
int VoronoiGraph::GetChildSplitThreshold() {
    return this->max_count;
}
NodeLinkedList* VoronoiGraph::GetChildren() {
    return this->node_children;
}
VoronoiGraph* VoronoiGraph::GetParent() {
    return this->parent_tree;
}
NodeLinkedList* VoronoiGraph::OrphanChildList() {
    NodeLinkedList* children = this->node_children;
    this->node_children = nullptr;
    return children;
}

void VoronoiGraph::RelocateNode(VoronoiNode* node) {
    this->RemoveNode(node);
    this->AddNode(node);
}
