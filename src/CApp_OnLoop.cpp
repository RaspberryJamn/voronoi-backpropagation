#include "CApp.h"

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

        double band_width = 10.0;
        double gain = 0.1;
//        std::cout << g_historic_nearest_node << " ";
        NodeLinkedList* nearby = this->voronoi_graph->GetNearby((double)x, (double)y, band_width, gain, g_historic_nearest_node);
//        std::cout << nearby->node << std::endl;
        double r = 0;
        double g = 0;
        double b = 0;
//        if (nearby->node == nullptr) {
//            std::cout << "erm" << std::endl;
//        }
//        if (nearby->node != nullptr) {
//            g_historic_nearest_node = nearby->node;
//            RGBColor c = nearby->node->SampleColor(x,y);
//            r += c.r;
//            g += c.g;
//            b += c.b;
////            double z = 0;
////            double exp_offset = -nearby->node->GetDist(); // for numerical precision
////            NodeLinkedList* current_list_entry = nearby;
////            while (current_list_entry != nullptr) {
////                NodeLinkedList* next_list_entry = current_list_entry->next;
////                VoronoiNode* node = current_list_entry->node;
////
////                node->UpdateExp(gain, exp_offset);
////                z += node->GetExp();
////
////                current_list_entry = next_list_entry;
////            }
////            current_list_entry = nearby;
////            while (current_list_entry != nullptr) {
////                NodeLinkedList* next_list_entry = current_list_entry->next;
////                VoronoiNode* node = current_list_entry->node;
////
////                double m = node->GetExp()/z;
////                RGBColor c = node->SampleColor(x,y);
////                r += m*c.r;
////                g += m*c.g;
////                b += m*c.b;
////
////                current_list_entry = next_list_entry;
////            }
////            current_list_entry = nearby;
////            while (current_list_entry != nullptr) {
////                NodeLinkedList* next_list_entry = current_list_entry->next;
////                delete current_list_entry;
////                current_list_entry = next_list_entry;
////            }
////            nearby = nullptr;
//        } else {
////            std::cout << "eefefrm" << std::endl;
//        }
//        std::cout << "2 " << nearby << std::endl;
        SDL_SetRenderDrawColor(this->main_renderer, (Uint8)r, (Uint8)g, (Uint8)b, 0xFF);
        SDL_RenderDrawPoint(this->main_renderer, x, y);

    }

    SDL_SetRenderTarget(this->main_renderer, nullptr);
}
