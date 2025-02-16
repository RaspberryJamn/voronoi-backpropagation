#include "VoronoiGraph.h"
#include "VoronoiNode.h"
#include "IdkFunctions.h"
#include <cmath>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "NNLayer.h"
#include "RGBColor.h"

VoronoiGraph::VoronoiGraph(size_t output_size) : NNLayer::NNLayer(output_size, 0) {
    this->band_width = 0;
    this->gain = 0;
    this->quad_tree.SetGain(this->gain);
    this->quad_tree.SetBandWidth(this->band_width);
    this->gain_gradient = 0;

    this->node_xy_rate = 500.0;
    this->active_cumm_loss = 0;
    this->recent_cumm_loss = 0;
    this->recent_loss_variance = 0;

    this->backward_count = 0;
    this->input_size = 2;
    this->output_size = 3;
    this->parameter_size = 0;
}

VoronoiGraph::~VoronoiGraph() {
    this->band_width = 0;
    this->gain = 0;
    this->quad_tree.SetGain(this->gain);
    this->quad_tree.SetBandWidth(this->band_width);
    this->gain_gradient = 0;

    this->node_xy_rate = 0.0;
    this->active_cumm_loss = 0;
    this->recent_cumm_loss = 0;
    this->recent_loss_variance = 0;

    this->backward_count = 0;
    this->input_size = 2;
    this->output_size = 3;
    this->parameter_size = 0;
}

void VoronoiGraph::Reshape(int x, int y, int w, int h, int max_depth, int critical_mass) {
    this->quad_tree.RespecTree(x, y, w, h, max_depth, critical_mass);
    this->quad_tree.SetGain(this->gain);
    this->quad_tree.SetBandWidth(this->band_width);
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    this->quad_tree.AddNode(node);
//    this->quad_tree.PrintTree();
}
void VoronoiGraph::RemoveNode(VoronoiNode* node) {
    this->quad_tree.RemoveNode(node);
}

