#include "VoronoiGraph.h"
#include "CApp.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

bool VQuadTree::Contains(VQuadTree* tree, VoronoiNode* node) {
    if (tree == nullptr) {return false;}
    if (!tree->node_children.empty()) {
        if (find(tree->node_children.begin(), tree->node_children.end(), node) == tree->node_children.end()) {
            return false;
        }
        return true;
    } else {
        size_t search_first;
        int dx = node->sort.x-tree->half_x;
        int dy = node->sort.y-tree->half_y;
        if (dx < 0) {
            if (dy < 0) {
                search_first = 0;
            } else {
                search_first = 2;
            }
        } else {
            if (dy < 0) {
                search_first = 1;
            } else {
                search_first = 3;
            }
        }
        if (VQuadTree::Contains(tree->tree_children[search_first], node)) {
            return true;
        }
        if (VQuadTree::Contains(tree->tree_children[(search_first+1)%4], node)) {
            return true;
        }
        if (VQuadTree::Contains(tree->tree_children[(search_first+2)%4], node)) {
            return true;
        }
        if (VQuadTree::Contains(tree->tree_children[(search_first+3)%4], node)) {
            return true;
        }
        return false;
    }
}

void PrintIndents(int indent) {
    for (int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
}
void VQuadTree::Print(VQuadTree* tree, int indent) {
    if (tree == nullptr) {
        std::cout << "invalid tree, null pointer" << std::endl;
        return;
    }
    std::cout << "{" << std::endl;
    PrintIndents(indent+1); std::cout << "total_children: " << tree->total_children << "," << std::endl;
    PrintIndents(indent+1); std::cout << "depth: " << tree->depth << "," << std::endl;
    PrintIndents(indent+1); std::cout << "min_(x,y): (" << tree->min_x << "," << tree->min_y << ")" << std::endl;
    PrintIndents(indent+1); std::cout << "max_(x,y): (" << tree->max_x << "," << tree->max_y << ")" << std::endl;
    PrintIndents(indent+1); std::cout << "half_(x,y): (" << tree->half_x << "," << tree->half_y << ")" << std::endl;
    PrintIndents(indent+1); std::cout << "node_children: [" << std::endl;
        VoronoiNode::PrintList(tree->node_children, indent+2);
    PrintIndents(indent+1); std::cout << "]," << std::endl;
//    PrintIndents(indent+1); //NodeLinkedList::PrintNodes("node_children: ", tree->node_children, indent+1); // debug edit nevermind//NodeLinkedList::Print("node_children: ", tree->node_children, indent+1);//
    if (tree->tree_children[0] == nullptr) {
        PrintIndents(indent+1); std::cout << "tree_children: []" << std::endl;
    } else {
        PrintIndents(indent+1); std::cout << "tree_children: [" << std::endl;
        PrintIndents(indent+2); std::cout << "[0]: "; VQuadTree::Print(tree->tree_children[0], indent+2);
        PrintIndents(indent+2); std::cout << "[1]: "; VQuadTree::Print(tree->tree_children[1], indent+2);
        PrintIndents(indent+2); std::cout << "[2]: "; VQuadTree::Print(tree->tree_children[2], indent+2);
        PrintIndents(indent+2); std::cout << "[3]: "; VQuadTree::Print(tree->tree_children[3], indent+2);
        PrintIndents(indent+1); std::cout << "]" << std::endl;
    }
    PrintIndents(indent); std::cout<< "}" << std::endl;
}

void VoronoiQuadTree::RenderTree(SDL_Renderer* target_renderer, AtlasNumberDrawer* number_renderer) {
    this->RenderVQuadTree(this->root, target_renderer);
    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
        VoronoiNode::Render(current_node, target_renderer, number_renderer);
    });
}
void VoronoiQuadTree::RenderVQuadTree(VQuadTree* branch, SDL_Renderer* target_renderer) {
    if (branch == nullptr) {return;}
    SDL_Rect dest = {0,0,0,0};
    if (branch->tree_children[0] != nullptr) {

        this->RenderVQuadTree(branch->tree_children[0], target_renderer);
        this->RenderVQuadTree(branch->tree_children[1], target_renderer);
        this->RenderVQuadTree(branch->tree_children[2], target_renderer);
        this->RenderVQuadTree(branch->tree_children[3], target_renderer);

        SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        dest = {branch->half_x-1, branch->half_y-3, 3, 7}; SDL_RenderDrawRect(target_renderer, &dest);
        dest = {branch->half_x-3, branch->half_y-1, 7, 3}; SDL_RenderDrawRect(target_renderer, &dest);
        SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
        dest = {branch->half_x-0, branch->half_y-3, 1, 7}; SDL_RenderDrawRect(target_renderer, &dest);
        dest = {branch->half_x-3, branch->half_y-0, 7, 1}; SDL_RenderDrawRect(target_renderer, &dest);
    } else {
        SDL_Rect dest = {0,0,0,0};
        SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        dest = {branch->half_x-1, branch->half_y-1, 3, 3}; SDL_RenderDrawRect(target_renderer, &dest);
        SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
        dest = {branch->half_x-0, branch->half_y-1, 1, 3}; SDL_RenderDrawRect(target_renderer, &dest);
        dest = {branch->half_x-1, branch->half_y-0, 3, 1}; SDL_RenderDrawRect(target_renderer, &dest);
    }
    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
    dest = {branch->min_x, branch->min_y, branch->max_x-branch->min_x, branch->max_y-branch->min_y}; SDL_RenderDrawRect(target_renderer, &dest);
    SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    dest = {branch->min_x+1, branch->min_y+1, branch->max_x-branch->min_x-2, branch->max_y-branch->min_y-2}; SDL_RenderDrawRect(target_renderer, &dest);
}

void PrintNodeVector(std::string comment, const std::vector<VoronoiNode*>& c) {
    std::cout << comment << '[';
    bool first{true};
    for (const auto& x : c)
        std::cout << (first ? first = false, "" : ", ") << x;
    std::cout << "]\n";
}
void VoronoiQuadTree::PrintTree() {
//    PrintNodeVector("all_child_nodes: ", this->all_child_nodes);
    std::cout << "all_child_nodes: [" << std::endl;
    VoronoiNode::PrintList(this->all_child_nodes, 1);
    std::cout << "]" << std::endl;
    VQuadTree::Print(this->root, 0);
}
