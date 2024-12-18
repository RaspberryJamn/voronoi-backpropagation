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

// node1.mag = (node1.x*node1.x+node1.y*node1.y)
// e1 = exp(node1.mag)
// z = e1+e2+e3
// m1 = e1/z
// final_color = m1*node1.color+m2*node2.color+m3*node3.color
  // final_color = (m1*node1.color^2+m2*node2.color^2+m3*node3.color^2)^.5
// loss = (final_color-desired_color)^2

// dnode1.mag/dnode1.x = node1.x
// dm1/dnode1.mag = m1*(1-m1)
// dfinal_color/dm1 = node1.color
  // dfinal_color/dm1 = .5*(node1.color^2)/final_color
// dnode1.color/dnode1.x = something
// dfinal_color/dnode1.color = m1
  // dfinal_color/dnode1.color = m1/final_color*(node1.color)
// dloss/dfinal_color = 2(final_color-desired_color)

// x_grad += dloss/dfinal_color * output_color * m*(1-m) * x
  // x_grad += dloss/dfinal_color * .5*(node1.color^2)/final_color * m*(1-m) * x

//int g_offset = 0;
int g_train_sample_x = 0;
int g_train_sample_y = 0;
VoronoiNode* g_train_past_nearest_0_0_seed = nullptr; // keeps tab on a good seed for the top left corner
VoronoiNode* g_train_past_nearest_0_y_seed = nullptr; // keeps tab on a good seed for the left hand spine of the current scan line
VoronoiNode* g_train_running_seed = nullptr; // seed for the current sample, reads from and writes to the previous two

//#ifndef LOGGING_GLOBALS
//#define LOGGING_GLOBALS 0
//bool g_start_logging_after_gradients_updated = false;
//int g_stop_logging_after_100_logs = 0;
//#endif // LOGGING_GLOBALS

void CApp::OnLoop() {

    for (int i = 0; i < (this->media_texture->GetWidth()*this->media_texture->GetHeight()/(1.0+this->refresh_period)*loop_advantage_factor); i++) {
        int x = g_train_sample_x;
        int y = g_train_sample_y;

        std::vector<VoronoiNode*> nearby = this->voronoi_graph->GetNearby((double)x, (double)y, g_train_running_seed);

        this->voronoi_graph->DoBackwardsPassFromNearby(nearby, x, y, this->SampleSourceImage(x,y));

        g_train_running_seed = nearby.front(); // new info

        nearby.clear();

        if (g_train_sample_x <= 0) {
            g_train_past_nearest_0_y_seed = g_train_running_seed; // having hit the end of the line and slid back to the left, writing the value as step two
            if (g_train_sample_y <= 0) {
                g_train_past_nearest_0_0_seed = g_train_running_seed; // having hit the bottom of the image and slid back to the top, writing the value as step two
            }
        }

        g_train_sample_x+=2;
        if (g_train_sample_x >= this->source_texture->GetWidth()) {
            g_train_sample_x = 0;
            g_train_running_seed = g_train_past_nearest_0_y_seed; // hit the end of the line, slide back to the left, reading the value as step one
            g_train_sample_y+=2;
            if (g_train_sample_y >= this->source_texture->GetHeight()) {
                g_train_sample_y = 0;
                g_train_running_seed = g_train_past_nearest_0_0_seed; // hit the bottom of the image, slide back to the top, reading the value as step one

                this->voronoi_graph->UpdateAllGradients(0.00000003);
            }
        }
    }

}
