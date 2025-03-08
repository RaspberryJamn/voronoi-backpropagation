#include "CApp.h"
#include <iostream>
#include <vector>
#include <string>

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
    this->source_texture->LoadFromFile("assets/teto.png");//gradient.png");
    if(this->source_texture->WasSuccessful() == false) {
        std::cout << "Failed to load texture" << std::endl;
        return false;
    }
    this->source_pixels = this->source_texture->GetPixels();

//    this->media_texture = new Texture(this->main_renderer);
//    this->media_texture->NewBlankFromDims(this->source_texture->GetWidth(), this->source_texture->GetHeight());

    this->main_font = TTF_OpenFont("assets/consola.ttf", 12);
//    std::cout << "font pointer: " << this->main_font << std::endl;
    if(this->main_font == nullptr) { // wasnt actually catching issues... "Opaque data!" is right
        std::cout << "Failed to load font, SDL_ttf error: " << TTF_GetError() << std::endl;
        return false;
    }
//    if (!this->AddStringToTextTextures("Last frametime: ")) { return false; }// 0
//    if (!this->AddStringToTextTextures("ms")) { return false; } // 1
//    if (!this->AddStringToTextTextures("Average frametime: ")) { return false; } // 2
//    if (!this->AddStringToTextTextures("last ")) { return false; } // 3
//    if (!this->AddStringToTextTextures("running ")) { return false; } // 4
//    if (!this->AddStringToTextTextures("loss ")) { return false; } // 5
//    if (!this->AddStringToTextTextures("mean ")) { return false; } // 6
//    if (!this->AddStringToTextTextures("variance ")) { return false; } // 7
//    if (!this->AddStringToTextTextures("node ")) { return false; } // 8
//    if (!this->AddStringToTextTextures("x10 ")) { return false; } // 9
//    if (!this->AddStringToTextTextures("x1000 ")) { return false; } // 10

    SDL_Color number_color = {0,0,0,255};
    this->number_renderer.BuildAtlas(this->main_renderer, this->main_font, number_color);

    return true;
}
