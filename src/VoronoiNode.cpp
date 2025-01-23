#include "VoronoiNode.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include "NeuralNetwork/Input.h"
#include "NeuralNetwork/Sigmoid.h"
#include "NeuralNetwork/Dense.h"

VoronoiNode::VoronoiNode(double x, double y) {
    this->Init(x, y, 128+120*std::cos(x/100), 128+120*std::cos(x/70+10), 128+120*std::cos((x+y)/180));
}
VoronoiNode::VoronoiNode(double x, double y, double r, double g, double b) {
    this->Init(x, y, r, g, b);
}
void VoronoiNode::Init(double x, double y, double r, double g, double b) {
    this->x = x;
    this->y = y;
    this->color = RGBColor(r,g,b);
    this->x_grad = 0;
    this->y_grad = 0;
    this->color_grad = RGBColor(0,0,0);
    this->last_x_grad = 0;
    this->last_y_grad = 0;
    this->last_color_grad = RGBColor(0,0,0);
    this->poke_count = 0;
    this->last_poke_count = 0;

    this->network.AddLayer(new NNLayerNSP::Input(2));
    this->network.AddLayer(new NNLayerNSP::Dense(0));
    this->network.AddLayer(new NNLayerNSP::Dense(3));

    this->gain = 0;
    this->gain_grad = 0;
    this->recent_gain_grad = 0;

    this->mag = 0;
    this->exp = 0;
    this->m_value = 0;
    this->sorting_dist = 0;
    this->sorting_x = (int)x;
    this->sorting_y = (int)y;
    this->sorting_x_min = 0;
    this->sorting_y_min = 0;
    this->sorting_x_max = 0;
    this->sorting_y_max = 0;
}

VoronoiNode::~VoronoiNode() {
    this->x = 0;
    this->y = 0;
    this->sorting_x = 0;
    this->sorting_y = 0;
}

void VoronoiNode::SetPosition(double x, double y) {
    this->x = x;
    this->y = y;
}
void VoronoiNode::SetBounds(int min_x, int min_y, int max_x, int max_y) {
    this->sorting_x_min = min_x;
    this->sorting_y_min = min_y;
    this->sorting_x_max = max_x;
    this->sorting_y_max = max_y;
}
bool VoronoiNode::IsBounded(int x, int y) {
    if ((x < this->sorting_x_min) ||
        (x >=this->sorting_x_max) ||
        (y < this->sorting_y_min) ||
        (y >=this->sorting_y_max)) {
        return false;
    }
    return true;
}

void VoronoiNode::Clamp(int min_x, int min_y, int max_x, int max_y) {
    if (this->x <  min_x) { this->x = min_x;}
    if (this->x >= max_x) { this->x = max_x-1;}
    if (this->y <  min_y) { this->y = min_y;}
    if (this->y >= max_y) { this->y = max_y-1;}
}

double VoronoiNode::GetX() {
    return this->x;
}
double VoronoiNode::GetY() {
    return this->y;
}

void PrintIndents(int indent);
void VoronoiNode::Print(int indent) {
    RGBColor c = this->color;
    std::cout << this << ": {" << std::endl;
    PrintIndents(indent+1); std::cout << "sort_(x,y): (" << this->sorting_x << "," << this->sorting_y << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "precise_(x,y): (" << this->x << "," << this->y << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "min_corner_(x,y): (" << this->sorting_x_min << "," << this->sorting_y_min << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "max_corner_(x,y): (" << this->sorting_x_max << "," << this->sorting_y_max << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "color: [" << c.r << "," << c.g << "," << c.b << "]," << std::endl;
    PrintIndents(indent+1); std::cout << "dist:" << this->mag<< "," << std::endl;
    PrintIndents(indent+1); std::cout << "residency:" << this->residential_slot << "," << std::endl;
    PrintIndents(indent+1); std::cout << "reference:" << this->tree_slot << std::endl;
    PrintIndents(indent); std::cout << "}";
}

