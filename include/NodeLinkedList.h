#ifndef NODELINKEDLIST_H
#define NODELINKEDLIST_H
#include "VoronoiNode.h"
class VoronoiNode;
#include <string>

struct NodeLinkedList {
    NodeLinkedList* next;
    VoronoiNode* node;

    static void Print(std::string header, NodeLinkedList* list, int indent);
    static void DeleteList(NodeLinkedList* list);
    static void DeleteNodes(NodeLinkedList* list);
    static void Append(VoronoiNode* node, NodeLinkedList** list_ref);
    static void RemoveResidence(VoronoiNode* node, NodeLinkedList** list_ref);
    static void RemoveTreeLocation(VoronoiNode* node, NodeLinkedList** list_ref);
    static int Length(NodeLinkedList* list);
};

#endif
