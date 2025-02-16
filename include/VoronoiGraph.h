#ifndef VORONOIGRAPH_H
#define VORONOIGRAPH_H
#include <vector>
#include "RGBColor.h"
#include "VoronoiQuadTree.h"
#include "VoronoiNode.h"
#include "NNLayer.h"
#include "NNLayer/Input.h"
#include "NNLayer/Sigmoid.h"
#include "NNLayer/Dense.h"

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
        void RenderTree(SDL_Renderer* render_target, AtlasNumberDrawer* number_renderer);

        void UpdateAllGradients();

        double GetActiveCummLoss(); // counts on backward passes specifically
        double GetRecentCummLoss();
        void CalculateVarianceOfCummulativeLoss();
        double GetRecentCummLossMean();
        double GetRecentCummLossVariance();
        double GetLearningRate();
        void SetLearningRate(double learning_rate);
        void SetXYRate(double xy_rate);

        std::vector<VoronoiNode*> GetRecentNearby();

        RGBColor Sample(double x, double y);
        void Poke(double x, double y, RGBColor image_sample);
        void CalculateParameterSize() override;
        void Init(double** write_weights_start) override;
        void Forward(double** io_values_start, double** read_weight_start) override;
        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;


    private:

        VoronoiQuadTree quad_tree;

        std::vector<VoronoiNode*> recent_nearby;

        double learning_rate;

        double node_xy_rate;
        double variance_strength;
        double xy_pressure;
        double xy_centrality;

        double band_width;

        double gain;
        double gain_gradient;

        double recent_cumm_loss;
        double active_cumm_loss;
        double recent_loss_mean;
        double recent_loss_variance;

        int backward_count;

};

#endif // VORONOIGRAPH_H
