#include "CApp.h"

void CApp::OnCleanup() {

    delete this->source_texture;
    this->source_texture = nullptr;
    this->source_pixels = nullptr;

    delete this->media_texture;
    this->media_texture = nullptr;

    delete this->voronoi_graph;
    this->voronoi_graph = nullptr;

    std::for_each(this->text_textures.begin(), this->text_textures.end(), [&](Texture* current_texture) {
        delete current_texture;
    });
    this->text_textures.clear();

    TTF_CloseFont(this->main_font);
    this->main_font = nullptr;

    delete this->root_screen_element;
    this->root_screen_element = nullptr;

    // try to destroy the window as the last thing you do
    delete this->main_window;
    this->main_window = nullptr;
    this->main_renderer = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
