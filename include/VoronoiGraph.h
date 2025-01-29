#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
#include "VoronoiQuadTree.h"
#include "VoronoiNode.h"
#include <vector>
#include <SDL.h>
#include "NNLayer.h"

class VoronoiGraph : virtual public NNLayer::NNLayer {
    public:
        VoronoiGraph(size_t output_size); // you cannot change the output size
        ~VoronoiGraph();

        void Reshape(int x, int y, int w, int h, int max_depth, int critical_mass);
        void AddNode(VoronoiNode* node);
        void RemoveNode(VoronoiNode* node);

        void SetGain(double gain);
        void SetBandWidth(double band_width);
        double GetGain();

        void PrintTree();
        void RenderTree(SDL_Renderer* render_target);

        void UpdateAllGradients(double learning_rate);

        void SetNearbySeed(VoronoiNode* seed);
        VoronoiNode* GetRecentNearest();
        std::vector<VoronoiNode*> GetRecentNearby();

        RGBColor Sample(double x, double y, VoronoiNode** io_seed);
        void Poke(double x, double y, RGBColor image_sample, VoronoiNode** io_seed);
        void CalculateParameterSize() override;
        void Init(double** write_weights_start) override;
        void Forward(double** io_values_start, double** read_weight_start) override;
        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;


    private:

        VoronoiQuadTree quad_tree;

        VoronoiNode* seed;
        std::vector<VoronoiNode*> recent_nearby;

        double band_width;

        double gain;
        double gain_gradient;

};

#endif // VORONOIGRAPH_H
