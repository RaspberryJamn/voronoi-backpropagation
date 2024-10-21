#include "CApp.h"

void CApp::OnCleanup() {


    delete this->source_texture;
    this->source_texture = nullptr;

    // try to destroy the window as the last thing you do
    delete this->main_window;
    this->main_window = nullptr;
    this->main_renderer = nullptr;

    TTF_CloseFont(this->main_font);
    this->main_font = nullptr;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
