#include "VoronoiGraph.h"
#include "CApp.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

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
//    PrintIndents(indent+1); NodeLinkedList::Print("node_children: ", tree->node_children, indent+1);
    PrintIndents(indent+1); //NodeLinkedList::PrintNodes("node_children: ", tree->node_children, indent+1); // debug edit nevermind//NodeLinkedList::Print("node_children: ", tree->node_children, indent+1);//
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
bool VQuadTree::Contains(VQuadTree* tree, VoronoiNode* node) {
    if (tree == nullptr) {return false;}
    if (!tree->node_children.empty()) {
//        return NodeLinkedList::Contains(tree->node_children, node);
        if (find(tree->node_children.begin(), tree->node_children.end(), node) == tree->node_children.end()) {
            return false;
        }
        return true;
    } else {
        size_t search_first;
        int dx = node->GetSortingPosX()-tree->half_x;
        int dy = node->GetSortingPosX()-tree->half_y;
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
bool VoronoiGraph::EnsureCompleteContainment() {
//    if (this->all_child_nodes.size() != this->total_child_count) {
//        std::cout << "all_child_nodes length fail" << std::endl;
//        return false;
//    }
//    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
//        if (current_node->GetResidence()->node != current_node) { // a bit redundant
//            std::cout << "node residence does not contain node, node: ";
//            current_node->Print(0); std::cout << std::endl;
//            return false;
//        }
//        if (current_node->GetTreeSlot()->node != current_node) {
//            std::cout << "node tree reference does not contain node, node: ";
//            current_node->Print(0); std::cout << std::endl;
//            return false;
//        }
//        if (!VQuadTree::Contains(this->root, current_node)) {
//            std::cout << "tree containment fail, " << current_node << " not contained" << std::endl;
//            current_node->Print(0); std::cout << std::endl;
//            return false;
//        }
//    });
//    NODELINKEDLIST_FOREACH(this->all_child_nodes, {
//        if (current_node->GetResidence()->node != current_node) { // a bit redundant
//            std::cout << "node residence does not contain node, node: ";
//            current_node->Print(0); std::cout << std::endl;
//            return false;
//        }
//        if (current_node->GetTreeSlot()->node != current_node) {
//            std::cout << "node tree reference does not contain node, node: ";
//            current_node->Print(0); std::cout << std::endl;
//            return false;
//        }
//        if (!VQuadTree::Contains(this->root, current_node)) {
//            std::cout << "tree containment fail, " << current_node << " not contained" << std::endl;
//            current_node->Print(0); std::cout << std::endl;
//            return false;
//        }
//    });
    return true;
}
void VoronoiGraph::RenderTree(SDL_Renderer* target_renderer) {
    this->RenderVQuadTree(this->root, target_renderer);
//    SDL_Rect dest = {0,0,0,0};
//    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
//    dest = {this->x, this->y, this->w, this->h}; SDL_RenderDrawRect(target_renderer, &dest);
//    SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//    dest = {this->x+1, this->y+1, this->w-2, this->h-2}; SDL_RenderDrawRect(target_renderer, &dest);
    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
        current_node->Render(target_renderer);
    });
//    NODELINKEDLIST_FOREACH(this->all_child_nodes, {
//        current_node->Render(target_renderer);
//    });
}
void VoronoiGraph::RenderVQuadTree(VQuadTree* branch, SDL_Renderer* target_renderer) {
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

//    if (branch->node_children != nullptr) {
//        NODELINKEDLIST_FOREACH(branch->node_children, {
//            current_node->Render(target_renderer);
//        });
//    }
}

void println(std::string comment, const std::vector<VoronoiNode*>& c) {
    std::cout << comment << '[';
    bool first{true};
    for (const auto& x : c)
        std::cout << (first ? first = false, "" : ", ") << x;
    std::cout << "]\n";
}
void VoronoiGraph::PrintTree() {
//    std::cout << "all_child_nodes: " << this->all_child_nodes << std::endl;
    println("all_child_nodes: ", this->all_child_nodes);
//    NodeLinkedList::PrintNodes("all_child_nodes: ", this->all_child_nodes, 0);
    VQuadTree::Print(this->root, 0);
}

