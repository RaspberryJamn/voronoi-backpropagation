#include "VoronoiGraph.h"
#include "CApp.h"
#include "IdkFunctions.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

VoronoiGraph::VoronoiGraph() {

    this->band_width = 0;
    this->gain = 0;
    this->quad_tree.SetGain(this->gain);
    this->quad_tree.SetBandWidth(this->band_width);
    this->gain_gradient = 0;
}

VoronoiGraph::~VoronoiGraph() {

    this->band_width = 0;
    this->gain = 0;

}

void VoronoiGraph::Reshape(int x, int y, int w, int h, int max_depth, int critical_mass) {
    this->quad_tree.RespecTree(x, y, w, h, max_depth, critical_mass);
    this->quad_tree.SetGain(this->gain);
    this->quad_tree.SetBandWidth(this->band_width);
}
void VoronoiGraph::AddNode(VoronoiNode* node) {
    this->quad_tree.AddNode(node);
}
void VoronoiGraph::RemoveNode(VoronoiNode* node) {
    this->quad_tree.RemoveNode(node);
}

RGBColor VoronoiGraph::Sample(double x, double y, VoronoiNode** io_seed) { // nearby nodes already have their distances calculated, aka gain and bandwidth are baked in

    this->recent_nearby.clear();
    if (io_seed == nullptr) {
        this->recent_nearby = this->quad_tree.GetNearby(x, y, nullptr);
    } else {
        this->recent_nearby = this->quad_tree.GetNearby(x, y, *io_seed);
        (*io_seed) = this->recent_nearby.front();
    }
    double z = 0;
    double exp_offset = -this->recent_nearby.front()->GetDist(); // for numerical precision. softmax doesnt care about offsets so long as theyre applied to all applicants

    RGBColor final_color = RGBColor(0,0,0);

    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        current_node->CalculateExp(exp_offset); // work done here
        z += current_node->GetExp();
    });
    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        double m = current_node->GetExp()/z;
        current_node->SetM(m);
        RGBColor c = current_node->ForwardPass(x, y);//node->SampleColor(x, y); // heres the only place x and y are used
        final_color += c*c*m;

    });

    final_color = RGBColor(std::sqrt(final_color.r),
                           std::sqrt(final_color.g),
                           std::sqrt(final_color.b));
    final_color.Clamp();
    return final_color;
}
void VoronoiGraph::Poke(double x, double y, RGBColor image_sample, VoronoiNode** io_seed) {

    RGBColor final_color = this->Sample(x, y, io_seed);

    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        double gain_grad = 0;
        current_node->BackwardPass(x, y, this->gain, &gain_grad, final_color, (final_color-image_sample)*2);
    });
}

void VoronoiGraph::UpdateAllGradients(double learning_rate) {
    this->gain_gradient = 0;
    std::vector<VoronoiNode*> nodes = this->quad_tree.GetAllNodes();
    std::for_each(nodes.begin(), nodes.end(), [&](VoronoiNode* current_node) {
        current_node->ApplyGradients(learning_rate);
        this->gain_gradient += current_node->GetGainGradient();
    });
//    this->gain -= this->gain_gradient*learning_rate*0.0000000005;
//    G_Clamp<double>(&this->gain, 0.0005, 1.0);
//    this->SetGain(this->gain);
    this->quad_tree.UpdateNodePositions();
}

void VoronoiGraph::SetGain(double gain) {
    this->gain = gain;
    this->quad_tree.SetGain(this->gain);
}
void VoronoiGraph::SetBandWidth(double band_width) {
    this->band_width = band_width;
    this->quad_tree.SetBandWidth(this->band_width);
}
double VoronoiGraph::GetGain() {
    return this->gain;
}

std::vector<VoronoiNode*> VoronoiGraph::GetRecentNearby() {
    return this->recent_nearby;
}

void VoronoiGraph::PrintTree() {
    this->quad_tree.PrintTree();
}
void VoronoiGraph::RenderTree(SDL_Renderer* render_target) {
    this->quad_tree.RenderTree(render_target);
}
