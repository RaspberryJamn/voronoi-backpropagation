#include "CApp.h"
#include <iostream>

bool CApp::OnInit() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL video not initialized, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)){
        std::cout << "SDL image not initialized, SDL_image error: " << IMG_GetError() << std::endl;
        return false;
    }

    if(TTF_Init() == -1){
        std::cout << "SDL ttf not initialized, SDL_ttf error: " << TTF_GetError() << std::endl;
        return false;
    }

    this->main_window = new CanvasWindow();
    this->main_window->Init("sdl2 window",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            800,
                            600,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (this->main_window == nullptr) {
        std::cout << "Window not created, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    this->main_renderer = this->main_window->CreateRenderer();
    if (this->main_renderer == nullptr) {
        std::cout << "Main window renderer not created, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(this->main_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

//    SDL_RendererInfo info;
//    SDL_GetRendererInfo(this->main_renderer, &info);
//    std::cout << "renderer info: " << info.flags << std::endl;
//    return false;

    if (this->ImportAssets() == false) {
        std::cout << "Failed to load assets" << std::endl;
        return false;
    }
    this->text_texture->SetRendererAndFont(this->main_renderer, this->main_font);

    return true;
}