VoronoiGraph::VoronoiGraph() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->root = new VQuadTree(this->x, this->y, this->x+this->w, this->y+this->h, 0);

    this->all_child_nodes.clear();
    this->total_child_count = 0;

    this->gain = 0;

    this->nearby_candidates.clear();

    this->recent_x = 0;
    this->recent_y = 0;
    this->recent_sort_x = 0;
    this->recent_sort_y = 0;
    this->band_width = 0;
    this->sort_band_width = 0;
    this->current_bounding_mag = 0;
    this->current_box_radius = 0;
}

VoronoiGraph::~VoronoiGraph() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->DeleteTree(this->root);
    this->root = nullptr;

//    delete this->all_child_nodes;
//    NodeLinkedList::DeleteNodes(this->all_child_nodes);
//    this->all_child_nodes = nullptr;
    this->total_child_count = 0;

    this->gain = 0;

//    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates.clear();

    this->recent_x = 0;
    this->recent_y = 0;
    this->recent_sort_x = 0;
    this->recent_sort_y = 0;
    this->band_width = 0;
    this->sort_band_width = 0;
    this->current_bounding_mag = 0;
    this->current_box_radius = 0;
}

void VoronoiGraph::DeleteTree(VQuadTree* branch) {
    if (branch->node_children.empty()) {
        if (branch->tree_children[0] != nullptr) {
            this->DeleteTree(branch->tree_children[0]); branch->tree_children[0] = nullptr;
            this->DeleteTree(branch->tree_children[1]); branch->tree_children[1] = nullptr;
            this->DeleteTree(branch->tree_children[2]); branch->tree_children[2] = nullptr;
            this->DeleteTree(branch->tree_children[3]); branch->tree_children[3] = nullptr;
        }
    } else {
//        NodeLinkedList::DeleteList(branch->node_children);
        branch->node_children.clear();
    }
    delete branch;
}

void VoronoiGraph::RespecTree(int x, int y, int w, int h, int max_depth, int critical_mass) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->max_depth = max_depth;
    this->critical_mass = critical_mass;
    this->total_child_count = this->total_child_count;

    this->DeleteTree(this->root); // simply delete the entirety of the tree, no need to remove every node before readding it later
    this->root = nullptr;
    this->root = new VQuadTree(this->x, this->y, this->x+this->w, this->y+this->h, 0);
//    if (this->root == nullptr) {
////        this->DeleteTree(this->root);
//        this->root = new VQuadTree(this->x, this->y, this->x+this->w, this->y+this->h, 0);
//    } else {
//        std::cout << "removeing all nodes" << std::endl;
//        NODELINKEDLIST_FOREACH(this->all_child_nodes, {
//            current_node->CalculateSortingPos();
//            this->recent_sort_x = current_node->GetSortingPosX();
//            this->recent_sort_y = current_node->GetSortingPosY();
//            this->RemoveFromBranch(current_node, this->root);
//        });
//    }
//    std::cout << "removed all nodes" << std::endl;
//    NodeLinkedList* all_nodes_copy = NodeLinkedList::Copy(this->all_child_nodes);
    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
        current_node->CalculateSortingPos();
        this->recent_sort_x = current_node->GetSortingPosX();
        this->recent_sort_y = current_node->GetSortingPosY();
        this->AddToChildren(current_node, this->root);
    });
//    NODELINKEDLIST_FOREACH(this->all_child_nodes, {//all_nodes_copy, {//
////        current_node->SetTreeSlot(nullptr);
//        current_node->CalculateSortingPos();
//        this->recent_sort_x = current_node->GetSortingPosX();
//        this->recent_sort_y = current_node->GetSortingPosY();
//        this->AddToChildren(current_node, this->root);
//    });
//    std::cout << "added all nodes" << std::endl;
//    NodeLinkedList::DeleteList(all_nodes_copy);
}

