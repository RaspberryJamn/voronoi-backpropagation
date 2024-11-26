#include "VoronoiNode.h"
#include <cmath>
#include <iostream>

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

    this->residential_slot = nullptr;
    this->tree_slot = nullptr;

    this->mag = 0;
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

void VoronoiNode::SetResidence(NodeLinkedList* home) {
    this->residential_slot = home;
}
NodeLinkedList* VoronoiNode::GetResidence() {
    return this->residential_slot;
}

void VoronoiNode::SetTreeSlot(NodeLinkedList* location) {
    this->tree_slot = location;
}
NodeLinkedList* VoronoiNode::GetTreeSlot() {
    return this->tree_slot;
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
bool VoronoiNode::IsBounded() {
    if ((this->sorting_x < this->sorting_x_min) ||
        (this->sorting_x > this->sorting_x_max) ||
        (this->sorting_y < this->sorting_y_min) ||
        (this->sorting_y > this->sorting_y_max)) {
        return false;
    }
    return true;
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
}
RGBColor VoronoiNode::GetColor() {
    // eg Generate(sample_x-this->x, sample_x-this->y)
    return this->color;
}

//RGBColor VoronoiNode::SampleColor(double sample_x, double sample_y) {
//    // eg Generate(sample_x-this->x, sample_x-this->y)
//    return this->color;
//}

RGBColor VoronoiNode::ForwardPass(double sample_x, double sample_y) {
    // eg Generate(sample_x-this->x, sample_x-this->y)
    return this->color;
}

// dnode1.mag/dnode1.x = node1.x
// dm1/dnode1.mag = m1*(1-m1)
// dfinal_color/dm1 = .5*(node1.color^2)/final_color
// dnode1.color/dnode1.x = something
// dfinal_color/dnode1.color = m1/final_color*(node1.color)
// dloss/dfinal_color = 2(final_color-desired_color)

//x_grad += dloss/dfinal_color * .5*(node1.color^2)/final_color * m*(1-m) * x
void VoronoiNode::BackwardPass(double sample_x, double sample_y, RGBColor final_color, RGBColor dldfinal_col) {
    if (final_color.r < 1) {final_color.r = 1;}
    if (final_color.g < 1) {final_color.g = 1;}
    if (final_color.b < 1) {final_color.b = 1;}
    RGBColor dfinalcoldm = ((this->color*this->color)/final_color)*.5;
    double dldmag = RGBColor::Trace(dldfinal_col*dfinalcoldm) * -this->m_value*(1.0-this->m_value);
    this->x_grad += dldmag * this->x;
    this->y_grad += dldmag * this->y;
    this->color_grad += ((dldfinal_col*this->color)/final_color)*this->m_value;
}

void VoronoiNode::ApplyGradients(double learning_rate) {
//    std::cout << this->x_grad << std::endl;
    SDL_assert(!std::isnan(this->x_grad));
    SDL_assert(!std::isnan(this->y_grad));
    SDL_assert(!std::isnan(this->color_grad.r));
    SDL_assert(!std::isnan(this->color_grad.g));
    SDL_assert(!std::isnan(this->color_grad.b));

    this->x += 10;
//    this->x -= this->x_grad*learning_rate;
//    this->y -= this->y_grad*learning_rate;
//    this->color -= this->color_grad*learning_rate*100.0;
    this->x_grad = 0;
    this->y_grad = 0;
    this->color_grad.r = 0;
    this->color_grad.g = 0;
    this->color_grad.b = 0;
}

void VoronoiNode::CalculateExp(double offset) {
    this->exp = std::exp(-this->mag + offset);
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

void VoronoiNode::CalculateDist(double from_x, double from_y, double gain) {
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->mag = (dx*dx+dy*dy)*gain;
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