void VoronoiGraph::PrintTree() {
    this->quad_tree.PrintTree();
}
void VoronoiGraph::RenderTree(SDL_Renderer* render_target, AtlasNumberDrawer* number_renderer) {
    this->quad_tree.RenderTree(render_target, number_renderer);
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

void VoronoiGraph::UpdateAllGradients(double learning_rate) {
    std::srand(std::time(0));
    this->gain_gradient = 0;
    std::vector<VoronoiNode*> nodes = this->quad_tree.GetAllNodes();
    std::for_each(nodes.begin(), nodes.end(), [&](VoronoiNode* current_node) {
//        std::cout << current_node->model.x_grad << std::endl;
//        std::cout << current_node->model.y_grad << std::endl;
        if (!((current_node->model.x_grad > 0) || (current_node->model.x_grad <= 0))) {
//            SDL_assert(false);
            current_node->model.x_grad = 0;
        }
        if (!((current_node->model.y_grad > 0) || (current_node->model.y_grad <= 0))) {
//            SDL_assert(false);
            current_node->model.y_grad = 0;
        }
        current_node->x -= std::rand()%100-50;//(current_node->model.x_grad + (std::rand()%100+std::rand()%100-100)/300)/this->backward_count*learning_rate*this->node_xy_rate;
        current_node->y -= std::rand()%100-50;//(current_node->model.y_grad + (std::rand()%100+std::rand()%100-100)/300)/this->backward_count*learning_rate*this->node_xy_rate;
        current_node->model.network.ApplyGradients(learning_rate);
//        this->gain_gradient += current_node->GetGainGradient();

        current_node->model.x_grad = 0;
        current_node->model.y_grad = 0;
        current_node->model.network.ClearGradients();

        current_node->model.prev_accum_loss = current_node->model.accum_loss;
        current_node->model.accum_loss = 0;
    });
    this->backward_count = 0;
    this->recent_cumm_loss = this->active_cumm_loss;
    this->active_cumm_loss = 0;
    this->CalculateVarianceOfCummulativeLoss();
//    this->gain -= this->gain_gradient*learning_rate*0.0000000005;
//    G_Clamp<double>(&this->gain, 0.0005, 1.0);
//    this->gain_gradient = 0;
    this->quad_tree.UpdateNodePositions();
}

std::vector<VoronoiNode*> VoronoiGraph::GetRecentNearby() {
    return this->recent_nearby;
}
void VoronoiGraph::CalculateVarianceOfCummulativeLoss() {
    double mean = 0;
    int n = this->quad_tree.GetNodeCount();
    std::vector<VoronoiNode*> all_nodes = this->quad_tree.GetAllNodes();
    std::for_each(all_nodes.begin(), all_nodes.end(), [&](VoronoiNode* current_node) {
        mean += current_node->model.prev_accum_loss;
    });
    mean /= n;
    this->recent_loss_mean = mean;
    double variance = 0;
    std::for_each(all_nodes.begin(), all_nodes.end(), [&](VoronoiNode* current_node) {
        double delta = current_node->model.prev_accum_loss-this->recent_loss_mean;
        variance += delta*delta;
    });
    variance /= n;
    this->recent_loss_variance = variance;
}
double VoronoiGraph::GetRecentCummLossVariance() {
    return this->recent_loss_variance;
}
double VoronoiGraph::GetRecentCummLossMean() {
    return this->recent_loss_mean;
}
double VoronoiGraph::GetActiveCummLoss() {
    return this->active_cumm_loss;
}
double VoronoiGraph::GetRecentCummLoss() {
    return this->recent_cumm_loss;
}

RGBColor VoronoiGraph::Sample(double x, double y) { // nearby nodes already have their distances calculated, aka gain and bandwidth are baked in
    double io[5];
    io[0] = x;
    io[1] = y;
    double* active_vector = io;
    double* active_weights = nullptr;//io; // a formality of the NNLayer framework
    this->Forward(&active_vector, &active_weights); // moves active_vector from [x, y, ...] to [final_r, final_g, final_b]
    return RGBColor(active_vector[0]*256.0, active_vector[1]*256.0, active_vector[2]*256.0);
//    return RGBColor(io[2], io[3], io[4]);
}

void VoronoiGraph::Poke(double x, double y, RGBColor image_sample) {

    double forward_values[5];
    forward_values[0] = x;
    forward_values[1] = y;
    RGBColor final_color = this->Sample(x, y);
    forward_values[2] = final_color.r;
    forward_values[3] = final_color.g;
    forward_values[4] = final_color.b;

    double backward_values[5];
    backward_values[0] = 0; // will be overwritten
    backward_values[1] = 0;
    backward_values[2] = 2*((final_color.r-image_sample.r)/256.0); // d_( (finalcolor-targetcolor)^2 )_d_finalcolor
    backward_values[3] = 2*((final_color.g-image_sample.g)/256.0);
    backward_values[4] = 2*((final_color.b-image_sample.b)/256.0);

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
    double* current_layer = (*io_values_start); // [sample_x, sample_y, ...]
    double* next_layer = current_layer+this->input_size; // [final_r, final_g, final_b] (ranges from 0.0..1.0)

    this->recent_nearby.clear();
    this->recent_nearby = this->quad_tree.GetNearby(current_layer[0], current_layer[1]);

    // establish both the exp per node and the sum of all exps
    double z = 0;
    double exp_offset = -this->recent_nearby.front()->model.mag; // for numerical precision. softmax doesnt care about constant offsets so long as theyre applied to all applicants
    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        current_node->model.exp = std::exp(-(current_node->model.mag + exp_offset));
        z += current_node->model.exp;
    });
    // now that m can be inferred, calculate the content of each node's contribution and combine using each contribution's weight (m)
    next_layer[0] = 0;
    next_layer[1] = 0;
    next_layer[2] = 0;
    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        double m = current_node->model.exp/z;
        current_node->model.m = m;

        double io[3]; // [rel_x, rel_y, ...] -> [r, g, b]
        io[0] = current_layer[0]-current_node->x;
        io[1] = current_layer[1]-current_node->y;
        current_node->model.network.Input(io, 2);
        current_node->model.network.Forward();
        current_node->model.network.Output(io, 3);
        // node colors converted to a more perceptual space (^2) before interpolation,
        next_layer[0] += io[0]*io[0]*m;
        next_layer[1] += io[1]*io[1]*m;
        next_layer[2] += io[2]*io[2]*m;

    });
    // and are converted back (^0.5) when outputted
    next_layer[0] = std::sqrt(next_layer[0]);
    next_layer[1] = std::sqrt(next_layer[1]);
    next_layer[2] = std::sqrt(next_layer[2]);

    (*io_values_start) = next_layer;
    (*read_weight_start) = (*read_weight_start)+this->parameter_size; // does not move
}


// this->mag = ((this->x-sample_x)^2+(this->y-sample_y)^2)*gain
//     this->exp = exp(-this->mag) // oops, correction
//     g_z = this->exp+that->exp+there->exp
//     this->m = this->exp/g_z
// this->color = this->Generate(sample_x-this->x, sample_y-this->y)
// finalcolor = (this->m*this->color^2+that->m*that->color^2+there->m*there->color^2)^0.5

// loss = (finalcolor-target_color)^2
// d_loss_d_g_color = 2*(finalcolor-target_color)

// d_finalcolor_d_this->m = 0.5/finalcolor*(this->color^2)
// d_finalcolor_d_this->color = 1.0/finalcolor*this->m*this->color
// d_this->color_d_this->weights = 0//d_this->Generate(sample_x-this->x, sample_y-this->y)_d_weights
// d_this->color_d_this->x = 0//d_this->Generate(sample_x-this->x, sample_y-this->y)_d_this->x

