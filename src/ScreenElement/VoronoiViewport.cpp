#include "VoronoiViewport.h"
#include "IdkFunctions.h"
#include <iostream>

double g_running_loss = 0;
double g_last_full_frame_loss = 0;
int g_render_round = 0;

namespace ScreenElement {
    VoronoiViewport::VoronoiViewport(SDL_Renderer* renderer, VoronoiGraph* graph) : ScreenElement(renderer) {
        this->graph = graph;
        this->refresh_period = 100;
        // will crop up as an issue later im sure of it
        SDL_Rect graph_shape = this->graph->GetShape();
        this->bounding_box = graph_shape;
        this->media_width = graph_shape.w;
        this->media_height = graph_shape.h;
        this->texture = new Texture(this->renderer);
        this->texture->NewBlankFromDims(this->media_width, this->media_height);

        this->render_progress.sample_x = 0;
        this->render_progress.sample_y = 0;
        this->render_progress.hits = 0;

        this->render_stats.running_loss = 0;
        this->render_stats.last_full_frame_loss = 0;
        this->render_stats.round = 0;
    }
    void VoronoiViewport::SetupChildElements() {

    }

    VoronoiViewport::~VoronoiViewport() {}

    void VoronoiViewport::HandleMouseEvent(MouseInfo mouse) {}

    void VoronoiViewport::RenderFullFrameVoronoi(double* running_loss) {
        double batch_loss = 0;
        SDL_Texture* previous_target = this->texture->SetSelfAsRenderTarget();
        for (int i = 0; i < (this->media_width*this->media_height/(1.0+this->refresh_period)); i++) {
            int x = this->render_progress.sample_x;
            int y = this->render_progress.sample_y;

            RGBColor sample = this->graph->Sample((double)x, (double)y);
            G_Clamp<double>(&sample.r, 0, 255);
            G_Clamp<double>(&sample.g, 0, 255);
            G_Clamp<double>(&sample.b, 0, 255);
            this->render_progress.hits++;

            RGBColor target_color = this->graph->SampleTrueReferenceColor(x, y);
            RGBColor diff = sample - target_color;
            batch_loss += RGBColor::Trace(diff*diff);

    //        int nearby_count = this->voronoi_graph->GetRecentNearby().size();
            RGBColor c = sample;//(target_color-sample)*127+RGBColor(127,127,127);//RGBColor(255,255,255)*(1-1.0/(nearby_count*2+1));//
            SDL_SetRenderDrawColor(this->renderer, (Uint8)c.r, (Uint8)c.g, (Uint8)c.b, 0xFF);
            SDL_RenderDrawPoint(this->renderer, x, y);

            this->render_progress.sample_x++;
            if (this->render_progress.sample_x == this->media_width) {
                this->render_progress.sample_x = 0;
                this->render_progress.sample_y++;
                if (this->render_progress.sample_y == this->media_height) {
                    this->render_progress.sample_y = 0;
                    this->render_stats.running_loss = 0;
                    this->render_progress.hits = 0;
    //                g_offset++; // finished frame
                }
            }
        }
        SDL_SetRenderTarget(this->renderer, previous_target);
        this->render_stats.running_loss += batch_loss;
    }

