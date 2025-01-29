#include "VoronoiNode.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include "NNLayer/Input.h"
#include "NNLayer/Sigmoid.h"
#include "NNLayer/Dense.h"

VoronoiNode::VoronoiNode(double x, double y) {
    this->Init(x, y, 128+120*std::cos(x/100), 128+120*std::cos(x/70+10), 128+120*std::cos((x+y)/180));
}
VoronoiNode::VoronoiNode(double x, double y, double r, double g, double b) {
    this->Init(x, y, r, g, b);
}
void VoronoiNode::Init(double x, double y, double r, double g, double b) {
    this->x = x;
    this->y = y;
//    this->color = RGBColor(r,g,b);
    this->model.x_grad = 0;
    this->model.y_grad = 0;
//    this->color_grad = RGBColor(0,0,0);
//    this->last_x_grad = 0;
//    this->last_y_grad = 0;
//    this->last_color_grad = RGBColor(0,0,0);
//    this->poke_count = 0;
//    this->last_poke_count = 0;

    this->model.network.AddLayer(new NNLayer::Input(2));
    this->model.network.AddLayer(new NNLayer::Dense(0));
    this->model.network.AddLayer(new NNLayer::Dense(3));

//    this->gain = 0;
//    this->gain_grad = 0;
//    this->recent_gain_grad = 0;

    this->model.mag = 0;
    this->model.exp = 0;
    this->model.m = 0;
    this->sort.dist = 0;
    this->sort.x = (int)x;
    this->sort.y = (int)y;
    this->sort.x_min = 0;
    this->sort.y_min = 0;
    this->sort.x_max = 0;
    this->sort.y_max = 0;
}

VoronoiNode::~VoronoiNode() {
    this->x = 0;
    this->y = 0;
    this->sort = {0};
}

void VoronoiNode::SetPosition(double x, double y) {
    this->x = x;
    this->y = y;
}
void VoronoiNode::SetBounds(int min_x, int min_y, int max_x, int max_y) {
    this->sort.x_min = min_x;
    this->sort.y_min = min_y;
    this->sort.x_max = max_x;
    this->sort.y_max = max_y;
}
bool VoronoiNode::IsBounded(int x, int y) {
    if ((x < this->sort.x_min) ||
        (x >=this->sort.x_max) ||
        (y < this->sort.y_min) ||
        (y >=this->sort.y_max)) {
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
//    RGBColor c = this->color;
    std::cout << this << ": {" << std::endl;
    PrintIndents(indent+1); std::cout << "sort_(x,y): (" << this->sort.x << "," << this->sort.y << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "precise_(x,y): (" << this->x << "," << this->y << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "min_corner_(x,y): (" << this->sort.x_min << "," << this->sort.y_min << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "max_corner_(x,y): (" << this->sort.x_max << "," << this->sort.y_max << ")," << std::endl;
//    PrintIndents(indent+1); std::cout << "color: [" << c.r << "," << c.g << "," << c.b << "]," << std::endl;
    PrintIndents(indent+1); std::cout << "dist:" << this->model.mag<< "," << std::endl;
//    PrintIndents(indent+1); std::cout << "residency:" << this->residential_slot << "," << std::endl;
//    PrintIndents(indent+1); std::cout << "reference:" << this->tree_slot << std::endl;
    PrintIndents(indent); std::cout << "}";
}

void VoronoiNode::Render(SDL_Renderer* target_renderer) {
    SDL_Rect dest = {0,0,0,0};

    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
//    dest = {this->sorting_x-3, this->sorting_y-3, 7, 7}; SDL_RenderDrawRect(target_renderer, &dest);
    dest = {this->sort.x-2, this->sort.y-3, 5, 7}; SDL_RenderDrawRect(target_renderer, &dest);
    dest = {this->sort.x-3, this->sort.y-2, 7, 5}; SDL_RenderDrawRect(target_renderer, &dest);

    SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    dest = {this->sort.x-2, this->sort.y-2, 5, 5}; SDL_RenderDrawRect(target_renderer, &dest);

    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);//(Uint8)this->color.r, (Uint8)this->color.g, (Uint8)this->color.b, 0xFF);
    dest = {this->sort.x-1, this->sort.y-1, 3, 3}; SDL_RenderDrawRect(target_renderer, &dest);
//    SDL_RenderDrawPoint(target_renderer, this->sorting_x, this->sorting_y);
    SDL_RenderDrawLine(target_renderer, this->sort.x, this->sort.y-2, this->sort.x, sort.y+2);
    SDL_RenderDrawLine(target_renderer, this->sort.x-2, this->sort.y, this->sort.x+2, sort.y);

//    this->RenderLoggedGradient(target_renderer);
}
//void VoronoiNode::RenderLoggedGradient(SDL_Renderer* target_renderer) {
//    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
//    double delta_x = 0;//this->last_x_grad;
//    double delta_y = 0;//this->last_y_grad;
//    double mag = 1;//std::sqrt(delta_x*delta_x+delta_y*delta_y);
//    double scale = -1.0;// /(this->last_poke_count+1);
//    if (mag == 0) {
//        mag = 1;
//    }
//    delta_x *= scale/mag;
//    delta_y *= scale/mag;
//    double draw_length = 1;
//    delta_x *= draw_length;
//    delta_y *= draw_length;
//    SDL_RenderDrawLine(target_renderer, this->sort.x, this->sort.y, this->sort.x+(int)delta_x, this->sort.y+(int)delta_y);
//    SDL_Rect dest = {this->sort.x-(int)delta_x-1, sort.y-(int)delta_y-1, 3, 3}; SDL_RenderDrawRect(target_renderer, &dest);
////    SDL_RenderDrawPoint(target_renderer, this->sorting_x-(int)delta_x, sorting_y-(int)delta_y);
////    std::cout << "[" << this->log_d_loss_d_finalcolor.r*scale << "," << this->log_d_loss_d_finalcolor.g*scale << "," << this->log_d_loss_d_finalcolor.b*scale << "]" << std::endl;
//}

