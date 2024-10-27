#include "VoronoiGraph.h"
#include <cmath>

VoronoiGraph::VoronoiGraph(int x, int y, int w, int h, int dont_split_if_less_than_dim, int do_split_if_above_count) {
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
        new_first->node->SetBounds(this->x, this->x+this->w, this->y, this->y+this->h);

        this->node_children = new_first;
        this->total_children++;

        if (this->w >= this->min_dim) {
            if (this->total_children > this->max_count) {
                this->tree_children[0] = new VoronoiGraph(this->x     , this->y     , this->half_x-this->x          , this->half_y-this->y          , this->min_dim, this->max_count);
                this->tree_children[1] = new VoronoiGraph(this->half_x, this->y     , this->w-(this->half_x-this->x), this->half_y-this->y          , this->min_dim, this->max_count);
                this->tree_children[2] = new VoronoiGraph(this->x     , this->half_y, this->half_x-this->x          , this->h-(this->half_y-this->y), this->min_dim, this->max_count);
                this->tree_children[3] = new VoronoiGraph(this->half_x, this->half_y, this->w-(this->half_x-this->x), this->h-(this->half_y-this->y), this->min_dim, this->max_count);

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

bool VoronoiGraph::RemoveNode(VoronoiNode* node) {
    if (this->total_children > this->max_count) { // is branch
        bool contains_node;
        if (node->GetSortingPosY() < this->half_y) {
            if (node->GetSortingPosX() < this->half_x) {
                contains_node = this->tree_children[0]->RemoveNode(node);
            } else {
                contains_node = this->tree_children[1]->RemoveNode(node);
            }
        } else {
            if (node->GetSortingPosX() < this->half_x) {
                contains_node = this->tree_children[2]->RemoveNode(node);
            } else {
                contains_node = this->tree_children[3]->RemoveNode(node);
            }
        }
        if (contains_node) {
            this->total_children--;
            if (this->total_children <= this->max_count) { // (any child has critical mass) -> (parent has critical mass), therefore (parent does not have critical mass) -> (no child has critical mass)
                this->ConsolidateChildLists();
            }
        }
        return contains_node;
    } else { // is leaf

        NodeLinkedList* current_slot = this->node_children;
        while (current_slot != nullptr) {
            if (current_slot->node == node) { // scanned onto the node in question
                NodeLinkedList* first_slot = this->node_children; // get reference to list[0]
                current_slot->node = first_slot->node; // data of list[0] overwrites the current slot, list[0].node has one reference of redundancy
                this->node_children = this->node_children->next; // list[0] becomes otherwise unreferenced, list[0].node is back to only one valid reference
                delete first_slot;
                this->total_children--;
                return true; // we found it, all good
            }
            current_slot = current_slot->next;
        }
        return false; // node not contained within the leaf node it's allegedly located in
    }
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
NodeLinkedList* VoronoiGraph::OrphanChildList() {
    NodeLinkedList* children = this->node_children;
    this->node_children = nullptr;
    return children;
}

void VoronoiGraph::RelocateNode(VoronoiNode* node) {
    this->RemoveNode(node);
    node->UpdateSortingPos();
    this->AddNode(node);
}

NodeLinkedList* VoronoiGraph::GetNearby(double x, double y, double band_width, VoronoiNode* seed) {
    int sort_center_x = (int)x;
    int sort_center_y = (int)y;
    int sort_band_width = (int)(std::ceil(band_width));

    double bounding_mag = 999999999.0;// whatever
    int bounding_box_radius = 999999999 + sort_band_width;

    if (seed != nullptr) {
        seed->UpdateDist(x, y);
        seed->UpdateSortingDist();
        bounding_mag = seed->GetDist();
        bounding_box_radius = seed->GetSortingDist() + sort_band_width;
    }

    NodeLinkedList* head = nullptr;

    this->BuildNearbyList(x, y, sort_center_x, sort_center_y, band_width, sort_band_width, &bounding_mag, &bounding_box_radius, &head);

    NodeLinkedList* result_list = nullptr;

    while (head != nullptr) { // consume the head, commandeer everything that passes the most recent standards into result_list
        NodeLinkedList* next_entry = head->next;
        if (head->node->GetDist() < bounding_mag) { // passes the check
            head->next = result_list;
            result_list = head;
        } else { // fails the check
            delete head;
        }
        head = next_entry;
    }

    return result_list;
}

void VoronoiGraph::BuildNearbyList(double x, double y, int sort_x, int sort_y, double band_width, int sort_band_width, double* bounding_mag, int* bounding_box_radius, NodeLinkedList** headptr) {
    if (this->total_children <= this->max_count) {

        double max_dist = *bounding_mag;
        VoronoiNode* new_nearest = nullptr;

        NodeLinkedList* current_list_entry = this->node_children;
        while (current_list_entry != nullptr) {
            VoronoiNode* current_node = current_list_entry->node;
            current_node->UpdateDist(x, y);
            double relative_dist = current_node->GetDist()-max_dist;
            if (relative_dist > 0) { // beats the outer circle
                NodeLinkedList* new_entry = new NodeLinkedList();
                new_entry->node = current_node;
                new_entry->next = *headptr;
                *headptr = new_entry;
                if (relative_dist > band_width) { // beats the middle circle
                    max_dist = relative_dist+max_dist-band_width;
                    new_nearest = current_node;
                }
            }
            current_list_entry = current_list_entry->next;
        }
        if (new_nearest != nullptr) {
            *bounding_mag = max_dist;
            new_nearest->UpdateSortingDist();
            *bounding_box_radius = new_nearest->GetSortingDist() + sort_band_width;
        }

    } else {
        size_t search_1;
        size_t search_2;
        size_t search_3;
        size_t search_4;
        bool search_x_then_y;
        int dx = sort_x-this->half_x; // from vertically dividing line to sample
        int dy = sort_y-this->half_y; // from horizontally dividing line to sample
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

        this->tree_children[search_1]->BuildNearbyList(x, y, sort_x, sort_y, band_width, sort_band_width, bounding_mag, bounding_box_radius, headptr);

        int box_radius = *bounding_box_radius;
        if (search_x_then_y) { // search x first
            if ((box_radius < dx) || (box_radius < -dx)) { // center line further than bounding box
                return;
            }
        } else { // search y first
            if ((box_radius < dy) || (box_radius < -dy)) { // horizon line further than bounding box
                return;
            }
        }
        this->tree_children[search_2]->BuildNearbyList(x, y, sort_x, sort_y, band_width, sort_band_width, bounding_mag, bounding_box_radius, headptr);

        box_radius = *bounding_box_radius;
        if (search_x_then_y) { // search y second
            if ((box_radius < dy) || (box_radius < -dy)) { // horizon line further than bounding box
                return;
            }
        } else { // search x second
            if ((box_radius < dx) || (box_radius < -dx)) { // center line further than bounding box
                return;
            }
        }
        this->tree_children[search_3]->BuildNearbyList(x, y, sort_x, sort_y, band_width, sort_band_width, bounding_mag, bounding_box_radius, headptr);

        if ((dx*dx+dy*dy) > -*bounding_mag) { // center point further than bounding circle
            return;
        }
        this->tree_children[search_4]->BuildNearbyList(x, y, sort_x, sort_y, band_width, sort_band_width, bounding_mag, bounding_box_radius, headptr);
    }
}