bool VoronoiGraph::SplitValid(VQuadTree* branch) {
    return (branch->total_children > this->critical_mass) && (branch->depth < this->max_depth);
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    this->all_child_nodes.push_back(node);
//    NodeLinkedList::Append(node, &this->all_child_nodes);
    node->SetResidence(nullptr);//this->all_child_nodes);
//    NodeLinkedList::AddResidenceSlot(node, &this->all_child_nodes);
    this->total_child_count++;

    node->CalculateSortingPos(); // this is where the new node's sort position is actually ascertained
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->AddToChildren(node, this->root);
}
void VoronoiGraph::AddToChildren(VoronoiNode* node, VQuadTree* branch) {

    if (this->SplitValid(branch)) { // after the split would happen, and on that point it had been allowed to do so

        this->AddToChildrenSplit(node, branch);
        branch->total_children++;

    } else { // we're either below critical mass or above it and weren't allowed to split

//        NodeLinkedList::Append(node, &branch->node_children); // node given a slot
        branch->node_children.push_back(node);
//        node->SetTreeSlot(branch->node_children);
        branch->total_children++;
//        NodeLinkedList::AddTreeSlot(node, &branch->node_children);
        node->SetBounds(branch->min_x, branch->min_y, branch->max_x, branch->max_y); // node knows its shape


        if (this->SplitValid(branch)) { // or more specifically, does it _now_ qualify for splitting

            VQuadTree::CalculateHalfXY(branch);

            branch->tree_children[0] = new VQuadTree(branch->min_x , branch->min_y , branch->half_x, branch->half_y, branch->depth+1);
            branch->tree_children[1] = new VQuadTree(branch->half_x, branch->min_y , branch->max_x , branch->half_y, branch->depth+1);
            branch->tree_children[2] = new VQuadTree(branch->min_x , branch->half_y, branch->half_x, branch->max_y , branch->depth+1);
            branch->tree_children[3] = new VQuadTree(branch->half_x, branch->half_y, branch->max_x , branch->max_y , branch->depth+1);

            std::for_each(branch->node_children.begin(), branch->node_children.end(), [&](VoronoiNode* current_node) {

//            });
//            NODELINKEDLIST_FOREACH(branch->node_children, {
                VoronoiNode* repositioned_node = current_node; // get the node
                this->recent_sort_x = repositioned_node->GetSortingPosX(); // and its position (NOT recalculated)
                this->recent_sort_y = repositioned_node->GetSortingPosY();
//                repositioned_node->SetTreeSlot(nullptr); // testing adding this
//                delete current_slot; // and their old slot is deleted // and doing this a step earlier
                this->AddToChildrenSplit(repositioned_node, branch); // the nodes in each slot find their new home
            });

            branch->node_children.clear(); // at the end, everything in the list has been deleted, so the head of the list is cleared
        }
    }
}
void VoronoiGraph::AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch) {
    if (this->recent_sort_y < branch->half_y) {
        if (this->recent_sort_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[0]);
        } else {
            this->AddToChildren(node, branch->tree_children[1]);
        }
    } else {
        if (this->recent_sort_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[2]);
        } else {
            this->AddToChildren(node, branch->tree_children[3]);
        }
    }
}

