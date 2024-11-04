#include "NodeLinkedList.h"

void NodeLinkedListNSP::Print(std::string header, NodeLinkedList* list) {
    std::cout << header;
    if (list == nullptr) {
        std::cout << "empty list" << std::endl;
        return;
    }
    std::cout << "{";
    NodeLinkedList* current_slot = list;
    while (current_slot != nullptr) {
        std::cout << std::endl << "  " << current_slot << ":{next:" << current_slot->next << ", node:" << current_slot->node << ", node_dist:" << current_slot->node->GetDist() << "}";
        current_slot = current_slot->next;
    }
    std::cout << std::endl << "}" << std::endl;
}

void NodeLinkedListNSP::DeleteList(NodeLinkedList* list) {
    NodeLinkedList* current_slot = list;
    while (current_slot != nullptr) {
        NodeLinkedList* next_slot = current_slot->next;
        delete current_slot;
        current_slot = next_slot;
    }
}

void NodeLinkedListNSP::DeleteNodes(NodeLinkedList* list) {
    NodeLinkedList* current_slot = list;
    while (current_slot != nullptr) {
        NodeLinkedList* next_slot = current_slot->next;
        delete current_slot->node;
        delete current_slot;
        current_slot = next_slot;
    }
}

void NodeLinkedListNSP::Append(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* new_first = new NodeLinkedList();
    new_first->next = *list_ref;
    new_first->node = node;
    *list_ref = new_first;
}