void VoronoiNode::Render(SDL_Renderer* target_renderer) {
    SDL_Rect dest = {0,0,0,0};

    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
//    dest = {this->sorting_x-3, this->sorting_y-3, 7, 7}; SDL_RenderDrawRect(target_renderer, &dest);
    dest = {this->sorting_x-2, this->sorting_y-3, 5, 7}; SDL_RenderDrawRect(target_renderer, &dest);
    dest = {this->sorting_x-3, this->sorting_y-2, 7, 5}; SDL_RenderDrawRect(target_renderer, &dest);

    SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    dest = {this->sorting_x-2, this->sorting_y-2, 5, 5}; SDL_RenderDrawRect(target_renderer, &dest);

    SDL_SetRenderDrawColor(target_renderer, (Uint8)this->color.r, (Uint8)this->color.g, (Uint8)this->color.b, 0xFF);
    dest = {this->sorting_x-1, this->sorting_y-1, 3, 3}; SDL_RenderDrawRect(target_renderer, &dest);
//    SDL_RenderDrawPoint(target_renderer, this->sorting_x, this->sorting_y);
    SDL_RenderDrawLine(target_renderer, this->sorting_x, this->sorting_y-2, this->sorting_x, sorting_y+2);
    SDL_RenderDrawLine(target_renderer, this->sorting_x-2, this->sorting_y, this->sorting_x+2, sorting_y);

    this->RenderLoggedGradient(target_renderer);
}
void VoronoiNode::RenderLoggedGradient(SDL_Renderer* target_renderer) {
    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
    double delta_x = this->last_x_grad;
    double delta_y = this->last_y_grad;
    double mag = 1;//std::sqrt(delta_x*delta_x+delta_y*delta_y);
    double scale = -1.0/(this->last_poke_count+1);
    if (mag == 0) {
        mag = 1;
    }
    delta_x *= scale/mag;
    delta_y *= scale/mag;
    double draw_length = 1;
    delta_x *= draw_length;
    delta_y *= draw_length;
    SDL_RenderDrawLine(target_renderer, this->sorting_x, this->sorting_y, this->sorting_x+(int)delta_x, sorting_y+(int)delta_y);
    SDL_Rect dest = {this->sorting_x-(int)delta_x-1, sorting_y-(int)delta_y-1, 3, 3}; SDL_RenderDrawRect(target_renderer, &dest);
//    SDL_RenderDrawPoint(target_renderer, this->sorting_x-(int)delta_x, sorting_y-(int)delta_y);
//    std::cout << "[" << this->log_d_loss_d_finalcolor.r*scale << "," << this->log_d_loss_d_finalcolor.g*scale << "," << this->log_d_loss_d_finalcolor.b*scale << "]" << std::endl;
}

RGBColor VoronoiNode::ForwardPass(double sample_x, double sample_y) {
    // eg Generate(sample_x-this->x, sample_x-this->y)
    return this->color;
}
// this->mag = ((this->x-sample_x)^2+(this->y-sample_y)^2)*gain
//     this->exp = exp(-this->mag) // oops, correction
//     g_z = this->exp+that->exp+there->exp
//     this->m = this->exp/g_z
// this->color = this->Generate(sample_x-this->x, sample_y-this->y)
// g_color = (this->m*this->color^2+that->m*that->color^2+there->m*there->color^2)^0.5

// loss = (g_color-target_color)^2
// d_loss_d_g_color = 2*(g_color-target_color)

// d_g_color_d_this->m = 0.5/g_color*(this->color^2)
// d_g_color_d_this->color = 1.0/g_color*this->m*this->color
// d_this->color_d_this->weights = 0//d_this->Generate(sample_x-this->x, sample_y-this->y)_d_weights
// d_this->color_d_this->x = 0//d_this->Generate(sample_x-this->x, sample_y-this->y)_d_this->x

// d_this->m_d_this->mag = this->m*(this->m-1) // oops, correction
// d_this->mag_d_this->x = 2*(this->x-sample_x)*gain

