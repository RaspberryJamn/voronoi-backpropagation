#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H

#include <iostream>
#include <VoronoiNode.h>

class VoronoiNode;

struct NodeLinkedList { // unordered
    //NodeLinkedList* prev;
    NodeLinkedList* next;
    VoronoiNode* node;
}; //NodeLinkedList;


class VoronoiGraph {
    public:
        VoronoiGraph(int x, int y, int w, int h, int dont_split_if_less_than_dim, int do_split_if_above_count);
        ~VoronoiGraph();

        void AddNode(VoronoiNode* node);
        bool RemoveNode(VoronoiNode* node); // returns false and does nothing if node is not in expected location
        void RelocateNode(VoronoiNode* node);

        NodeLinkedList* GetNearby(int x, int y, double band_width, VoronoiNode* seed);

        NodeLinkedList* OrphanChildList(); // only for internal use
    private:
        NodeLinkedList* node_children;
        int total_children;
        VoronoiGraph* tree_children[4];

        int x;
        int y;
        int w;
        int h;
        int min_dim;
        int max_count;
        int half_x;
        int half_y;


        void DeleteAllNodes();
        void InsertToChildren(VoronoiNode* node);
        void ConsolidateChildLists();
};


#endif // VORONOIGRAPH_H
