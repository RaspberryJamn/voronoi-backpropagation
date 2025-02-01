#include "CApp.h"
#include "IdkFunctions.h"
#include <sstream>
#include <cmath>
#include <algorithm>

int g_sample_x = 0;
int g_sample_y = 0;
VoronoiNode* g_past_nearest_0_0_seed = nullptr; // keeps tab on a good seed for the top left corner
VoronoiNode* g_past_nearest_0_y_seed = nullptr; // keeps tab on a good seed for the left hand spine of the current scan line
VoronoiNode* g_running_seed = nullptr; // seed for the current sample, reads from and writes to the previous two

VoronoiNode* g_add_remove_node = nullptr;

int g_render_hits = 0;
void CApp::RenderFullFrameVoronoi(double* running_loss) {

    bool calculate_loss = (running_loss != nullptr);
    double batch_loss = 0;

    this->media_texture->SetSelfAsRenderTarget();

    for (int i = 0; i < (this->media_texture->GetWidth()*this->media_texture->GetHeight()/(1.0+this->refresh_period)); i++) {
        int x = g_sample_x;
        int y = g_sample_y;

        RGBColor sample = this->voronoi_graph->Sample((double)x, (double)y, &g_running_seed);
        G_Clamp<double>(&sample.r, 0, 255);
        G_Clamp<double>(&sample.g, 0, 255);
        G_Clamp<double>(&sample.b, 0, 255);
        g_render_hits++;

        if (calculate_loss == true) {
            RGBColor target_color = this->SampleSourceImage(x,y);
            RGBColor diff = sample - target_color;
            batch_loss += RGBColor::Trace(diff*diff);
        }

        RGBColor c = sample;//(target_color-sample)*127+RGBColor(127,127,127);
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

                if (calculate_loss) {
                    (*running_loss) = 0;
                }
                g_render_hits = 0;
//                g_offset++; // finished frame
            }
        }
    }

    SDL_SetRenderTarget(this->main_renderer, nullptr);

    if (calculate_loss) {
        (*running_loss) += batch_loss;
    }
}

double g_running_loss = 0;
double g_last_full_frame_loss = 0;
void CApp::OnRender() {

    SDL_Rect dest = {0,0,0,0};

    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(this->main_renderer);

    double pre_update = g_running_loss;
    this->RenderFullFrameVoronoi(&g_running_loss);
    if (g_running_loss < pre_update) {
        g_last_full_frame_loss = pre_update;
    }

    this->media_texture->Render(0,0);
    SDL_SetRenderDrawColor(this->main_renderer, 0x00, 0x00, 0x00, 0xFF);
    dest = {this->source_texture->GetWidth(), g_sample_y, 5, 5}; SDL_RenderFillRect(this->main_renderer, &dest);

    if (this->mouse.pressed) {
        if (this->mouse.started_dragging == true) {
            int min_x = this->mouse.x-25;
            int min_y = this->mouse.y-25;
            int max_x = this->mouse.x+25;
            int max_y = this->mouse.y+25;
            G_Clamp<int>(&min_x, 0, this->source_texture->GetWidth());
            G_Clamp<int>(&min_y, 0, this->source_texture->GetHeight());
            G_Clamp<int>(&max_x, 0, this->source_texture->GetWidth());
            G_Clamp<int>(&max_y, 0, this->source_texture->GetHeight());
            SDL_Rect transfer = {min_x,min_y,max_x-min_x,max_y-min_y};
            this->source_texture->Render(&transfer, &transfer);
//
//            RGBColor target_color = this->SampleSourceImage(this->mouse.x,this->mouse.y);
//    //        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)target_color.r, (Uint8)target_color.g, (Uint8)target_color.b, 0xFF);
//    //        SDL_RenderFillRect(this->main_renderer, &transfer);
//
//            VoronoiNode* double_sample = nullptr;
//            this->voronoi_graph->Sample(this->mouse.x, this->mouse.y, &double_sample);
//            std::vector<VoronoiNode*> nearby = this->voronoi_graph->GetRecentNearby();
//            std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
//                current_node->ClearGradients();
//            });
//            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y  , target_color, &double_sample);
//
//            this->voronoi_graph->Poke(this->mouse.x-1, this->mouse.y  , target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y-1, target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x+1, this->mouse.y  , target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y+1, target_color, &double_sample);
//
//            this->voronoi_graph->Poke(this->mouse.x-3, this->mouse.y  , target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y-3, target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x+3, this->mouse.y  , target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y+3, target_color, &double_sample);
//
//            this->voronoi_graph->Poke(this->mouse.x-2, this->mouse.y-2, target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x-2, this->mouse.y+2, target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x+2, this->mouse.y-2, target_color, &double_sample);
//            this->voronoi_graph->Poke(this->mouse.x+2, this->mouse.y+2, target_color, &double_sample);
////            std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
////                current_node->LogGradients();
////                current_node->RenderLoggedGradient(this->main_renderer);
////            });
        }

        this->voronoi_graph->RenderTree(this->main_renderer);
    }
//    this->voronoi_graph->RenderTree(this->main_renderer);

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

    string_bounds = {5,this->media_texture->GetHeight()+35,0,0};
    disp = g_running_loss;
    this->text_textures[5]->RenderRTL(&string_bounds); // "running "
    this->text_textures[6]->RenderRTL(&string_bounds); // "loss "
    this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);

    string_bounds = {5,this->media_texture->GetHeight()+50,0,0};
    disp = g_last_full_frame_loss;
    this->text_textures[4]->RenderRTL(&string_bounds); // "last "
    this->text_textures[6]->RenderRTL(&string_bounds); // "loss "
    this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);

//    string_bounds = {5,35,0,0};
//    disp = 1000.0/this->average_full_frametime;
////    if (disp < (1000.0/9999.0)) {disp = (1000.0/9999.0);}
////    disp = (1000.0/disp);
//    this->text_textures[4]->RenderRTL(&string_bounds); // "Average fps: "
//    this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
//    this->text_textures[5]->RenderRTL(&string_bounds); // "fps"

    SDL_RenderPresent(this->main_renderer);

}
