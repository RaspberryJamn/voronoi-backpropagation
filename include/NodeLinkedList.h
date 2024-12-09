#ifndef NODELINKEDLIST_H
#define NODELINKEDLIST_H
#include "VoronoiNode.h"
class VoronoiNode;
#include <string>

// NODELINKEDLIST_FOREACH(NodeLinkedList* list, {code block}); // defines NodeLinkedList* current_slot and VoronoiNode* current_node

#define NODELINKEDLIST_FOREACH(list, block) do {                             \
    NodeLinkedList* macro_scanning_slot = list;                              \
    while (macro_scanning_slot != nullptr) {                                 \
        [[maybe_unused]] NodeLinkedList* current_slot = macro_scanning_slot; \
        [[maybe_unused]] VoronoiNode* current_node = current_slot->node;     \
        macro_scanning_slot = macro_scanning_slot->next;                     \
        block                                                                \
    }                                                                        \
} while (0)

struct NodeLinkedList {
    NodeLinkedList* next;
    VoronoiNode* node;
    NodeLinkedList() : next(nullptr), node(nullptr) {}
//    ~NodeLinkedList() : {} // maybe this helps?
//    NodeLinkedList(VoronoiNode* node) : next(nullptr), node(node) {}
//    NodeLinkedList(VoronoiNode* node, NodeLinkedList* next) : next(next), node(node) {}

    static void Print(std::string header, NodeLinkedList* list, int indent);
    static void PrintNodes(std::string header, NodeLinkedList* list, int indent);
    static void DeleteList(NodeLinkedList* list);
    static void DeleteNodes(NodeLinkedList* list);
    static void Append(VoronoiNode* node, NodeLinkedList** list_ref);
    static void RemoveGeneric(VoronoiNode* node, NodeLinkedList** list_ref);
    static void LinkAOntoB(NodeLinkedList** a, NodeLinkedList* b);
    static NodeLinkedList* Copy(NodeLinkedList* list);
//    static void AddResidenceSlot(VoronoiNode* node, NodeLinkedList** list_ref);
//    static void AddTreeSlot(VoronoiNode* node, NodeLinkedList** list_ref);
//    static void RemoveResidence(VoronoiNode* node, NodeLinkedList** list_ref);
//    static void RemoveTreeLocation(VoronoiNode* node, NodeLinkedList** list_ref);
    static int Length(NodeLinkedList* list);
    static bool Contains(NodeLinkedList* list, VoronoiNode* node);
};


#endif