RGBColor VoronoiNode::ForwardPass(double sample_x, double sample_y) {
    // eg Generate(sample_x-this->x, sample_x-this->y)
//    return this->color;
    double io[3];
    io[0] = sample_x-this->x;
    io[1] = sample_y-this->y;
    this->model.network.Input(io, 2);

    this->model.network.Forward();

    this->model.network.Output(io, 3);
    return RGBColor(io[0], io[1], io[2]);
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

void VoronoiNode::BackwardPass(double sample_x, double sample_y, double gain, double* gain_grad, RGBColor finalcolor, RGBColor d_loss_d_finalcolor) {
//    std::cout << std::floor(100.0*d_loss_d_finalcolor.r)/100.0 << " ";
    double thiscolorarr[3];
    this->model.network.Output(thiscolorarr, 3);
    RGBColor thiscolor = RGBColor(thiscolorarr[0], thiscolorarr[1], thiscolorarr[2]);
    RGBColor d_finalcolor_d_m = ((thiscolor*thiscolor)/finalcolor)*.5;
    RGBColor d_finalcolor_d_thiscolor = (thiscolor/finalcolor)*this->model.m;
    double d_m_d_mag = this->model.m*(this->model.m-1.0);
    double d_mag_d_x = 2.0*(this->x-sample_x)*gain;
    double d_mag_d_y = 2.0*(this->y-sample_y)*gain;

    double d_loss_d_m = RGBColor::Trace(d_loss_d_finalcolor*d_finalcolor_d_m);
    double d_loss_d_mag = d_loss_d_m * d_m_d_mag;

    RGBColor d_loss_d_thiscolor = d_loss_d_finalcolor*d_finalcolor_d_thiscolor;
    double d_loss_d_thiscolor_arr[3];
    d_loss_d_thiscolor_arr[0] = d_loss_d_thiscolor.r;
    d_loss_d_thiscolor_arr[1] = d_loss_d_thiscolor.g;
    d_loss_d_thiscolor_arr[2] = d_loss_d_thiscolor.b;
    this->model.network.SetOutputGradient(d_loss_d_thiscolor_arr, 3);

    this->model.network.Backward();

    double d_loss_d_colorxy[2]; this->model.network.GetInputGradient(d_loss_d_colorxy, 2);

//    this->log_d_loss_d_finalcolor += d_loss_d_finalcolor;
    this->model.x_grad += d_loss_d_mag * d_mag_d_x + d_loss_d_colorxy[0];
    this->model.y_grad += d_loss_d_mag * d_mag_d_y + d_loss_d_colorxy[1];
//    this->color_grad += d_loss_d_thiscolor;
    (*gain_grad) += d_loss_d_mag*(this->model.mag/gain);
//    this->poke_count++;
}

void VoronoiNode::ApplyGradients(double learning_rate) {
//    this->LogGradients();

    std::srand(std::time(0)+this->y*this->x);

    this->x -= (this->model.x_grad + (std::rand()%100+std::rand()%100-100)*500)*learning_rate;
    this->y -= (this->model.y_grad + (std::rand()%100+std::rand()%100-100)*500)*learning_rate;
//    this->color -= this->color_grad*learning_rate*8;
//    this->color.Clamp();
    this->model.network.ApplyGradients(learning_rate);

    this->ClearGradients();
}
//void VoronoiNode::LogGradients() {
//    this->last_x_grad = this->x_grad;
//    this->last_y_grad = this->y_grad;
////    this->last_color_grad = this->color_grad;
////    this->last_poke_count = this->poke_count;
////    this->recent_gain_grad = this->gain_grad;
//}
void VoronoiNode::ClearGradients() {
//    this->log_d_loss_d_finalcolor = RGBColor(0,0,0);
    this->model.x_grad = 0;
    this->model.y_grad = 0;
//    this->color_grad.r = 0;
//    this->color_grad.g = 0;
//    this->color_grad.b = 0;
//    this->gain_grad = 0;
//    this->poke_count = 0;
}

void VoronoiNode::CalculateExp(double offset) {
    this->model.exp = std::exp(-this->model.mag - offset);
}
double VoronoiNode::GetExp() {
    return this->model.exp;
}
void VoronoiNode::SetM(double m) {
    this->model.m = m;
}
double VoronoiNode::GetM() {
    return this->model.m;
}
double VoronoiNode::GetGainGradient() {
    return 0;//this->recent_gain_grad;
}

//void VoronoiNode::GetLastGradients(double* x_grad, double* y_grad, RGBColor* color_grad) {
//    (*x_grad) = this->last_x_grad;
//    (*y_grad) = this->last_y_grad;
////    (*color_grad) = this->last_color_grad;
//}

void VoronoiNode::CalculateDist(double from_x, double from_y, double gain) {
//    this->model.gain = gain;
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->model.mag = (dx*dx+dy*dy)*gain;
}

double VoronoiNode::GetDist() {
    return this->model.mag;
}

void VoronoiNode::CalculateSortingDist(double from_x, double from_y) {
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->sort.dist = (int)(std::ceil(std::sqrt(dx*dx+dy*dy)));
}

int VoronoiNode::GetSortingDist() {
    return this->sort.dist;
}

void VoronoiNode::CalculateSortingPos() {
    this->sort.x = (int)this->x;
    this->sort.y = (int)this->y;
}

int VoronoiNode::GetSortingPosX() {
    return this->sort.x;
}

int VoronoiNode::GetSortingPosY() {
    return this->sort.y;
}