    bool VoronoiViewport::DrawIndividualUnder() {
        return false;
    }
    bool VoronoiViewport::DrawIndividualOver() {
        double pre_update = this->render_stats.running_loss;
        RenderFullFrameVoronoi(&this->render_stats.running_loss);
        if (this->render_stats.running_loss < pre_update) {
            this->render_stats.last_full_frame_loss = pre_update;
            this->render_stats.round++;
    //        if (this->render_stats.round == 20) {
    //            this->voronoi_graph->SetXYRate(0.0);
    //            this->voronoi_graph->SetLearningRate(this->voronoi_graph->GetLearningRate()*2.0);
    //            std::cout << "round " << this->render_stats.round << " hit, node positions frozen" << std::endl;
    //        }
    //        if (this->render_stats.round%5 == 0) {
            double new_learning_rate = this->graph->GetLearningRate()*0.98;
            this->graph->SetLearningRate(new_learning_rate);
            std::cout << this->render_stats.round << " rounds passed, learning rate now: " << new_learning_rate << std::endl;
    //        }
        }

        this->texture->Render(0,0);

        SDL_Rect dest = {0,0,0,0};
        SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0xFF);
        dest = {this->media_width, this->render_progress.sample_y, 5, 5}; SDL_RenderFillRect(this->renderer, &dest);

//        if (this->mouse.pressed) {
//            if (this->mouse.started_dragging == true) {
//                int min_x = this->mouse.x-25;
//                int min_y = this->mouse.y-25;
//                int max_x = this->mouse.x+25;
//                int max_y = this->mouse.y+25;
//                G_Clamp<int>(&min_x, 0, this->source_texture->GetWidth());
//                G_Clamp<int>(&min_y, 0, this->source_texture->GetHeight());
//                G_Clamp<int>(&max_x, 0, this->source_texture->GetWidth());
//                G_Clamp<int>(&max_y, 0, this->source_texture->GetHeight());
//                SDL_Rect transfer = {min_x,min_y,max_x-min_x,max_y-min_y};
//                this->source_texture->Render(&transfer, &transfer);
//    //
//    //            RGBColor target_color = this->SampleSourceImage(this->mouse.x,this->mouse.y);
//    //    //        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)target_color.r, (Uint8)target_color.g, (Uint8)target_color.b, 0xFF);
//    //    //        SDL_RenderFillRect(this->main_renderer, &transfer);
//    //
//    //            VoronoiNode* double_sample = nullptr;
//    //            this->voronoi_graph->Sample(this->mouse.x, this->mouse.y, &double_sample);
//    //            std::vector<VoronoiNode*> nearby = this->voronoi_graph->GetRecentNearby();
//    //            std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
//    //                current_node->ClearGradients();
//    //            });
//    //            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y  , target_color, &double_sample);
//    //
//    //            this->voronoi_graph->Poke(this->mouse.x-1, this->mouse.y  , target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y-1, target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x+1, this->mouse.y  , target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y+1, target_color, &double_sample);
//    //
//    //            this->voronoi_graph->Poke(this->mouse.x-3, this->mouse.y  , target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y-3, target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x+3, this->mouse.y  , target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x  , this->mouse.y+3, target_color, &double_sample);
//    //
//    //            this->voronoi_graph->Poke(this->mouse.x-2, this->mouse.y-2, target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x-2, this->mouse.y+2, target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x+2, this->mouse.y-2, target_color, &double_sample);
//    //            this->voronoi_graph->Poke(this->mouse.x+2, this->mouse.y+2, target_color, &double_sample);
//    ////            std::for_each(nearby.begin(), nearby.end(), [&](VoronoiNode* current_node) {
//    ////                current_node->LogGradients();
//    ////                current_node->RenderLoggedGradient(this->main_renderer);
//    ////            });
//            }
//
//            this->voronoi_graph->RenderTree(this->main_renderer, &this->number_renderer);
//        }
    //    this->voronoi_graph->RenderTree(this->main_renderer);

//        SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//        double disp;
//
//        SDL_Rect string_bounds = {5,this->media_texture->GetHeight()+5,0,0};
//        disp = this->last_frametime;
//        if (disp > 9999) {disp = 9999;}
//        this->text_textures.at(0)->RenderRTL(&string_bounds); // "Last frametime: "
//        this->number_renderer.DrawRTL(std::to_string((int)disp), &string_bounds);
//        this->text_textures.at(1)->RenderRTL(&string_bounds); // "ms"
//
//        string_bounds = {5,this->media_texture->GetHeight()+20,0,0};
//        disp = this->average_frametime;
//        this->text_textures.at(2)->RenderRTL(&string_bounds); // "Average frametime: "
//        this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
//        this->text_textures.at(1)->RenderRTL(&string_bounds); // "ms"
//
//        string_bounds = {5,this->media_texture->GetHeight()+35,0,0};
//        disp = this->render_stats.running_loss;
//        this->text_textures.at(4)->RenderRTL(&string_bounds); // "running "
//        this->text_textures.at(5)->RenderRTL(&string_bounds); // "loss "
//        this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
//
//        string_bounds = {5,this->media_texture->GetHeight()+50,0,0};
//        disp = this->render_stats.last_full_frame_loss;
//        this->text_textures.at(3)->RenderRTL(&string_bounds); // "last "
//        this->text_textures.at(5)->RenderRTL(&string_bounds); // "loss "
//        this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
//
//        string_bounds = {5,this->media_texture->GetHeight()+65,0,0};
//        disp = this->voronoi_graph->GetRecentCummLossMean()*1000.0;
//        this->text_textures.at(6)->RenderRTL(&string_bounds); // "mean "
//        this->text_textures.at(8)->RenderRTL(&string_bounds); // "node "
//        this->text_textures.at(5)->RenderRTL(&string_bounds); // "loss "
//        this->text_textures.at(10)->RenderRTL(&string_bounds); // "x100 "
//        this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);
//
//        string_bounds = {5,this->media_texture->GetHeight()+80,0,0};
//        disp = this->voronoi_graph->GetRecentCummLossVariance()*1000.0;
//        this->text_textures.at(8)->RenderRTL(&string_bounds); // "node "
//        this->text_textures.at(5)->RenderRTL(&string_bounds); // "loss "
//        this->text_textures.at(7)->RenderRTL(&string_bounds); // "variance "
//        this->text_textures.at(10)->RenderRTL(&string_bounds); // "x100 "
//        this->number_renderer.DrawRTL(std::to_string((int)(disp)), &string_bounds);

//        SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//        SDL_RenderClear(this->renderer);
//        if (this->owns_texture) {
//            this->texture->Render(this->bounding_box.x, this->bounding_box.y);
//        }
        return true;
    }
}
