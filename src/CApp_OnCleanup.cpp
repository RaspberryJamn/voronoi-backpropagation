#include "CApp.h"

void CApp::OnCleanup() {


    SDL_DestroyTexture(this->source_texture);
    this->source_texture = nullptr;

//    SDL_FreeSurface(this->Source_Surface);
//    this->Source_Surface = nullptr;

    SDL_DestroyRenderer(this->main_renderer);
    this->main_renderer = nullptr;

    // try to destroy the window as the last thing you do
    delete this->main_window;
    this->main_window = nullptr;
    // handled by SDL_DestroyWindow
//    SDL_FreeSurface(this->Display_Surface);
//    this->Display_Surface = nullptr;

    IMG_Quit();
    SDL_Quit();
}