/* QUARANTINE REMOVAL CODE
void VoronoiGraph::RemoveNode(VoronoiNode* node) {
    NodeLinkedList::RemoveResidence(node, &this->all_child_nodes); // here is where the node is officially located, now gone
    // the node's sort position should not be recalculated now as that would mess with the function's ability to actually find the node
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->RemoveFromChildren(node, this->root); // this, in effect, is really just about consolidating branches no longer above critical mass
}
void VoronoiGraph::RemoveFromChildren(VoronoiNode* node, VQuadTree* branch) {
    if (!this->SplitValid(branch)) { // this branch is below critical mass or is above but was disallowed splitting

        if (!NodeLinkedList::Contains(branch->node_children, node)) {
            std::cout << "failed contains check in remove" << std::endl;
            std::cout << "attempted removed node: "; node->Print(0); std::cout << std::endl;
            std::cout << "erroring branch: "; VQuadTree::Print(branch, 0);
            this->PrintTree();
        }
        SDL_assert(NodeLinkedList::Contains(branch->node_children, node));

        NodeLinkedList::RemoveTreeLocation(node, &branch->node_children);
        branch->total_children--; // if below critical mass, will stay below critical mass. if above, well if it couldnt split before it wouldnt need to merge now

    } else { // above critical mass and was allowed to split, aka does have tree children and does not have node children
        if (this->recent_sort_y < branch->half_y) {
            if (this->recent_sort_x < branch->half_x) {
                this->RemoveFromChildren(node, branch->tree_children[0]);
            } else {
                this->RemoveFromChildren(node, branch->tree_children[1]);
            }
        } else {
            if (this->recent_sort_x < branch->half_x) {
                this->RemoveFromChildren(node, branch->tree_children[2]);
            } else {
                this->RemoveFromChildren(node, branch->tree_children[3]);
            }
        }
        branch->total_children--;
        if (branch->total_children <= this->critical_mass) { // (any child has critical mass) -> (parent has critical mass), therefore (parent does not have critical mass) -> (no child has critical mass)
            this->ConsolidateChildLists(branch);
        }
    }
}
void VoronoiGraph::ConsolidateChildLists(VQuadTree* branch) {
    NodeLinkedList* growing_start = branch->tree_children[0]->node_children;
    delete branch->tree_children[0];
    branch->tree_children[0] = nullptr;

    for (int i = 1; i <= 3; i++) {
        NodeLinkedList* prepend_start = branch->tree_children[i]->node_children;
        delete branch->tree_children[i];
        branch->tree_children[i] = nullptr;

        if (prepend_start != nullptr) {
            NodeLinkedList* sliding_slot = prepend_start;
            NodeLinkedList* next_slot = sliding_slot->next;
            while (next_slot != nullptr) {
                sliding_slot = next_slot;
                next_slot = sliding_slot->next;
            } // now sliding_slot has the end of the newcomer list
            sliding_slot->next = growing_start;
            growing_start = prepend_start;
        }
    }

    branch->node_children = growing_start;
}
*/

void VoronoiGraph::RemoveNode(VoronoiNode* node) {
//    SDL_assert(NodeLinkedList::Contains(this->all_child_nodes, node));
    this->all_child_nodes.erase(find(this->all_child_nodes.begin(), this->all_child_nodes.end(), node));
//    NodeLinkedList::RemoveGeneric(node, &this->all_child_nodes);
    this->total_child_count--;
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->RemoveFromBranch(node, this->root);
}

void VoronoiGraph::RemoveFromBranch(VoronoiNode* node, VQuadTree* branch) {
    if (this->SplitValid(branch)) { // branched
        if (this->recent_sort_y < branch->half_y) {
            if (this->recent_sort_x < branch->half_x) {
                this->RemoveFromBranch(node, branch->tree_children[0]);
            } else {
                this->RemoveFromBranch(node, branch->tree_children[1]);
            }
        } else {
            if (this->recent_sort_x < branch->half_x) {
                this->RemoveFromBranch(node, branch->tree_children[2]);
            } else {
                this->RemoveFromBranch(node, branch->tree_children[3]);
            }
        }
        branch->total_children--;
        if (!this->SplitValid(branch)) {// should the trees remerge?
            this->ConsolidateChildLists(branch);
        }
        return;
    } else { // leaf
//        NodeLinkedList::RemoveGeneric(node, &branch->node_children);
        branch->node_children.erase(find(branch->node_children.begin(), branch->node_children.end(), node));
        branch->total_children--;
    }
}

