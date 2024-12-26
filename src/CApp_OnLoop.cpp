#include "CApp.h"
#include <cmath>
#include <iostream>
#include <sstream>

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
VoronoiNode* g_train_past_nearest_0_0_seed = nullptr; // keeps tab on a good seed for the top left corner
VoronoiNode* g_train_past_nearest_0_y_seed = nullptr; // keeps tab on a good seed for the left hand spine of the current scan line
VoronoiNode* g_train_running_seed = nullptr; // seed for the current sample, reads from and writes to the previous two

void CApp::OnLoop() {

    int stride = 4;
    for (int i = 0; i < (this->media_texture->GetWidth()*this->media_texture->GetHeight()/(1.0+this->refresh_period)*loop_advantage_factor); i++) {
        int x = g_train_sample_x;
        int y = g_train_sample_y;

//        std::vector<VoronoiNode*> nearby = this->voronoi_graph->GetNearby((double)x, (double)y, g_train_running_seed);
//
//        this->voronoi_graph->DoBackwardsPassFromNearby(nearby, x, y, this->SampleSourceImage(x,y));
//        g_train_running_seed = nearby.front(); // new info
//
//        nearby.clear();
        this->voronoi_graph->Poke((double)x, (double)y, this->SampleSourceImage(x,y), &g_train_running_seed);

        if (g_train_sample_x <= 0) {
            g_train_past_nearest_0_y_seed = g_train_running_seed; // having hit the end of the line and slid back to the left, writing the value as step two
            if (g_train_sample_y <= 0) {
                g_train_past_nearest_0_0_seed = g_train_running_seed; // having hit the bottom of the image and slid back to the top, writing the value as step two
            }
        }

        g_train_sample_x+=stride;
        if (g_train_sample_x >= this->source_texture->GetWidth()) {
            g_train_sample_x = 0;
            g_train_running_seed = g_train_past_nearest_0_y_seed; // hit the end of the line, slide back to the left, reading the value as step one
            g_train_sample_y+=stride;
            if (g_train_sample_y >= this->source_texture->GetHeight()) {
                g_train_sample_y = 0;
                g_train_running_seed = g_train_past_nearest_0_0_seed; // hit the bottom of the image, slide back to the top, reading the value as step one

                this->voronoi_graph->UpdateAllGradients(0.00000000006);
            }
        }
    }

}
