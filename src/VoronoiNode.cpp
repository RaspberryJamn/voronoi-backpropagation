#include "VoronoiNode.h"
#include <vector>
#include <iostream>
//#include <cmath>
//#include <ctime>
//#include "NNLayer/Input.h"
//#include "NNLayer/Sigmoid.h"
//#include "NNLayer/Dense.h"

//void PrintIndents(int indent);
//void VoronoiNode::Print(int indent) {
////    RGBColor c = this->color;
//    std::cout << this << ": {" << std::endl;
//    PrintIndents(indent+1); std::cout << "sort_(x,y): (" << this->sort.x << "," << this->sort.y << ")," << std::endl;
//    PrintIndents(indent+1); std::cout << "precise_(x,y): (" << this->x << "," << this->y << ")," << std::endl;
//    PrintIndents(indent+1); std::cout << "min_corner_(x,y): (" << this->sort.x_min << "," << this->sort.y_min << ")," << std::endl;
//    PrintIndents(indent+1); std::cout << "max_corner_(x,y): (" << this->sort.x_max << "," << this->sort.y_max << ")," << std::endl;
////    PrintIndents(indent+1); std::cout << "color: [" << c.r << "," << c.g << "," << c.b << "]," << std::endl;
//    PrintIndents(indent+1); std::cout << "dist:" << this->model.mag<< "," << std::endl;
////    PrintIndents(indent+1); std::cout << "residency:" << this->residential_slot << "," << std::endl;
////    PrintIndents(indent+1); std::cout << "reference:" << this->tree_slot << std::endl;
//    PrintIndents(indent); std::cout << "}";
//}
//
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


void VoronoiNode::CalculateMag(double from_x, double from_y, double gain) {
//    this->model.gain = gain;
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->sort.mag = (dx*dx+dy*dy)*gain;
    this->model.mag = this->sort.mag;
}

void VoronoiNode::CalculateBoxRadius(double from_x, double from_y) {
    double dx = this->x-from_x;
    double dy = this->y-from_y;
    this->sort.box_radius = (int)(std::ceil(std::sqrt(dx*dx+dy*dy)));
}

void PrintIndents(int indent);
void VoronoiNode::Print(VoronoiNode* node, int indent) {
    PrintIndents(indent); std::cout << node << ": {" << std::endl;
    PrintIndents(indent+1); std::cout << "sort_(x,y): (" << node->sort.x << "," << node->sort.y << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "precise_(x,y): (" << node->x << "," << node->y << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "min_corner_(x,y): (" << node->sort.x_min << "," << node->sort.y_min << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "max_corner_(x,y): (" << node->sort.x_max << "," << node->sort.y_max << ")," << std::endl;
    PrintIndents(indent+1); std::cout << "mag:" << node->sort.mag << "," << std::endl;
    PrintIndents(indent+1); std::cout << "box_radius:" << node->sort.box_radius << "," << std::endl;
    PrintIndents(indent+1); std::cout << "grad_(x,y): (" << node->model.x_grad << "," << node->model.y_grad << ")," << std::endl;
    PrintIndents(indent); std::cout << "}" << std::endl;
}

void VoronoiNode::PrintList(std::vector<VoronoiNode*> node_list, int indent) {
    std::for_each(node_list.begin(), node_list.end(), [&](VoronoiNode* current_node) {
        VoronoiNode::Print(current_node, indent);
    });
}

void VoronoiNode::Render(VoronoiNode* node, SDL_Renderer* target_renderer) {
    SDL_Rect dest = {0,0,0,0};

    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
//    dest = {this->sorting_x-3, this->sorting_y-3, 7, 7}; SDL_RenderDrawRect(target_renderer, &dest);
    dest = {node->sort.x-2, node->sort.y-3, 5, 7}; SDL_RenderDrawRect(target_renderer, &dest);
    dest = {node->sort.x-3, node->sort.y-2, 7, 5}; SDL_RenderDrawRect(target_renderer, &dest);

    SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    dest = {node->sort.x-2, node->sort.y-2, 5, 5}; SDL_RenderDrawRect(target_renderer, &dest);

    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);//(Uint8)this->color.r, (Uint8)this->color.g, (Uint8)this->color.b, 0xFF);
    dest = {node->sort.x-1, node->sort.y-1, 3, 3}; SDL_RenderDrawRect(target_renderer, &dest);
//    SDL_RenderDrawPoint(target_renderer, this->sorting_x, this->sorting_y);
    SDL_RenderDrawLine(target_renderer, node->sort.x, node->sort.y-2, node->sort.x, node->sort.y+2);
    SDL_RenderDrawLine(target_renderer, node->sort.x-2, node->sort.y, node->sort.x+2, node->sort.y);

//    this->RenderLoggedGradient(target_renderer);
}
