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
        PrintIndents(indent+1); std::cout << "[" << current_slot << "]: ";
        if (current_node != nullptr) {current_node->Print(indent+1);} else {std::cout << "null pointer";}
        std::cout << " => " << current_slot->next << std::endl;
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
    SDL_assert(node != nullptr);
    NodeLinkedList* new_first = new NodeLinkedList();
    new_first->node = node;
    new_first->next = *list_ref;
    *list_ref = new_first;
}

void NodeLinkedList::AddResidence(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList::Append(node, list_ref);
    node->SetResidence(*list_ref);
}

void NodeLinkedList::AddTreeSlot(VoronoiNode* node, NodeLinkedList** list_ref) {
    NodeLinkedList::Append(node, list_ref);
    node->SetTreeSlot(*list_ref);
}

void NodeLinkedList::RemoveResidence(VoronoiNode* node, NodeLinkedList** list_ref) {
//    SDL_assert(node != nullptr);
//    NodeLinkedList* head_slot = (*list_ref);
//    NodeLinkedList* residency_slot = node->GetResidence();
//    residency_slot->node = head_slot->node; // residency slot previously occupied by this node now has the first node moved in
//    residency_slot->node->SetResidence(residency_slot); // upon moving in, it gets the deed
//    node->SetResidence(nullptr); // and the old node gets its deed revoked
//
//    *list_ref = head_slot->next; // the official home listing lets go of the first entry, which should now be vacant
//    delete head_slot; // no more vacant entry

    SDL_assert(node != nullptr);
    NodeLinkedList* prev = nullptr;
    NodeLinkedList* current = *list_ref;

    while (current != nullptr) {
        if (current->node == node) {
            // Carefully unlink
            if (prev == nullptr) {
                *list_ref = current->next;
            } else {
                prev->next = current->next;
            }

            // Critically: clear all references
            node->SetResidence(nullptr);

            // Actually free the slot
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }

    // If we get here, node wasn't found
    SDL_assert(false); // Or handle more gracefully

}

void NodeLinkedList::RemoveTreeLocation(VoronoiNode* node, NodeLinkedList** list_ref) {
//    SDL_assert(node != nullptr);
//    NodeLinkedList* head_slot = (*list_ref); // pointer to first slot
//    NodeLinkedList* tree_slot = node->GetTreeSlot(); // the slot that references node
////    if (tree_slot == nullptr) {std::cout << "RemoveTreeLocation" << std::endl; return;}
//    tree_slot->node = head_slot->node; // tree slot previously referencing this node now points to the first node, which the first slot is also doing
//    tree_slot->node->SetTreeSlot(tree_slot); // the newly referenced node now updates its contacts
//    node->SetTreeSlot(nullptr); // the old node clears its contacts (if there was an ovelap in head slot and tree slot, well this node's unreferenced anyways)
//
//    *list_ref = head_slot->next; // the tree listing contains an out of date slot at its head, which it drops
//    delete head_slot; // and that slot is erased

    SDL_assert(node != nullptr);
    NodeLinkedList* prev = nullptr;
    NodeLinkedList* current = *list_ref;

    while (current != nullptr) {
        if (current->node == node) {
            // Carefully unlink
            if (prev == nullptr) {
                *list_ref = current->next;
            } else {
                prev->next = current->next;
            }

            // Critically: clear all references
            node->SetTreeSlot(nullptr);

            // Actually free the slot
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }

    // If we get here, node wasn't found
    SDL_assert(false); // Or handle more gracefully

}

int NodeLinkedList::Length(NodeLinkedList* list) {
    int result = 0;
    NODELINKEDLIST_FOREACH(list, {
        result++;
    });
    return result;
}

bool NodeLinkedList::Contains(NodeLinkedList* list, VoronoiNode* node) {
//    SDL_assert(node != nullptr);
    NODELINKEDLIST_FOREACH(list, {
        if (current_node == node) {
            return true;
        }
    });
    return false;
}
