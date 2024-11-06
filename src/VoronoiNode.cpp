#include "VoronoiNode.h"
#include <cmath>

VoronoiNode::VoronoiNode(double x, double y) {
    this->Init(x, y, 128+120*std::cos(x/100), 128+120*std::cos(x/70+10), 128+120*std::cos((x+y)/180));
}
VoronoiNode::VoronoiNode(double x, double y, double r, double g, double b) {
    this->Init(x, y, r, g, b);
}
void VoronoiNode::Init(double x, double y, double r, double g, double b) {
    this->x = x;
    this->y = y;
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
    this->x_grad = 0;
    this->y_grad = 0;
    this->color_grad[0] = 0;
    this->color_grad[1] = 0;
    this->color_grad[2] = 0;

    this->residential_slot = nullptr;
    this->tree_slot = nullptr;

    this->mag = 0;
    this->sorting_dist = 0;
    this->sorting_x = (int)x;
    this->sorting_y = (int)y;
    this->sorting_x_min = 0;
    this->sorting_x_max = 0;
    this->sorting_y_min = 0;
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
void VoronoiNode::SetBounds(int min_x, int max_x, int min_y, int max_y) {
    this->sorting_x_min = min_x;
    this->sorting_x_max = max_x;
    this->sorting_y_min = min_y;
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

RGBColor VoronoiNode::SampleColor(double sample_x, double sample_y) {
    // eg Generate(sample_x-this->x, sample_x-this->y)
    return (RGBColor){this->color[0],this->color[1],this->color[2]};
}
void VoronoiNode::CalculateExp(double offset) {
    this->exp = std::exp(-this->mag + offset);
}

double VoronoiNode::GetExp() {
    return this->exp;
}

void VoronoiNode::CalculateDist(double from_x, double from_y, double gain) {
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->mag = (dx*dx+dy*dy)*gain;
}

double VoronoiNode::GetDist() {
    return this->mag;
}

void VoronoiNode::CalculateSortingDist() {
    this->sorting_dist = (int)(std::ceil(std::sqrt(this->mag)));
}

int VoronoiNode::GetSortingDist() {
    return this->sorting_dist;
}

void VoronoiNode::CalculateSortingPos() {
    this->sorting_x = (int)x;
    this->sorting_y = (int)y;
}

int VoronoiNode::GetSortingPosX() {
    return this->sorting_x;
}

int VoronoiNode::GetSortingPosY() {
    return this->sorting_y;
}
