#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H

#include <iostream>

typedef struct NodeLinkedList { // unordered
    //NodeLinkedList* prev;
    NodeLinkedList* next;
    VoronoiNode* node
} //NodeLinkedList;

class VoronoiGraph {
    public:
        VoronoiGraph(int x, int y, int w, int h);
        ~VoronoiGraph();

        void AddNode(VoronoiNode* node);
        void RemoveNode(VoronoiNode* node);

        NodeLinkedList* GetNearby(int x, int y, double);

    private:
        NodeLinkedList* node_children;
        VoronoiGraph* tree_children[4];
        int x;
        int y;
        int w;
        int h;
};


#endif // VORONOIGRAPH_H
