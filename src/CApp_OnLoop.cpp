#include "CApp.h"
#include <cmath>

int g_offset = 0;
int g_sample_x = 0;
int g_sample_x_dir = 1;
int g_sample_y = 0;
int g_sample_y_dir = 1;
VoronoiNode* g_historic_nearest_node = nullptr;
void CApp::OnLoop() {

    this->media_texture->SetSelfAsRenderTarget();

    for (int i = 0; i < 100; i++) {

        g_sample_x += g_sample_x_dir;
        if ((g_sample_x < 0) || (g_sample_x == source_texture->GetWidth())) {
            g_sample_x -= g_sample_x_dir;
            g_sample_x_dir *= -1;
            g_sample_y += g_sample_y_dir;
            if ((g_sample_y < 0) || (g_sample_y == source_texture->GetHeight())) {
                g_sample_y -= g_sample_y_dir;
                g_sample_y_dir *= -1;

                g_offset++; // finished frame
            }
        }
        int x = g_sample_x;
        int y = g_sample_y;

        double band_width = 6.0;
        double gain = 0.0005;
        NodeLinkedList* nearby = this->voronoi_graph->GetNearby((double)x, (double)y, band_width, gain, g_historic_nearest_node);//nullptr);//
        double r = 0;
        double g = 0;
        double b = 0;

        int len = NodeLinkedList::Length(nearby);
        r = 255*(1-1/((double)len+1));
        g = r;
        b = r;

        g_historic_nearest_node = nearby->node;

        double z = 0;
        double exp_offset = 0;//-nearby->node->GetDist(); // for numerical precision // apparently not?
        NodeLinkedList* current_slot = nearby;
        while (current_slot != nullptr) {
            NodeLinkedList* next_slot = current_slot->next;
            VoronoiNode* node = current_slot->node;

            node->CalculateExp(exp_offset);
            z += node->GetExp();

            current_slot = next_slot;
        }
        current_slot = nearby;
        while (current_slot != nullptr) {
            NodeLinkedList* next_slot = current_slot->next;
            VoronoiNode* node = current_slot->node;

            double m = node->GetExp()/z;
            RGBColor c = node->SampleColor(x,y);
            r += m*c.r*c.r;
            g += m*c.g*c.g;
            b += m*c.b*c.b;

            current_slot = next_slot;
        }
        NodeLinkedList::DeleteList(nearby);
        nearby = nullptr;
        r = std::sqrt(r);
        g = std::sqrt(g);
        b = std::sqrt(b);

        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)r, (Uint8)g, (Uint8)b, 0xFF);
        SDL_RenderDrawPoint(this->main_renderer, x, y);

    }

    SDL_SetRenderTarget(this->main_renderer, nullptr);
}
