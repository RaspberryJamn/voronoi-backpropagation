#ifndef NODELINKEDLIST_H
#define NODELINKEDLIST_H
//#include "VoronoiNode.h"

class VoronoiNode;

struct NodeLinkedList { // unordered
    NodeLinkedList* next;
    VoronoiNode* node;
};

namespace NodeLinkedListNSP {
    void Print(std::string header, NodeLinkedList* list);
    void DeleteList(NodeLinkedList* list);
    void DeleteNodes(NodeLinkedList* list);
    void Append(VoronoiNode* node, NodeLinkedList** list_ref);
}

#endif
