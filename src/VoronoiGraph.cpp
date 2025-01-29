#include "VoronoiGraph.h"
#include "CApp.h"
#include "IdkFunctions.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

VoronoiGraph::VoronoiGraph(size_t output_size) : NNLayer(output_size) {
    this->band_width = 0;
    this->gain = 0;
    this->quad_tree.SetGain(this->gain);
    this->quad_tree.SetBandWidth(this->band_width);
    this->gain_gradient = 0;

    this->input_size = 2;
    this->output_size = 3;
    this->parameter_size = 0;
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

void VoronoiGraph::SetNearbySeed(VoronoiNode* seed) {
    this->seed = seed;
}

VoronoiNode* VoronoiGraph::GetRecentNearest() {
    return this->recent_nearby.front();
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

RGBColor VoronoiGraph::Sample(double x, double y, VoronoiNode** io_seed) { // nearby nodes already have their distances calculated, aka gain and bandwidth are baked in
    this->SetNearbySeed((*io_seed));
    double io[5];
    io[0] = x;
    io[1] = y;
    double* active_vector = io;
    double* active_weights = io;
    this->Forward(&active_vector, &active_weights);
    (*io_seed) = this->GetRecentNearest();
    return RGBColor(active_vector[0], active_vector[1], active_vector[2]);
}

void VoronoiGraph::Poke(double x, double y, RGBColor image_sample, VoronoiNode** io_seed) {

    RGBColor final_color = this->Sample(x, y, io_seed);

    double forward_values[5];
    forward_values[0] = x;
    forward_values[1] = y;
    forward_values[2] = final_color.r;
    forward_values[3] = final_color.g;
    forward_values[4] = final_color.b;
    double backward_values[5];
    backward_values[0] = 0;
    backward_values[1] = 0;
    backward_values[2] = 2*(final_color.r-image_sample.r);
    backward_values[3] = 2*(final_color.g-image_sample.g);
    backward_values[4] = 2*(final_color.b-image_sample.b);

    double* active_vector = forward_values+5;
    double* active_back_vector = backward_values+5;
    double* active_weights = forward_values+0;
    double* active_gradients = forward_values+0;
    this->Backward(&active_vector, &active_back_vector, &active_weights, &active_gradients);
}

void VoronoiGraph::CalculateParameterSize() {
    this->parameter_size = 0;
}

void VoronoiGraph::Init(double** write_weights_start) {
    (*write_weights_start) = (*write_weights_start)+this->parameter_size;
}

void VoronoiGraph::Forward(double** io_values_start, double** read_weight_start) { // io_values_start = &[sample_x, sample_y, final_r, final_g, final_b]
    double* current_layer = (*io_values_start);
    double* next_layer = current_layer+this->input_size;

    this->recent_nearby.clear();
    this->recent_nearby = this->quad_tree.GetNearby(current_layer[0], current_layer[1], this->seed);
    double z = 0;
    double exp_offset = -this->GetRecentNearest()->GetDist(); // for numerical precision. softmax doesnt care about offsets so long as theyre applied to all applicants
    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        current_node->CalculateExp(exp_offset);
        z += current_node->GetExp();
    });
    next_layer[0] = 0;
    next_layer[1] = 0;
    next_layer[2] = 0;
    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        double m = current_node->GetExp()/z;
        current_node->SetM(m);
        RGBColor c = current_node->ForwardPass(current_layer[0], current_layer[1]);//node->SampleColor(x, y); // heres the only place x and y are used
        next_layer[0] += c.r*c.r*m;
        next_layer[1] += c.g*c.g*m;
        next_layer[2] += c.b*c.b*m;

    });
    next_layer[0] = std::sqrt(next_layer[0]);
    next_layer[1] = std::sqrt(next_layer[1]);
    next_layer[2] = std::sqrt(next_layer[2]);
    G_Clamp<double>(&next_layer[0], 0, 255);
    G_Clamp<double>(&next_layer[1], 0, 255);
    G_Clamp<double>(&next_layer[2], 0, 255);

    (*io_values_start) = next_layer;
    (*read_weight_start) = (*read_weight_start)+this->parameter_size; // does not move
}

void VoronoiGraph::Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) { // [x,y,r,g,b] [0,0,dldr,dldg,dldb] [N/A] [N/A]
    double* current_output = (*read_values_tail)-this->output_size; // read only [r,g,b]
    double* preceding_input = current_output-this->input_size; // read only [x,y]
    double* current_value_gradient = (*io_back_values_tail)-this->output_size; // read only [dldr,dldg,dldb]
    double* preceding_value_gradient = current_value_gradient-this->input_size; // write only [dldx, dldy]
    double* weights = (*read_weights_tail)-this->parameter_size; // read only
    double* weights_gradient = (*write_gradient_tail)-this->parameter_size; // appended

    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        double gain_grad = 0;
        current_node->BackwardPass(preceding_input[0],
                                   preceding_input[1],
                                   this->gain,
                                   &gain_grad,
                                   RGBColor(current_output[0],current_output[1],current_output[2]),
                                   RGBColor(current_value_gradient[0],current_value_gradient[1],current_value_gradient[2]));
    });

    (*read_weights_tail) = weights; // does not move
    (*write_gradient_tail) = weights_gradient; // does not move
    (*read_values_tail) = preceding_input+this->input_size;
    (*io_back_values_tail) = preceding_value_gradient+this->input_size;
}
