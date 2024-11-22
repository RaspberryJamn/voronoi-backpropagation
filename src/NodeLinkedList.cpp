#include "NodeLinkedList.h"
#include <iostream>


/*
#define NODELINKEDLIST_FOREACH(list, block) do {                             \ // outdated, lives in NodeLinkedList.h now
    NodeLinkedList* macro_scanning_slot = list;                              \
    while (macro_scanning_slot != nullptr) {                                 \
        [[maybe_unused]] NodeLinkedList* current_slot = macro_scanning_slot; \
        macro_scanning_slot = macro_scanning_slot->next;                     \
        [[maybe_unused]] VoronoiNode* current_node = current_slot->node;     \
        block                                                                \
    }                                                                        \
} while (0)
*/

void PrintIndents(int indent);

void NodeLinkedList::Print(std::string header, NodeLinkedList* list, int indent) {
    std::cout << header;
    if (list == nullptr) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "{" << std::endl;
//    NodeLinkedList* current_slot = list;
//    while (current_slot != nullptr) {
//        PrintIndents(indent+1); std::cout << current_slot << ":{next:" << current_slot->next << ", node:" << current_slot->node << ", node_dist:" << current_slot->node->GetDist() << "}" << std::endl;
//        current_slot = current_slot->next;
//    }
    NODELINKEDLIST_FOREACH(list, {
        PrintIndents(indent+1); std::cout << current_slot << ":{next:" << current_slot->next << ", node:" << current_slot->node << ", node_dist:" << current_slot->node->GetDist() << "}" << std::endl;
    });
    PrintIndents(indent); std::cout << "}" << std::endl;
}

void NodeLinkedList::DeleteList(NodeLinkedList* list) {
//    NodeLinkedList* current_slot = list;
//    while (current_slot != nullptr) {
//        NodeLinkedList* next_slot = current_slot->next;
//        delete current_slot;
//        current_slot = next_slot;
//    }
    NODELINKEDLIST_FOREACH(list, {
        delete current_slot;
    });
}

void NodeLinkedList::DeleteNodes(NodeLinkedList* list) {
//    NodeLinkedList* current_slot = list;
//    while (current_slot != nullptr) {
//        NodeLinkedList* next_slot = current_slot->next;
//        delete current_slot->node;
//        delete current_slot;
//        current_slot = next_slot;
//    }
    NODELINKEDLIST_FOREACH(list, {
        delete current_node;
        delete current_slot;
    });
}

void NodeLinkedList::Append(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* new_first = new NodeLinkedList();
    new_first->next = *list_ref;
    new_first->node = node;
    *list_ref = new_first;
}

void NodeLinkedList::RemoveResidence(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* head_ptr_copy = (*list_ref);
    NodeLinkedList* residency_ptr = node->GetResidence();
    residency_ptr->node = head_ptr_copy->node; // residency slot previously occupied by this node now has the first node moved in
    residency_ptr->node->SetResidence(residency_ptr); // upon moving in, it gets the deed
//    head_ptr_copy->node = node; // (conceptually its a swap)
    node->SetResidence(nullptr); // and the old node gets its deed revoked

    *list_ref = head_ptr_copy->next; // the official home listing lets go of the first entry, which should now be vacant
    delete head_ptr_copy; // no more vacant entry
}

void NodeLinkedList::RemoveTreeLocation(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* head_ptr_copy = (*list_ref);
    NodeLinkedList* tree_ptr = node->GetTreeSlot(); // tree slot, not residence
    tree_ptr->node = head_ptr_copy->node; // tree slot previously referencing this node now points to the first node
    tree_ptr->node->SetTreeSlot(tree_ptr); // the newly referenced node now updates its contacts
    node->SetTreeSlot(nullptr); // the old node clears its contacts

    *list_ref = tree_ptr->next; // the tree listing contains an out of date slot at its head, which it drops
    delete head_ptr_copy; // and that slot is erased
}

int NodeLinkedList::Length(NodeLinkedList* list) {
    int result = 0;
//    NodeLinkedList* current_slot = list;
//    while (current_slot != nullptr) {
//        result++;
//        current_slot = current_slot->next;
//    }
    NODELINKEDLIST_FOREACH(list, {
        result++;
    });
    return result;
}

bool NodeLinkedList::Contains(NodeLinkedList* list, VoronoiNode* node) {
//    NodeLinkedList* current_slot = list;
//    while (current_slot != nullptr) {
//        if (current_slot->node == node) {
//            return true;
//        }
//        current_slot = current_slot->next;
//    }
    NODELINKEDLIST_FOREACH(list, {
        if (current_node == node) {
            return true;
        }
    });
    return false;
}
