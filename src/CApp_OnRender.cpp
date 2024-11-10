#include "CApp.h"
#include <sstream>
#include <cmath>

RGBColor CreateRGBSoftmaxFromNearby(NodeLinkedList* nearby, double x, double y) { // nearby nodes already have their distances calculated, aka gain and bandwidth are baked in

    double z = 0;
    double exp_offset = -nearby->node->GetDist(); // for numerical precision. softmax doesnt care about offsets so long as theyre applied to all applicants
    NodeLinkedList* current_slot = nearby;
    while (current_slot != nullptr) { // calculate exp of all nodes and accumulate their sum for z
        VoronoiNode* node = current_slot->node;

        node->CalculateExp(exp_offset); // work done here
        z += node->GetExp();

        current_slot = current_slot->next;
    }

    double r = 0;
    double g = 0;
    double b = 0;
    current_slot = nearby;
    while (current_slot != nullptr) {
        VoronoiNode* node = current_slot->node;

        double m = node->GetExp()/z;
        RGBColor c = node->SampleColor(x, y); // heres the only place x and y are used
        r += m*c.r*c.r;
        g += m*c.g*c.g;
        b += m*c.b*c.b;

        current_slot = current_slot->next;
    }

    r = std::sqrt(r);
    g = std::sqrt(g);
    b = std::sqrt(b);

    return RGBColor(r,g,b);
}

int g_offset = 0;
int g_sample_x = 0;
int g_sample_y = 0;
VoronoiNode* g_past_nearest_0_0_seed = nullptr; // keeps tab on a good seed for the top left corner
VoronoiNode* g_past_nearest_0_y_seed = nullptr; // keeps tab on a good seed for the left hand spine of the current scan line
VoronoiNode* g_running_seed = nullptr; // seed for the current sample, reads from and writes to the previous two
void CApp::RenderFullFrameVoronoi() {
    this->media_texture->SetSelfAsRenderTarget();

    for (int i = 0; i < (this->media_texture->GetWidth()*this->media_texture->GetHeight()); i++) {
        int x = g_sample_x;
        int y = g_sample_y;
        double band_width = 6.0;
        double gain = 0.0005;
        NodeLinkedList* nearby = this->voronoi_graph->GetNearby((double)x, (double)y, band_width, gain, g_running_seed);

        RGBColor c = CreateRGBSoftmaxFromNearby(nearby, x, y);

        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)c.r, (Uint8)c.g, (Uint8)c.b, 0xFF);
        SDL_RenderDrawPoint(this->main_renderer, x, y);

        g_running_seed = nearby->node; // new info

        NodeLinkedList::DeleteList(nearby); // no longer needed
        nearby = nullptr;
        if (g_sample_x == 0) {
            g_past_nearest_0_y_seed = g_running_seed; // having hit the end of the line and slid back to the left, writing the value as step two
            if (g_sample_y == 0) {
                g_past_nearest_0_0_seed = g_running_seed; // having hit the bottom of the image and slid back to the top, writing the value as step two
            }
        }
        g_sample_x++;
        if (g_sample_x == this->source_texture->GetWidth()) {
            g_sample_x = 0;
            g_running_seed = g_past_nearest_0_y_seed; // hit the end of the line, slide back to the left, reading the value as step one
            g_sample_y++;
            if (g_sample_y == this->source_texture->GetHeight()) {
                g_sample_y = 0;
                g_running_seed = g_past_nearest_0_0_seed; // hit the bottom of the image, slide back to the top, reading the value as step one

                g_offset++; // finished frame
            }
        }
    }

    SDL_SetRenderTarget(this->main_renderer, nullptr);
}

//int g_jitter_offset = 20;

void CApp::OnRender() {

//    g_jitter_offset += 1;
//    if (g_jitter_offset > 400) {
//        g_jitter_offset = 0;
//    }
//
//    SDL_Rect stretch_rect;
//    stretch_rect.x = g_jitter_offset;
//    stretch_rect.y = g_jitter_offset;
//    stretch_rect.w = 400;
//    stretch_rect.h = 400;
////    SDL_RenderClear(this->main_renderer);
//
//    this->source_texture->Render(&stretch_rect);
    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->main_renderer);

    this->refresh_counter++;
    if (this->refresh_counter > this->refresh_period) {
        this->refresh_counter = 0;
        this->RenderFullFrameVoronoi();
    }

    this->media_texture->Render(0,0);

    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_Rect string_bounds = {5,5,0,0};
    this->text_textures[0]->RenderRTL(&string_bounds); // "Last frametime: "
    this->number_renderer.DrawRTL(std::to_string(this->last_frametime), &string_bounds);
    this->text_textures[1]->RenderRTL(&string_bounds); // "ms"

    string_bounds = {5,20,0,0};
    this->text_textures[2]->RenderRTL(&string_bounds); // "Last frametime: "
    this->number_renderer.DrawRTL(std::to_string((int)(1000.0/this->average_frametime)), &string_bounds);
    this->text_textures[3]->RenderRTL(&string_bounds); // "ms"

    SDL_RenderPresent(this->main_renderer);

}
