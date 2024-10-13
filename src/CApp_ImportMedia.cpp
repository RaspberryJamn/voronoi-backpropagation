#include "CApp.h"
#include <iostream>

SDL_Surface* LoadSurface(std::string path, SDL_PixelFormat* format) {

    SDL_Surface* optimized_surface = nullptr;

    SDL_Surface* loaded_surface = SDL_LoadBMP(path.c_str());
    if(loaded_surface == nullptr) {
        std::cout << "Source image at \"" << path.c_str() << "\" not loaded, SDL error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    optimized_surface = SDL_ConvertSurface(loaded_surface, format, 0);
    if(optimized_surface == nullptr) {
        std::cout << "Source image at \"" << path.c_str() << "\" loaded but not optimized, SDL error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_FreeSurface(loaded_surface);

    return optimized_surface;
}

SDL_Texture* LoadTexture(std::string path, SDL_Renderer* target_renderer) {

    SDL_Texture* new_texture = nullptr;

    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if(loaded_surface == nullptr) {
        std::cout << "Source image at \"" << path.c_str() << "\" not loaded, SDL_image error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    new_texture = SDL_CreateTextureFromSurface(target_renderer, loaded_surface);
    if(new_texture == nullptr) {
        std::cout << "Unable to create texture from \"" << path.c_str() << "\", SDL error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_FreeSurface(loaded_surface);

    return new_texture;
}

bool CApp::ImportMedia() {

//    this->Source_Surface = loadSurface("assets/kannabmp.bmp", this->Display_Surface->format);
//    if(this->Source_Surface == nullptr) {
//        std::cout << "Source image not loaded, SDL error: " << SDL_GetError() << std::endl;
//        return false;
//    }
    this->source_texture = LoadTexture("assets/kannapng.png", this->main_renderer);
    if(this->source_texture == nullptr) {
        std::cout << "Failed to load texture, SDL error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}
