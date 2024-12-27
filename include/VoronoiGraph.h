#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
#include "VoronoiQuadTree.h"
#include "VoronoiNode.h"
#include <vector>
#include <SDL.h>

class VoronoiGraph {
    public:
        VoronoiGraph();
        ~VoronoiGraph();

        void Reshape(int x, int y, int w, int h, int max_depth, int critical_mass);
        void AddNode(VoronoiNode* node);
        void RemoveNode(VoronoiNode* node);

        void PrintTree();
        void RenderTree(SDL_Renderer* render_target);

        RGBColor Sample(double x, double y, VoronoiNode** io_seed);
        void Poke(double x, double y, RGBColor image_sample, VoronoiNode** io_seed);

        void UpdateAllGradients(double learning_rate);
        void SetGain(double gain);
        void SetBandWidth(double band_width);
        double GetGain();

        std::vector<VoronoiNode*> GetRecentNearby();

    private:

        VoronoiQuadTree quad_tree;

        std::vector<VoronoiNode*> recent_nearby;

        double band_width;

        double gain;
        double gain_gradient;

};

#endif // VORONOIGRAPH_H
