#include "CApp.h"

void CApp::OnCleanup() {

    delete this->source_texture;
    this->source_texture = nullptr;
    this->source_pixels = nullptr;

    delete this->media_texture;
    this->media_texture = nullptr;

    delete this->voronoi_graph;
    this->voronoi_graph = nullptr;

    for (int i = 0; i < 7; i++) { // hardcoded but whatever man
        delete this->text_textures[i];
        this->text_textures[i] = nullptr;
    }

    TTF_CloseFont(this->main_font);
    this->main_font = nullptr;

    // try to destroy the window as the last thing you do
    delete this->main_window;
    this->main_window = nullptr;
    this->main_renderer = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
