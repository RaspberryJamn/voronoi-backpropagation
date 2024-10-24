#include "VoronoiNode.h"

VoronoiNode::VoronoiNode(double x, double y) {
    this->x = x;
    this->y = y;
    this->sorting_x = (int)x;
    this->sorting_y = (int)y;
    this->sorting_x_min = 0;
    this->sorting_x_max = 0;
    this->sorting_y_min = 0;
    this->sorting_y_max = 0;
    this->parent = nullptr;
}

VoronoiNode::~VoronoiNode() {
    this->x = 0;
    this->y = 0;
    this->sorting_x = 0;
    this->sorting_y = 0;
}

void VoronoiNode::SetParent(VoronoiGraph* parent, int min_x, int max_x, int min_y, int max_y) {
    this->parent = parent;
    this->sorting_x_min = min_x;
    this->sorting_x_max = max_x;
    this->sorting_y_min = min_y;
    this->sorting_y_max = max_y;
}

VoronoiGraph* VoronoiNode::GetParent() {
    return this->parent;
}

double VoronoiNode::GetX() {
    return this->x;
}

double VoronoiNode::GetY() {
    return this->y;
}

int VoronoiNode::GetSortingPosX() {
    return this->sorting_x;
}

int VoronoiNode::GetSortingPosY() {
    return this->sorting_y;
}