void VoronoiGraph::ConsolidateChildLists(VQuadTree* branch) {
//    NodeLinkedList* concatenated = nullptr;
//    NodeLinkedList::LinkAOntoB(&concatenated, branch->tree_children[0]->node_children); // fancy way of saying concatenated = branch->tree_children[0]->node_children
//    NodeLinkedList::LinkAOntoB(&concatenated, branch->tree_children[1]->node_children); // tree[0]nodes tail points into tree[1]nodes
//    NodeLinkedList::LinkAOntoB(&concatenated, branch->tree_children[2]->node_children); // tree[1]nodes tail points into tree[2]nodes
//    NodeLinkedList::LinkAOntoB(&concatenated, branch->tree_children[3]->node_children); // tree[2]nodes tail points into tree[3]nodes
//    branch->node_children = concatenated;
//    NodeLinkedList* concatenated = nullptr;
    std::vector<VoronoiNode*> concatenated = {};
    for (int i = 0; i < 4; i++) {
        std::for_each(branch->tree_children[i]->node_children.begin(), branch->tree_children[i]->node_children.end(), [&](VoronoiNode* current_node) {
            concatenated.push_back(current_node);
        });
        delete branch->tree_children[i];
        branch->tree_children[i] = nullptr;
    }
}

void VoronoiGraph::UpdateNodePositions() {
    SDL_assert(this->EnsureCompleteContainment());
    this->RespecTree(this->x, this->y, this->w, this->h, this->max_depth, this->critical_mass);
    SDL_assert(this->EnsureCompleteContainment());
////    NodeLinkedList::PrintNodes("all nodes:", this->all_child_nodes, 0);
//
////    NodeLinkedList* nodes_to_update = nullptr;
//    NodeLinkedList* list_copy = NodeLinkedList::Copy(this->all_child_nodes);
//
//    NODELINKEDLIST_FOREACH(list_copy, {
//        std::cout << "loop once, ";
//        // try always removing and adding every node
//        std::cout << "[" << current_slot <<"]: " << current_node <<", next: " << current_slot->next << ", progress: 0";
//
////        if (!current_node->IsBounded(current_node->GetSortingPosX(),
////                                     current_node->GetSortingPosY())
////            ) {
////            std::cout << std::endl << "node wasnt bounded after not moving: "; current_node->Print(0); std::cout << std::endl;
////            this->PrintTree();
////            SDL_assert(false); // node wasnt bounded
////        }
//
//        this->RemoveNode(current_node);
//        std::cout << "1";
//        this->AddNode(current_node);
//        std::cout << "2" << std::endl;
//
////        NodeLinkedList::Append(current_node, &nodes_to_update); // the node lives in this list now, nowhere else
////        int x_copy = current_node->GetSortingPosX(); // store a copy of the old sorting position before the most recent move
////        int y_copy = current_node->GetSortingPosY();
////        current_node->CalculateSortingPos(); // you moved, therefore you should have an updated sorting position
////        if (!current_node->IsBounded()) { // now, have you left the box you were in just a moment ago?
////            std::cout << "relocating" << std::endl;
////            this->recent_sort_x = x_copy;
////            this->recent_sort_y = y_copy;
////            this->RemoveFromChildren(current_node, this->root); // use the old for removing it
////            this->recent_sort_x = current_node->GetSortingPosX();// and the new for adding it
////            this->recent_sort_y = current_node->GetSortingPosY();
////            this->AddToChildren(current_node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
////        }
//    });
////    NodeLinkedList::PrintNodes("updating nodes:", nodes_to_update, 0);
////    NODELINKEDLIST_FOREACH(nodes_to_update, {
////        std::cout << "loop twice, ";
////        std::cout << "node: " << current_node;
////        std::cout << ", next: " << current_slot << std::endl;
////        this->AddNode(current_node);
////    });
////    NodeLinkedList::DeleteList(nodes_to_update); // these nodes all have homes now
//    std::cout << "finished loop" << std::endl;
}

