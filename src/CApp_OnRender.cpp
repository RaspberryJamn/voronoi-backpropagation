#include "CApp.h"
#include <sstream>
#include <cmath>

int g_sample_x = 0;
int g_sample_y = 0;
VoronoiNode* g_past_nearest_0_0_seed = nullptr; // keeps tab on a good seed for the top left corner
VoronoiNode* g_past_nearest_0_y_seed = nullptr; // keeps tab on a good seed for the left hand spine of the current scan line
VoronoiNode* g_running_seed = nullptr; // seed for the current sample, reads from and writes to the previous two

VoronoiNode* g_add_remove_node = nullptr;

void CApp::RenderFullFrameVoronoi() {
    this->media_texture->SetSelfAsRenderTarget();

    for (int i = 0; i < (this->media_texture->GetWidth()*this->media_texture->GetHeight()/(1.0+this->refresh_period)); i++) {
        int x = g_sample_x;
        int y = g_sample_y;

        RGBColor c = this->voronoi_graph->Sample((double)x, (double)y, &g_running_seed);
        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)c.r, (Uint8)c.g, (Uint8)c.b, 0xFF);
        SDL_RenderDrawPoint(this->main_renderer, x, y);

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

//                g_offset++; // finished frame
            }
        }
    }

    SDL_SetRenderTarget(this->main_renderer, nullptr);
}

void CApp::OnRender() {
    SDL_Rect dest = {0,0,0,0};

    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->main_renderer);

    this->RenderFullFrameVoronoi(); // not so "full frame" anymore

    this->media_texture->Render(0,0);
    SDL_SetRenderDrawColor(this->main_renderer, 0x00, 0x00, 0x00, 0xFF);
    dest = {this->source_texture->GetWidth(), g_sample_y, 5, 5}; SDL_RenderFillRect(this->main_renderer, &dest);

    if (this->mouse.pressed) {
        this->voronoi_graph->RenderTree(this->main_renderer);
    }

    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    double disp;

    SDL_Rect string_bounds = {5,this->media_texture->GetHeight()+5,0,0};
    disp = this->last_frametime;
    if (disp > 9999) {disp = 9999;}
    this->text_textures[0]->RenderRTL(&string_bounds); // "Last frametime: "
    this->number_renderer.DrawRTL(std::to_string((int)disp), &string_bounds);
    this->text_textures[1]->RenderRTL(&string_bounds); // "ms"

    string_bounds = {5,this->media_texture->GetHeight()+20,0,0};
    disp = this->average_frametime;
    this->text_textures[2]->RenderRTL(&string_bounds); // "Average frametime: "
    this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
    this->text_textures[3]->RenderRTL(&string_bounds); // "ms"

//    string_bounds = {5,35,0,0};
//    disp = 1000.0/this->average_full_frametime;
////    if (disp < (1000.0/9999.0)) {disp = (1000.0/9999.0);}
////    disp = (1000.0/disp);
//    this->text_textures[4]->RenderRTL(&string_bounds); // "Average fps: "
//    this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
//    this->text_textures[5]->RenderRTL(&string_bounds); // "fps"

    SDL_RenderPresent(this->main_renderer);

}
