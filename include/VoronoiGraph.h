#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
#include "NodeLinkedList.h"
#include "VoronoiNode.h"
#include "ErrorLogger.h"
#include <SDL.h>

struct VQuadTree {
    int total_children;
    int depth;
    int half_x;
    int half_y;
    int min_x;
    int min_y;
    int max_x;
    int max_y;
    NodeLinkedList* node_children;
    VQuadTree* tree_children[4];
//    VQuadTree(int half_x, int half_y, int depth) : total_children(0), depth(depth), half_x(half_x), half_y(half_y), node_children(nullptr) {//depth(depth), half_x(half_x), half_y(half_y), node_children(nullptr) {
//        this->tree_children[0] = nullptr;
//        this->tree_children[1] = nullptr;
//        this->tree_children[2] = nullptr;
//        this->tree_children[3] = nullptr;
//    }
    VQuadTree(int min_x, int min_y, int max_x, int max_y, int depth) : total_children(0), depth(depth), min_x(min_x), min_y(min_y), max_x(max_x), max_y(max_y), node_children(nullptr) {
        VQuadTree::CalculateHalfXY(this);
        this->tree_children[0] = nullptr;
        this->tree_children[1] = nullptr;
        this->tree_children[2] = nullptr;
        this->tree_children[3] = nullptr;
    }
    static void CalculateHalfXY(VQuadTree* branch) {
        branch->half_x = (branch->min_x + branch->max_x)/2;
        branch->half_y = (branch->min_y + branch->max_y)/2;
    }

    static void Print(VQuadTree* tree, int indent);
};

class VoronoiGraph {
    public:
        VoronoiGraph();
        ~VoronoiGraph();

        void RespecTree(int x, int y, int w, int h, int max_depth, int critical_mass);
        void PrintTree();
        void RenderTree(SDL_Renderer* render_target);

        void AddNode(VoronoiNode* node);
        void RemoveNode(VoronoiNode* node);
        void UpdateNodePositions();
        NodeLinkedList* GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed);
        NodeLinkedList* GetNearby(double x, double y, VoronoiNode* seed); // closest node on top, the mag of everything else is at most the nearest dist plus the band width

        static RGBColor ForwardPassFromNearby(NodeLinkedList* nearby, double x, double y);
        void DoBackwardsPassFromNearby(NodeLinkedList* nearby, double x, double y, RGBColor image_sample);
        void UpdateAllGradients(double learning_rate);
        void SetGain(double gain);
        void SetBandWidth(double band_width);
        double GetGain();

        void SetErrorLogger(ErrorLogger* error_logger);

//        double GetBandWidth();
//        NodeLinkedList* OrphanChildList(); // only for internal use
    private:
        void AddToChildren(VoronoiNode* node, VQuadTree* branch);
        void AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch);

//        void RemoveFromChildren(VoronoiNode* node, VQuadTree* branch);
//        void ConsolidateChildLists(VQuadTree* branch);
        void RemoveFromBranch(VoronoiNode* node, VQuadTree* branch);
        void ConsolidateChildLists(VQuadTree* branch);

        void BuildNearbyList(VQuadTree* branch);

        void DeleteTree(VQuadTree* branch);

        bool SplitValid(VQuadTree* branch);

        void RenderVQuadTree(VQuadTree* branch, SDL_Renderer* target_renderer);


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
        double gain_gradient;

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

        ErrorLogger* error_logger;
};

#endif // VORONOIGRAPH_H
