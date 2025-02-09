#include "CApp.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include "IdkFunctions.h"
#include "RGBColor.h"

RGBColor CApp::SampleSourceImage(int x, int y) {
    if (x < 0) { x = 0; }
    if (x >= this->source_texture->GetWidth()) { x = this->source_texture->GetWidth()-1; }
    if (y < 0) { y = 0; }
    if (y >= this->source_texture->GetHeight()) { y = this->source_texture->GetHeight()-1; }
    size_t i = (y*this->source_texture->GetWidth()+x)*4;
    return RGBColor(this->source_pixels[i],
                    this->source_pixels[i+1],
                    this->source_pixels[i+2]);
}

int g_train_sample_x = 0;
int g_train_sample_y = 0;
int g_train_hits = 0;

void CApp::OnLoop() {
//    return;
    if (this->mouse.pressed) {
        return;
    }
    this->ProgressivePoke(1.0/(1.0+this->refresh_period)*this->loop_advantage_factor);
}

void CApp::ProgressivePoke(double fraction) {
    bool debug_visualize_pokes = false;
    if (debug_visualize_pokes) {
        this->media_texture->SetSelfAsRenderTarget();
    }

    int stride = this->training_stride;
    for (int i = 0; i < (this->media_texture->GetWidth()*this->media_texture->GetHeight()*fraction); i += stride*stride) {
        int x = g_train_sample_x+std::rand()%stride-stride/2;
        int y = g_train_sample_y+std::rand()%stride-stride/2;
        G_Clamp<int>(&x, 0, this->media_texture->GetWidth()-1);
        G_Clamp<int>(&y, 0, this->media_texture->GetHeight()-1);
        if (debug_visualize_pokes) {
            SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderDrawPoint(this->main_renderer, x, y);
        }

        this->voronoi_graph->Poke((double)x, (double)y, this->SampleSourceImage(x,y));
        g_train_hits++;

        g_train_sample_x+=stride;
        if (g_train_sample_x >= this->source_texture->GetWidth()) {
            g_train_sample_x = 0;
            g_train_sample_y+=stride;
            if (g_train_sample_y >= this->source_texture->GetHeight()) {
                g_train_sample_y = 0;
//                std::cout << "printing graph" << std::endl;
//                this->voronoi_graph->PrintTree();
//                std::cout << "before update gradients in loop" << std::endl;
                this->voronoi_graph->UpdateAllGradients(10.0);
//                std::cout << "after update gradients in loop" << std::endl;
//                std::cout << "printing graph" << std::endl;
//                this->voronoi_graph->PrintTree();
                g_train_hits = 0;
            }
        }
    }

    if (debug_visualize_pokes) {
        SDL_SetRenderTarget(this->main_renderer, nullptr);
    }
}
