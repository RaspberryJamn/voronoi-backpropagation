#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H

#include <iostream>
#include <VoronoiNode.h>
#include "NodeLinkedList.h"

class VoronoiNode;

//struct NodeLinkedList { // unordered
//    NodeLinkedList* next;
//    VoronoiNode* node;
//};


class VoronoiQuadtree {
    public:
        VoronoiQuadtree(int x, int y, int w, int h, int dont_split_if_less_than_dim, int do_split_if_above_count);
        ~VoronoiQuadtree();

        void AddNode(VoronoiNode* node);
        bool RemoveNode(VoronoiNode* node); // returns false and does nothing if node is not in expected location
        void RelocateNode(VoronoiNode* node);

        NodeLinkedList* GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed); // closest node on top, the neg sq dist of everything else is no less than the nearest dist minus the band width

        NodeLinkedList* OrphanChildList(); // only for internal use
        void BuildNearbyList(double x, double y, int sort_x, int sort_y, double band_width, double gain, int sort_band_width, double* bounding_mag, int* bounding_box_radius, NodeLinkedList** headptr); // only for internal use
    private:
        NodeLinkedList* node_children;
        int total_children;
        VoronoiQuadtree* tree_children[4];

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
