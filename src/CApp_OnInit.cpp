#include "CApp.h"
#include <iostream>
#include "CanvasWindow.h"

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

//    this->Display_Surface = SDL_GetWindowSurface(this->Main_Window);
//    if (this->Display_Surface == nullptr) { // can this even happen?
//        std::cout << "Window surface not derived, SDL error: " << SDL_GetError() << std::endl;
//        return false;
//    }

    this->main_renderer = this->main_window->CreateRenderer();
    if (this->main_renderer == nullptr) {
        std::cout << "Main window renderer not created, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

//    SDL_RendererInfo info;
//    SDL_GetRendererInfo(this->main_renderer, &info);
//    std::cout << "renderer info: " << info.flags << std::endl;
//    return false;

    if (this->ImportMedia() == false) {
        std::cout << "Failed to load media" << std::endl;
        return false;
    }

    return true;
}
