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

bool CApp::ImportAssets() {

//    this->Source_Surface = loadSurface("assets/kannabmp.bmp", this->Display_Surface->format);
//    if(this->Source_Surface == nullptr) {
//        std::cout << "Source image not loaded, SDL error: " << SDL_GetError() << std::endl;
//        return false;
//    }
    this->source_texture = new Texture(this->main_renderer);
    this->source_texture->LoadFromFile("assets/kannapng.png");
    if(this->source_texture->WasSuccessful() == false) {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    this->source_pixels = this->source_texture->GetPixels();

    this->media_texture = new Texture(this->main_renderer);
    this->media_texture->NewBlankFromDims(this->source_texture->GetWidth(), this->source_texture->GetHeight());

    this->main_font = TTF_OpenFont("assets/consola.ttf", 12);
//    std::cout << "font pointer: " << this->main_font << std::endl;
    if(this->main_font == nullptr) { // wasnt actually catching issues... "Opaque data!" is right
        std::cout << "Failed to load font, SDL_ttf error: " << TTF_GetError() << std::endl;
        return false;
    }

    this->text_textures[0] = new Texture(this->main_renderer, this->main_font);
    this->text_textures[0]->LoadInRenderedText("Last frametime: ", {0,0,0,255});
    if (!this->text_textures[0]->WasSuccessful()) { std::cout << "Failed to create text texture" << std::endl; return false; }

    this->text_textures[1] = new Texture(this->main_renderer, this->main_font);
    this->text_textures[1]->LoadInRenderedText("ms", {0,0,0,255});
    if (!this->text_textures[1]->WasSuccessful()) { std::cout << "Failed to create text texture" << std::endl; return false; }

    this->text_textures[2] = new Texture(this->main_renderer, this->main_font);
    this->text_textures[2]->LoadInRenderedText("Average fps: ", {0,0,0,255});
    if (!this->text_textures[2]->WasSuccessful()) { std::cout << "Failed to create text texture" << std::endl; return false; }

    this->text_textures[3] = new Texture(this->main_renderer, this->main_font);
    this->text_textures[3]->LoadInRenderedText("fps", {0,0,0,255});
    if (!this->text_textures[3]->WasSuccessful()) { std::cout << "Failed to create text texture" << std::endl; return false; }

    SDL_Color number_color = {0,0,0,255};
    this->number_renderer.BuildAtlas(this->main_renderer, this->main_font, number_color);

    return true;
}