void VoronoiNode::BackwardPass(double sample_x, double sample_y, RGBColor finalcolor, RGBColor d_loss_d_finalcolor) {
//    std::cout << std::floor(100.0*d_loss_d_finalcolor.r)/100.0 << " ";
    RGBColor d_finalcolor_d_m = ((this->color*this->color)/finalcolor)*.5;
    RGBColor d_finalcolor_d_thiscolor = (this->color/finalcolor)*this->m_value;
    double d_m_d_mag = this->m_value*(this->m_value-1.0);
    double d_mag_d_x = 2.0*(this->x-sample_x)*this->gain;
    double d_mag_d_y = 2.0*(this->y-sample_y)*this->gain;

    double d_loss_d_m = RGBColor::Trace(d_loss_d_finalcolor*d_finalcolor_d_m);
    double d_loss_d_mag = d_loss_d_m * d_m_d_mag;

//    this->log_d_loss_d_finalcolor += d_loss_d_finalcolor;
    this->x_grad += d_loss_d_mag * d_mag_d_x;
    this->y_grad += d_loss_d_mag * d_mag_d_y;
    this->color_grad += d_loss_d_finalcolor*d_finalcolor_d_thiscolor;
    this->gain_grad += d_loss_d_mag*(this->mag/this->gain);
    this->poke_count++;
}

void VoronoiNode::ApplyGradients(double learning_rate) {
    this->LogGradients();

    std::srand(std::time(0)+this->color.r*this->x);

    this->x -= (this->x_grad + (std::rand()%100+std::rand()%100-100)*500)*learning_rate;
    this->y -= (this->y_grad + (std::rand()%100+std::rand()%100-100)*500)*learning_rate;
    this->color -= this->color_grad*learning_rate*8;
    this->color.Clamp();
    this->ClearGradients();
}
void VoronoiNode::LogGradients() {
    this->last_x_grad = this->x_grad;
    this->last_y_grad = this->y_grad;
    this->last_color_grad = this->color_grad;
    this->last_poke_count = this->poke_count;
    this->recent_gain_grad = this->gain_grad;
}
void VoronoiNode::ClearGradients() {
//    this->log_d_loss_d_finalcolor = RGBColor(0,0,0);
    this->x_grad = 0;
    this->y_grad = 0;
    this->color_grad.r = 0;
    this->color_grad.g = 0;
    this->color_grad.b = 0;
    this->gain_grad = 0;
    this->poke_count = 0;
}

void VoronoiNode::CalculateExp(double offset) {
    this->exp = std::exp(-this->mag - offset);
}
double VoronoiNode::GetExp() {
    return this->exp;
}
void VoronoiNode::SetM(double m) {
    this->m_value = m;
}
double VoronoiNode::GetM() {
    return this->m_value;
}
double VoronoiNode::GetGainGradient() {
    return this->recent_gain_grad;
}

void VoronoiNode::GetLastGradients(double* x_grad, double* y_grad, RGBColor* color_grad) {
    (*x_grad) = this->last_x_grad;
    (*y_grad) = this->last_y_grad;
    (*color_grad) = this->last_color_grad;
}

void VoronoiNode::CalculateDist(double from_x, double from_y, double gain) {
    this->gain = gain;
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->mag = (dx*dx+dy*dy)*this->gain;
}

double VoronoiNode::GetDist() {
    return this->mag;
}

void VoronoiNode::CalculateSortingDist(double from_x, double from_y) {
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->sorting_dist = (int)(std::ceil(std::sqrt(dx*dx+dy*dy)));
}

int VoronoiNode::GetSortingDist() {
    return this->sorting_dist;
}

void VoronoiNode::CalculateSortingPos() {
    this->sorting_x = (int)this->x;
    this->sorting_y = (int)this->y;
}

int VoronoiNode::GetSortingPosX() {
    return this->sorting_x;
}

int VoronoiNode::GetSortingPosY() {
    return this->sorting_y;
}