std::vector<VoronoiNode*> VoronoiGraph::GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed) {
    this->SetGain(gain);
    this->SetBandWidth(band_width);
    return this->GetNearby(x, y, seed);
}
std::vector<VoronoiNode*> VoronoiGraph::GetNearby(double x, double y, VoronoiNode* seed) {
    SDL_assert(this->EnsureCompleteContainment());
    this->recent_x = x;
    this->recent_y = y;
    this->recent_sort_x = (int)this->recent_x;
    this->recent_sort_y = (int)this->recent_y;

    if (seed != nullptr) {
        seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = seed->GetDist() + this->band_width;
        this->current_box_radius = seed->GetSortingDist() + this->sort_band_width;
    } else {
        VoronoiNode* arbitrary_seed = this->all_child_nodes.front();
        arbitrary_seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        arbitrary_seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = arbitrary_seed->GetDist() + this->band_width;
        this->current_box_radius = arbitrary_seed->GetSortingDist() + this->sort_band_width;
    }

//    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates.clear();

//    SDL_assert(this->EnsureCompleteContainment());
        this->BuildNearbyList(this->root); // this is where the job is mostly done
    SDL_assert(this->EnsureCompleteContainment());

    SDL_assert(!this->nearby_candidates.empty());

    std::vector<VoronoiNode*> result_list = {}; // start with an empty output list

    size_t nearest_slot = 0; // we're gonna be searching for the nearest node to swap to the front
    size_t current_slot = 0;
    double nearest_dist = this->nearby_candidates.front()->GetDist();

    SDL_assert(this->EnsureCompleteContainment());
//    NODELINKEDLIST_FOREACH(this->nearby_candidates, {
//        double current_distance = current_node->GetDist();
//        if (current_distance <= this->current_bounding_mag) { // scanned node passes the check, may have significant m value
//
//            current_slot->next = result_list; // this candidate slot gets prepened onto result_list
//            result_list = current_slot;
//
//            if (current_distance < nearest_dist) {
//                nearest_slot = current_slot;
//                nearest_dist = current_distance;
//            }
//
//        } else { // node fails the check, will definitely not have significant m value
//            delete current_slot;
//        }
//    });
    for (std::vector<VoronoiNode*>::iterator it = this->nearby_candidates.begin(); it != this->nearby_candidates.end();) {

        double current_distance = (*it)->GetDist();
        if (current_distance <= this->current_bounding_mag) { // scanned node passes the check, may have significant m value

            if (current_distance < nearest_dist) {
                nearest_slot = current_slot;
                nearest_dist = current_distance;
            }
            current_slot++;
            ++it;

        } else { // node fails the check, will definitely not have significant m value
//            current_slot++;
            it = this->nearby_candidates.erase(it);
        }
    }
//    SDL_assert(this->EnsureCompleteContainment());

    if (nearest_slot != 0) { // puts the nearest node at the top
//        VoronoiNode* first_node = result_list->node;
//        result_list->node = nearest_slot->node; // list[0] gets the nearest node
//        nearest_slot->node = first_node; // list[nearest] gets the first node
        VoronoiNode* temp_first = this->nearby_candidates.front();
        this->nearby_candidates.front() = this->nearby_candidates.at(nearest_slot);
        this->nearby_candidates.at(nearest_slot) = temp_first;
    }
//    SDL_assert(this->EnsureCompleteContainment());
    this->nearby_candidates.swap(result_list);
    return result_list;
}
void VoronoiGraph::BuildNearbyList(VQuadTree* branch) { // takes the running info on the best distance upper bound and adds the nodes descendant to this branch to the list of potential candidates
    if (!this->SplitValid(branch)) { // not split, this is a leaf node

//        SDL_assert(this->EnsureCompleteContainment());

        VoronoiNode* new_nearest = nullptr;
        std::for_each(branch->node_children.begin(), branch->node_children.end(), [&](VoronoiNode* current_node) {

//        NODELINKEDLIST_FOREACH(branch->node_children, {

            current_node->CalculateDist(this->recent_x, this->recent_y, this->gain);
            double relative_dist = this->current_bounding_mag-current_node->GetDist(); // from this node to the outer circle
            if (relative_dist > 0) { // beats the outer circle

//                NodeLinkedList* new_candidate_entry = new NodeLinkedList();
//                new_candidate_entry->node = current_node;
//                new_candidate_entry->next = this->nearby_candidates; // prepend the new candidate
//                this->nearby_candidates = new_candidate_entry;

//                NodeLinkedList* new_first = new NodeLinkedList();
//                VoronoiNode* uninit_node = new_first->node;
//                NodeLinkedList* uninit_next = new_first->next;
//
////                SDL_assert(NodeLinkedList::Contains(branch->node_children, new_first) == false); // this is SO fucking stupid
//                if (NodeLinkedList::Contains(branch->node_children, new_first) != false) {
//                    std::cout << "new_first: [" << new_first << "]: " << "{node: " << uninit_node << "} => " << uninit_next << std::endl;
//                    SDL_assert(false);
//                }
//
//                new_first->node = current_node;
//                new_first->next = this->nearby_candidates;
//                this->nearby_candidates = new_first;
//                NodeLinkedList* before_error = this->nearby_candidates;
//                this->nearby_candidates = new NodeLinkedList(current_node, this->nearby_candidates);
//                new_first->node = current_node;
//                new_first->next = this->nearby_candidates;

//                this->nearby_candidates = new NodeLinkedList(current_node, this->nearby_candidates);

//                SDL_assert(this->EnsureCompleteContainment());
//                NodeLinkedList::Append(current_node, &this->nearby_candidates); // someway, somehow, this sparks the issue
                this->nearby_candidates.push_back(current_node);
//                if (!this->EnsureCompleteContainment()) {
////                    std::cout << "before_error: " << before_error << std::endl;
//                    std::cout << "containment fail after appending a node " << current_node << " into slot [" << this->nearby_candidates << "] in branch: "; VQuadTree::Print(branch, 0);
//                    std::cout << "full tree: ";
//                    VQuadTree::Print(this->root, 0);
//                    SDL_assert(false);
//                }

                if (relative_dist >= this->band_width) { // beats the middle circle
                    this->current_bounding_mag = current_node->GetDist()+this->band_width; // now the outer circle is this dist plus out by band width
                    new_nearest = current_node;
                }
            }

        });
//        if (!this->EnsureCompleteContainment()) {
//            std::cout << "containment fail after leaf incorporation, leaf: [" << branch << "]: ";
//            VQuadTree::Print(branch, 0);
//            SDL_assert(false);
//        }
        if (new_nearest != nullptr) { // if a candidate in this branch beat the standing record
            new_nearest->CalculateSortingDist(this->recent_x, this->recent_y); // do the work to find the box radius
            this->current_box_radius = new_nearest->GetSortingDist() + this->sort_band_width; // update the search

            this->current_bounding_mag = new_nearest->GetDist()+this->band_width;
        }

//        SDL_assert(this->EnsureCompleteContainment());

    } else { // split, doesnt itself have child nodes but it's boughs may
/*
 *          0|1
 *          -+- the quadrants
 *          2|3
 *
 *            \ 0123 | 1032 /
 *              \    |    /
 *          0213  \  |  /  1302
 *          ---------+--------- the order in which they should be searched, as indexed by the position of the sample center
 *          2031  /  |  \  3120
 *              /    |    \
 *            / 2301 | 3210 \
 */
        size_t search_1;
        size_t search_2;
        size_t search_3;
        size_t search_4;
        bool search_x_then_y;
        int dx = this->recent_sort_x-branch->half_x; // and now this is the vector position of the sample center, with the branch as the origin
        int dy = this->recent_sort_y-branch->half_y;
        if (dx < 0) {
            if (dy < 0) {
                search_1 = 0;
                search_4 = 3;
            } else {
                search_1 = 2;
                search_4 = 1;
            }
        } else {
            if (dy < 0) {
                search_1 = 1;
                search_4 = 2;
            } else {
                search_1 = 3;
                search_4 = 0;
            }
        }
        if ((search_1 == 0) || (search_1 == 3)) {
            if ( dy < dx ) { // yes towards up left
                search_2 = 1;
                search_3 = 2;
            } else { // towards down right
                search_2 = 2;
                search_3 = 1;
            }
        } else {
            if ( dy < -dx ) { // yes towards up right
                search_2 = 0;
                search_3 = 3;
            } else { // towards down left
                search_2 = 3;
                search_3 = 0;
            }
        }
        search_x_then_y = ((search_2 < search_3) == (dy < 0)); // activates in an hourglass shape

        this->BuildNearbyList(branch->tree_children[search_1]);

        if (search_x_then_y) { // search x first
            // dx is the vector from midpoint to sample
            // (dx is positive, actually so positive its tail on the left cant possibly fit in the box) || (dx is negative, even more negative than the box can accomodate, the tail sticks out to the right)
            if ((dx > this->current_box_radius) || (dx < -this->current_box_radius)) {
                return;
            }
        } else { // search y first
            if ((dy > this->current_box_radius) || (dy < -this->current_box_radius)) {
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_2]);

        if (search_x_then_y) { // search y second
            if ((dy > this->current_box_radius) || (dy < -this->current_box_radius)) {
                return;
            }
        } else { // search x second
            if ((dx > this->current_box_radius) || (dx < -this->current_box_radius)) {
                return;
            }
        }
        this->BuildNearbyList(branch->tree_children[search_3]);

        if ((dx*dx+dy*dy)*this->gain > this->current_bounding_mag) { // center point mag further out than the bounding outer circle mag (bounding mag has gain in it)
            return;
        }
        this->BuildNearbyList(branch->tree_children[search_4]);
    }
}

