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
    NODELINKEDLIST_FOREACH(list, {
        PrintIndents(indent+1); std::cout << current_slot << ":{next:" << current_slot->next << ", node:" << current_node << ", node_dist:" << current_node->GetDist() << "}" << std::endl;
    });
    PrintIndents(indent); std::cout << "}" << std::endl;
}

void NodeLinkedList::PrintNodes(std::string header, NodeLinkedList* list, int indent) {
    std::cout << header;
    if (list == nullptr) {
        std::cout << "[]" << std::endl;
        return;
    }
    std::cout << "{" << std::endl;
    NODELINKEDLIST_FOREACH(list, {
        RGBColor c = current_node->GetColor();
        PrintIndents(indent+1); std::cout << current_node << ":{" << std::endl;
        PrintIndents(indent+2); std::cout << "sort_(x,y): (" << current_node->GetSortingPosX() << "," << current_node->GetSortingPosY() << ")," << std::endl;
        PrintIndents(indent+2); std::cout << "precise_(x,y): (" << current_node->GetX() << "," << current_node->GetY() << ")," << std::endl;
        PrintIndents(indent+2); std::cout << "color: [" << c.r << "," << c.g << "," << c.b << "]," << std::endl;
        PrintIndents(indent+2); std::cout << "dist:" << current_node->GetDist() << std::endl;
        PrintIndents(indent+1); std::cout << "}" << std::endl;
    });
    PrintIndents(indent); std::cout << "}" << std::endl;
}

void NodeLinkedList::DeleteList(NodeLinkedList* list) {
    NODELINKEDLIST_FOREACH(list, {
        delete current_slot;
    });
}

void NodeLinkedList::DeleteNodes(NodeLinkedList* list) {
    NODELINKEDLIST_FOREACH(list, {
        delete current_node;
        delete current_slot;
    });
}

void NodeLinkedList::Append(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* new_first = new NodeLinkedList();//node);//
    new_first->node = node;
    new_first->next = *list_ref;
    *list_ref = new_first;
}

void NodeLinkedList::RemoveResidence(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* head_slot = (*list_ref);
    NodeLinkedList* residency_slot = node->GetResidence();
    residency_slot->node = head_slot->node; // residency slot previously occupied by this node now has the first node moved in
    residency_slot->node->SetResidence(residency_slot); // upon moving in, it gets the deed
//    head_ptr_copy->node = node; // (conceptually its a swap)
    node->SetResidence(nullptr); // and the old node gets its deed revoked

    *list_ref = head_slot->next; // the official home listing lets go of the first entry, which should now be vacant
    delete head_slot; // no more vacant entry
}

void NodeLinkedList::RemoveTreeLocation(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList* head_slot = (*list_ref); // pointer to first slot
    NodeLinkedList* tree_slot = node->GetTreeSlot(); // the slot that references node
    tree_slot->node = head_slot->node; // tree slot previously referencing this node now points to the first node
    tree_slot->node->SetTreeSlot(tree_slot); // the newly referenced node now updates its contacts
    node->SetTreeSlot(nullptr); // the old node clears its contacts

    *list_ref = head_slot->next; // the tree listing contains an out of date slot at its head, which it drops
    delete head_slot; // and that slot is erased
}

int NodeLinkedList::Length(NodeLinkedList* list) {
    int result = 0;
    NODELINKEDLIST_FOREACH(list, {
        result++;
    });
    return result;
}

bool NodeLinkedList::Contains(NodeLinkedList* list, VoronoiNode* node) {
    NODELINKEDLIST_FOREACH(list, {
        if (current_node == node) {
            return true;
        }
    });
    return false;
}