// d_this->m_d_this->mag = this->m*(this->m-1) // oops, correction
// d_this->mag_d_this->x = 2*(this->x-sample_x)*gain
void VoronoiGraph::Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) { // [x,y,r,g,b] [0,0,dldr,dldg,dldb] [N/A] [N/A]
    double* current_output = (*read_values_tail)-this->output_size; // read only [r,g,b]
    double* preceding_input = current_output-this->input_size; // read only [x,y]
    double* current_value_gradient = (*io_back_values_tail)-this->output_size; // read only [dldr,dldg,dldb]
    double* preceding_value_gradient = current_value_gradient-this->input_size; // write only [dldx, dldy]
    double* weights = (*read_weights_tail)-this->parameter_size; // read only
    double* weights_gradient = (*write_gradient_tail)-this->parameter_size; // appended
    RGBColor finalcolor = RGBColor(current_output[0],current_output[1],current_output[2]);

    int total_child_count = this->quad_tree.GetNodeCount();
    RGBColor d_loss_d_finalcolor = RGBColor(current_value_gradient[0],current_value_gradient[1],current_value_gradient[2]);
    double final_pixel_loss = RGBColor::Trace(d_loss_d_finalcolor*d_loss_d_finalcolor)*0.25; // only works for MSE, be warned
    std::for_each(this->recent_nearby.begin(), this->recent_nearby.end(), [&](VoronoiNode* current_node) {
        // setup partial derivatives

        // gets current_node's generated color
        double currentscolor_arr[3];
        current_node->model.network.Output(currentscolor_arr, 3);
        RGBColor currentscolor = RGBColor(currentscolor_arr[0], currentscolor_arr[1], currentscolor_arr[2]);
        // d_loss_d_mag
//        RGBColor d_finalcolor_d_m = ((currentscolor*currentscolor)/finalcolor)*.5;
//        double d_loss_d_m = RGBColor::Trace(d_loss_d_finalcolor*d_finalcolor_d_m);
        double d_m_d_mag = current_node->model.m*(current_node->model.m-1.0);
//        double d_loss_d_mag = d_loss_d_m * d_m_d_mag;
        // d_loss_d_currentscolor
        RGBColor d_finalcolor_d_currentscolor = (currentscolor/finalcolor)*current_node->model.m;
        RGBColor d_loss_d_currentscolor = d_loss_d_finalcolor*d_finalcolor_d_currentscolor;
        double d_loss_d_currentscolor_arr[3];
        d_loss_d_currentscolor_arr[0] = d_loss_d_currentscolor.r;
        d_loss_d_currentscolor_arr[1] = d_loss_d_currentscolor.g;
        d_loss_d_currentscolor_arr[2] = d_loss_d_currentscolor.b;
        // propagate and store gradients back through the current_node's network
        current_node->model.network.SetOutputGradient(d_loss_d_currentscolor_arr, 3);
        current_node->model.network.Backward();
//        double d_loss_d_currentscolor_d_currentscolor_d_xy[2]; current_node->model.network.GetInputGradient(d_loss_d_currentscolor_d_currentscolor_d_xy, 2);
        //
        double d_varloss_d_accum_loss = 2.0/total_child_count*(current_node->model.prev_accum_loss-this->recent_loss_mean);
        double d_varloss_d_mag = d_varloss_d_accum_loss*final_pixel_loss*d_m_d_mag;
        double d_mag_d_x = 2.0*(current_node->x-preceding_input[0])*this->gain;
        double d_mag_d_y = 2.0*(current_node->y-preceding_input[1])*this->gain;
        // store gradient on position
//        current_node->model.x_grad += (d_loss_d_mag * d_mag_d_x + d_loss_d_currentscolor_d_currentscolor_d_xy[0]);
//        current_node->model.y_grad += (d_loss_d_mag * d_mag_d_y + d_loss_d_currentscolor_d_currentscolor_d_xy[1]);
        current_node->model.x_grad += d_varloss_d_mag*d_mag_d_x;
        current_node->model.y_grad += d_varloss_d_mag*d_mag_d_y;
//        gain_grad += d_loss_d_mag*(current_node->model.mag/gain); // unused

        // quickly take note of loss from this sample, for the purpose of having all nodes have approx equal loss
        current_node->model.accum_loss += final_pixel_loss*current_node->model.m; // color shall be considered constant, m is the vehicle by which varience is decreased
    });
    this->backward_count++;
    this->active_cumm_loss+=final_pixel_loss;

    (*read_weights_tail) = weights; // does not move
    (*write_gradient_tail) = weights_gradient; // does not move
    (*read_values_tail) = preceding_input+this->input_size;
    (*io_back_values_tail) = preceding_value_gradient+this->input_size;
}