RGBColor VoronoiGraph::ForwardPassFromNearby(std::vector<VoronoiNode*> nearby, double x, double y) { // nearby nodes already have their distances calculated, aka gain and bandwidth are baked in
    double z = 0;
    double exp_offset = -nearby.front()->GetDist(); // for numerical precision. softmax doesnt care about offsets so long as theyre applied to all applicants

    RGBColor final_color = RGBColor(0,0,0);

//    NODELINKEDLIST_FOREACH(nearby, {
//        current_node->CalculateExp(exp_offset); // work done here
//        z += current_node->GetExp();
//    });
//
//    NODELINKEDLIST_FOREACH(nearby, {
//        double m = current_node->GetExp()/z;
//        current_node->SetM(m);
//        RGBColor c = current_node->ForwardPass(x, y);//node->SampleColor(x, y); // heres the only place x and y are used
//        final_color += c*c*m;
//    });
    std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
        current_node->CalculateExp(exp_offset); // work done here
        z += current_node->GetExp();
    });
    std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
        double m = current_node->GetExp()/z;
        current_node->SetM(m);
        RGBColor c = current_node->ForwardPass(x, y);//node->SampleColor(x, y); // heres the only place x and y are used
        final_color += c*c*m;

    });

    final_color = RGBColor(std::sqrt(final_color.r),
                           std::sqrt(final_color.g),
                           std::sqrt(final_color.b));
    return final_color;
}
void VoronoiGraph::DoBackwardsPassFromNearby(std::vector<VoronoiNode*> nearby, double x, double y, RGBColor image_sample) {

    RGBColor final_color = this->ForwardPassFromNearby(nearby, x, y);

    std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
        current_node->BackwardPass(x, y, final_color, (final_color-image_sample)*2);
    });
}

void VoronoiGraph::UpdateAllGradients(double learning_rate) {
    std::for_each(this->all_child_nodes.begin(), this->all_child_nodes.end(), [&](VoronoiNode* current_node) {
        current_node->ApplyGradients(learning_rate);
    });
//    NODELINKEDLIST_FOREACH(this->all_child_nodes, {
//        current_node->ApplyGradients(learning_rate);
//    });
    this->SetGain(this->gain-this->gain_gradient*learning_rate);
    this->UpdateNodePositions();
}

void VoronoiGraph::SetGain(double gain) {
    this->gain = gain;
    this->SetBandWidth(this->band_width);
}
void VoronoiGraph::SetBandWidth(double band_width) {
    this->band_width = band_width;
    this->sort_band_width = (int)(std::ceil(this->band_width/this->gain));
}
double VoronoiGraph::GetGain() {
    return this->gain;
}

//void VoronoiGraph::SetErrorLogger(ErrorLogger* error_logger) {
//    this->error_logger = error_logger;
//}
