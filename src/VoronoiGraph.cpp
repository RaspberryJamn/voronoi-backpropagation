#include "VoronoiGraph.h"
#include "CApp.h"
#include <cmath>
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
    PrintIndents(indent+1); std::cout << "half_x: " << tree->half_x << "," << std::endl;
    PrintIndents(indent+1); std::cout << "half_y: " << tree->half_y << "," << std::endl;
    PrintIndents(indent+1); NodeLinkedList::Print("node_children: ", tree->node_children, indent+1);
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
void VoronoiGraph::RenderTree(SDL_Renderer* target_renderer) {
    this->RenderVQuadTree(this->root, target_renderer);
    SDL_SetRenderDrawColor(target_renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawLine(target_renderer, this->x        , this->y        , this->x+this->w, this->y        );
    SDL_RenderDrawLine(target_renderer, this->x        , this->y        , this->x        , this->y+this->h);
    SDL_RenderDrawLine(target_renderer, this->x+this->w, this->y+this->h, this->x+this->w, this->y        );
    SDL_RenderDrawLine(target_renderer, this->x+this->w, this->y+this->h, this->x        , this->y+this->h);
    SDL_SetRenderDrawColor(target_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(target_renderer, this->x+1        , this->y+1        , this->x+this->w-1, this->y+1        );
    SDL_RenderDrawLine(target_renderer, this->x+1        , this->y+1        , this->x+1        , this->y+this->h-1);
    SDL_RenderDrawLine(target_renderer, this->x+this->w-1, this->y+this->h-1, this->x+this->w-1, this->y+1        );
    SDL_RenderDrawLine(target_renderer, this->x+this->w-1, this->y+this->h-1, this->x+1        , this->y+this->h-1);
    NODELINKEDLIST_FOREACH(this->all_child_nodes, {
        current_node->Render(target_renderer);
    });
}
void VoronoiGraph::RenderVQuadTree(VQuadTree* branch, SDL_Renderer* target_renderer) {
    if (branch == nullptr) {return;}
    if (branch->tree_children[0] != nullptr) {
        this->RenderVQuadTree(branch->tree_children[0], target_renderer);
        this->RenderVQuadTree(branch->tree_children[1], target_renderer);
        this->RenderVQuadTree(branch->tree_children[2], target_renderer);
        this->RenderVQuadTree(branch->tree_children[3], target_renderer);

        SDL_Rect dest = {0,0,0,0};
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
//    if (branch->node_children != nullptr) {
//        NODELINKEDLIST_FOREACH(branch->node_children, {
//            current_node->Render(target_renderer);
//        });
//    }
}

void VoronoiGraph::PrintTree() {
    VQuadTree::Print(this->root, 0);
}

VoronoiGraph::VoronoiGraph() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
    this->max_depth = 0;
    this->critical_mass = 0;

    this->root = new VQuadTree(this->x+this->w/2, this->y+this->h/2, 0);

    this->all_child_nodes = nullptr;
    this->total_child_count = 0;

    this->gain = 0;

    this->nearby_candidates = nullptr;

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

    NodeLinkedList::DeleteNodes(this->all_child_nodes);
    this->all_child_nodes = nullptr;
    this->total_child_count = 0;

    this->gain = 0;

    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;

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
    if (branch->node_children == nullptr) {
        if (branch->tree_children[0] != nullptr) {
            this->DeleteTree(branch->tree_children[0]);
            this->DeleteTree(branch->tree_children[1]);
            this->DeleteTree(branch->tree_children[2]);
            this->DeleteTree(branch->tree_children[3]);
        }
    } else {
        NodeLinkedList::DeleteList(branch->node_children);
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

    this->DeleteTree(this->root); // simply delete the entirety of the tree, no need to remove every node before readding it later
    this->root = new VQuadTree(this->x+this->w/2, this->y+this->h/2, 0);

    NODELINKEDLIST_FOREACH(this->all_child_nodes, {
        current_node->CalculateSortingPos();
        this->recent_sort_x = current_node->GetSortingPosX();
        this->recent_sort_y = current_node->GetSortingPosY();
        this->AddToChildren(current_node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
    });
}

bool VoronoiGraph::SplitValid(VQuadTree* branch) {
    return (branch->total_children > this->critical_mass) && (branch->depth < this->max_depth);
}

void VoronoiGraph::AddNode(VoronoiNode* node) {
    NodeLinkedList::Append(node, &this->all_child_nodes);
    node->SetResidence(this->all_child_nodes);

    node->CalculateSortingPos(); // this is where the new node's sort position is actually ascertained
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
}
void VoronoiGraph::AddToChildren(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y) {

    if (this->SplitValid(branch)) { // after the split would happen, and on that point it had been allowed to do so

        this->AddToChildrenSplit(node, branch, min_x, min_y, max_x, max_y);
        branch->total_children++;

    } else { // we're either below critical mass or above it and weren't allowed to split

        NodeLinkedList::Append(node, &branch->node_children); // node given a slot
        node->SetTreeSlot(branch->node_children); // node knows its reference
        node->SetBounds(min_x, min_y, max_x, max_y); // node knows its shape // are we FOR SURE this is correct

        branch->total_children++;

        if (this->SplitValid(branch)) { // or more specifically, does it _now_ qualify for splitting

            branch->tree_children[0] = new VQuadTree((min_x+branch->half_x)/2, (min_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[1] = new VQuadTree((max_x+branch->half_x)/2, (min_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[2] = new VQuadTree((min_x+branch->half_x)/2, (max_y+branch->half_y)/2, branch->depth+1);
            branch->tree_children[3] = new VQuadTree((max_x+branch->half_x)/2, (max_y+branch->half_y)/2, branch->depth+1);

            // this branch's half x and y could theoretically be undecided up until now (where it's children would be undecided too)

            NODELINKEDLIST_FOREACH(branch->node_children, {
                VoronoiNode* repositioned_node = current_node; // get the node
                this->recent_sort_x = repositioned_node->GetSortingPosX(); // and its position (NOT recalculated)
                this->recent_sort_y = repositioned_node->GetSortingPosY();
                this->AddToChildrenSplit(repositioned_node, branch, min_x, min_y, max_x, max_y); // the nodes in each slot find their new home
                delete current_slot; // and their old slot is deleted
            });

            branch->node_children = nullptr; // at the end, everything in the list has been deleted, so the head of the list is cleared
        }
    }
}
void VoronoiGraph::AddToChildrenSplit(VoronoiNode* node, VQuadTree* branch, int min_x, int min_y, int max_x, int max_y) {
    if (this->recent_sort_y < branch->half_y) {
        if (this->recent_sort_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[0], min_x, min_y, branch->half_x, branch->half_y);
        } else {
            this->AddToChildren(node, branch->tree_children[1], branch->half_x, min_y, max_x, branch->half_y);
        }
    } else {
        if (this->recent_sort_x < branch->half_x) {
            this->AddToChildren(node, branch->tree_children[2], min_x, branch->half_y, branch->half_x, max_y);
        } else {
            this->AddToChildren(node, branch->tree_children[3], branch->half_x, branch->half_y, max_x, max_y);
        }
    }
}

void VoronoiGraph::RemoveNode(VoronoiNode* node) {
    NodeLinkedList::RemoveResidence(node, &this->all_child_nodes); // here is where the node is officially located, now gone
    // the node's sort position should not be recalculated now as that would mess with the function's ability to actually find the node
    this->recent_sort_x = node->GetSortingPosX();
    this->recent_sort_y = node->GetSortingPosY();
    this->RemoveFromChildren(node, this->root); // this, in effect, is really just about consolidating branches no longer above critical mass
}
void VoronoiGraph::RemoveFromChildren(VoronoiNode* node, VQuadTree* branch) {
    if (!this->SplitValid(branch)) { // this branch is below critical mass or is above but was disallowed splitting

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

void VoronoiGraph::UpdateNodePositions() {
    NodeLinkedList* current_slot = this->all_child_nodes;
    NodeLinkedList::Print("all nodes:", this->all_child_nodes, 0);
    while (current_slot != nullptr) {
        std::cout << "loop once" << std::endl;
        VoronoiNode* node = current_slot->node;
        // try always removing and adding every node
        current_slot = current_slot->next;
        this->RemoveNode(node); // fails on removal
        this->AddNode(node);

//        int x_copy = node->GetSortingPosX(); // store a copy of the old sorting position before the most recent move
//        int y_copy = node->GetSortingPosY();
//        node->CalculateSortingPos(); // you moved, therefore you should have an updated sorting position
//        if (!node->IsBounded()) { // now, have you left the box you were in just a moment ago?
//            std::cout << "relocating" << std::endl;
//            this->recent_sort_x = x_copy;
//            this->recent_sort_y = y_copy;
//            this->RemoveFromChildren(node, this->root); // use the old for removing it
//            this->recent_sort_x = node->GetSortingPosX();// and the new for adding it
//            this->recent_sort_y = node->GetSortingPosY();
//            this->AddToChildren(node, this->root, this->x, this->y, this->x+this->w, this->y+this->h);
//        }
//        current_slot = current_slot->next;
    }
    std::cout << "finished loop" << std::endl;
}

NodeLinkedList* VoronoiGraph::GetNearby(double x, double y, double band_width, double gain, VoronoiNode* seed) {
    this->SetGain(gain);
    this->SetBandWidth(band_width);
    return this->GetNearby(x, y, seed);
}
NodeLinkedList* VoronoiGraph::GetNearby(double x, double y, VoronoiNode* seed) {
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
        VoronoiNode* arbitrary_seed = this->all_child_nodes->node;
        arbitrary_seed->CalculateDist(this->recent_x, this->recent_y, this->gain); // work done here
        arbitrary_seed->CalculateSortingDist(this->recent_x, this->recent_y); // work done here
        this->current_bounding_mag = arbitrary_seed->GetDist() + band_width;
        this->current_box_radius = arbitrary_seed->GetSortingDist() + sort_band_width;
    }

    NodeLinkedList::DeleteList(this->nearby_candidates);
    this->nearby_candidates = nullptr;

        this->BuildNearbyList(this->root); // this is where the job is mostly done

    SDL_assert(this->nearby_candidates != nullptr);

    NodeLinkedList* result_list = nullptr; // start with an empty output list

    NodeLinkedList* nearest_slot = nullptr; // we're gonna be searching for the nearest node to swap to the front
    double nearest_dist = this->nearby_candidates->node->GetDist();

    NODELINKEDLIST_FOREACH(this->nearby_candidates, {
        double current_distance = current_node->GetDist();
        if (current_distance <= this->current_bounding_mag) { // scanned node passes the check, may have significant m value

            current_slot->next = result_list; // this candidate slot gets prepened onto result_list
            result_list = current_slot;

            if (current_distance < nearest_dist) {
                nearest_slot = current_slot;
                nearest_dist = current_distance;
            }

        } else { // node fails the check, will definitely not have significant m value
            delete current_slot;
        }
    });

    if (nearest_slot != nullptr) { // puts the nearest node at the top
        VoronoiNode* first_node = result_list->node;
        result_list->node = nearest_slot->node; // list[0] gets the nearest node
        nearest_slot->node = first_node; // list[nearest] gets the first node
    }
    return result_list;
}
void VoronoiGraph::BuildNearbyList(VQuadTree* branch) { // takes the running info on the best distance upper bound and adds the nodes descendant to this branch to the list of potential candidates
    if (!this->SplitValid(branch)) { // not split, this is a leaf node

        VoronoiNode* new_nearest = nullptr;

        NODELINKEDLIST_FOREACH(branch->node_children, {

            current_node->CalculateDist(this->recent_x, this->recent_y, this->gain);
            double relative_dist = this->current_bounding_mag-current_node->GetDist(); // from this node to the outer circle
            if (relative_dist > 0) { // beats the outer circle

                NodeLinkedList* new_candidate_entry = new NodeLinkedList(current_node);
                new_candidate_entry->next = this->nearby_candidates; // prepend the new candidate
                this->nearby_candidates = new_candidate_entry;

                if (relative_dist >= this->band_width) { // beats the middle circle
                    this->current_bounding_mag = current_node->GetDist()+this->band_width; // now the outer circle is this dist plus out by band width
                    new_nearest = current_node;
                }
            }

        });

        if (new_nearest != nullptr) { // if a candidate in this branch beat the standing record
            new_nearest->CalculateSortingDist(this->recent_x, this->recent_y); // do the work to find the box radius
            this->current_box_radius = new_nearest->GetSortingDist() + this->sort_band_width; // update the search

            this->current_bounding_mag = new_nearest->GetDist()+this->band_width;
        }

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

RGBColor VoronoiGraph::ForwardPassFromNearby(NodeLinkedList* nearby, double x, double y) { // nearby nodes already have their distances calculated, aka gain and bandwidth are baked in
    double z = 0;
    double exp_offset = -nearby->node->GetDist(); // for numerical precision. softmax doesnt care about offsets so long as theyre applied to all applicants

    RGBColor final_color = RGBColor(0,0,0);

    NODELINKEDLIST_FOREACH(nearby, {
        current_node->CalculateExp(exp_offset); // work done here
        z += current_node->GetExp();
    });

    NODELINKEDLIST_FOREACH(nearby, {
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
void VoronoiGraph::DoBackwardsPassFromNearby(NodeLinkedList* nearby, double x, double y, RGBColor image_sample) {

    RGBColor final_color = this->ForwardPassFromNearby(nearby, x, y);

    NODELINKEDLIST_FOREACH(nearby, {
        current_node->BackwardPass(x, y, final_color, (final_color-image_sample)*2);
    });
}

void VoronoiGraph::UpdateAllGradients(double learning_rate) {
    NODELINKEDLIST_FOREACH(this->all_child_nodes, {
        current_node->ApplyGradients(learning_rate);
    });
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
