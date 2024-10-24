#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H

#include <iostream>
#include <VoronoiNode.h>

typedef struct NodeLinkedList { // unordered
    //NodeLinkedList* prev;
    NodeLinkedList* next;
    VoronoiNode* node;
}; //NodeLinkedList;

class VoronoiGraph {
    public:
        VoronoiGraph(int x, int y, int w, int h, int dont_split_if_less_than_dim, int do_split_if_above_count, VoronoiGraph* parent);
        ~VoronoiGraph();

        void AddNode(VoronoiNode* node);
        void RemoveNode(VoronoiNode* node); // not specifically bound to this tree object
        void RelocateNode(VoronoiNode* node);

        NodeLinkedList* GetNearby(int x, int y, double band_width, VoronoiNode* seed);

        int GetChildCount(); // only for internal use {
        int GetChildSplitThreshold();
        NodeLinkedList* GetChildren();
        VoronoiGraph* GetParent();
        void DropFirstListNode();
        void DecrementChildCount();
        void ConsolidateChildLists();
        NodeLinkedList* OrphanChildList(); // }
    private:
        NodeLinkedList* node_children;
        int total_children;
        VoronoiGraph* tree_children[4];
        VoronoiGraph* parent_tree;

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
};


#endif // VORONOIGRAPH_H
