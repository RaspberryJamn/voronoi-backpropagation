#include "VoronoiNode.h"
#include <cmath>

VoronoiNode::VoronoiNode(double x, double y) {
    this->x = x;
    this->y = y;
    this->neg_sq_dist = 0;
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

void VoronoiNode::SetBounds(int min_x, int max_x, int min_y, int max_y) {
    this->sorting_x_min = min_x;
    this->sorting_x_max = max_x;
    this->sorting_y_min = min_y;
    this->sorting_y_max = max_y;
}

double VoronoiNode::GetX() {
    return this->x;
}

double VoronoiNode::GetY() {
    return this->y;
}

void VoronoiNode::UpdateDist(double from_x, double from_y) {
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->neg_sq_dist = -(dx*dx+dy*dy);
}

void VoronoiNode::UpdateSortingDist() {
    this->sorting_dist = (int)(std::ceil(std::sqrt(-this->neg_sq_dist)));
}

double VoronoiNode::GetDist() {
    return this->neg_sq_dist;
}

int VoronoiNode::GetSortingDist() {
    return this->sorting_dist;
}

void VoronoiNode::UpdateSortingPos() {
    this->sorting_x = (int)x;
    this->sorting_y = (int)y;
}

int VoronoiNode::GetSortingPosX() {
    return this->sorting_x;
}

int VoronoiNode::GetSortingPosY() {
    return this->sorting_y;
}
