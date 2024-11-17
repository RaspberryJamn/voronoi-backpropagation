#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
#include "NodeLinkedList.h"
#include "VoronoiNode.h"

struct VQuadTree {
    int total_children;
    int depth;
    int half_x;
    int half_y;
    NodeLinkedList* node_children;
    VQuadTree* tree_children[4];
    VQuadTree(int half_x, int half_y, int depth) : total_children(0), depth(depth), half_x(half_x), half_y(half_y), node_children(nullptr) {
        this->tree_children[0] = nullptr;
        this->tree_children[1] = nullptr;
        this->tree_children[2] = nullptr;
        this->tree_children[3] = nullptr;
    }

    static void Print(VQuadTree* tree, int indent);
};

class VoronoiGraph {
    public:
        VoronoiGraph();
        ~VoronoiGraph();

        void RespecTree(int x, int y, int w, int h, int max_depth, int critical_mass);
        void PrintTree();

        void AddNode(VoronoiNode* node);
        void RemoveNode(VoronoiNode* node);
        void UpdateNodePositions();

        NodeLinkedList* GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed); // closest node on top, the mag of everything else is at most the nearest dist plus the band width

        static RGBColor ForwardPassFromNearby(NodeLinkedList* nearby, double x, double y);
        void DoBackwardsPassFromNearby(NodeLinkedList* nearby, double x, double y, RGBColor image_sample);
        void UpdateAllGradients(double learning_rate);
//        NodeLinkedList* OrphanChildList(); // only for internal use
    private:
        VQuadTree* root;
        int x;
        int y;
        int w;
        int h;
        int max_depth;
        int critical_mass;
        NodeLinkedList* all_child_nodes;
        int total_child_count;

        double gain;
        double band_width;

        // GetNearby stuff {
        int sort_band_width;
        double recent_x;
        double recent_y;
        int recent_sort_x;
        int recent_sort_y;

        NodeLinkedList* nearby_candidates;
        double current_bounding_mag;
        int current_box_radius;
        // }

        void AddToChildren(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y);
        void AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y);

        void RemoveFromChildren(VoronoiNode* node, VQuadTree* branch);
        void ConsolidateChildLists(VQuadTree* branch);

        void BuildNearbyList(VQuadTree* branch);

        void DeleteTree(VQuadTree* branch);

        bool SplitValid(VQuadTree* branch);
};

#endif // VORONOIGRAPH_H
