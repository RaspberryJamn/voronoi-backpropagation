#include "Texture.h"
#include <iostream>

Texture::Texture() {
    this->texture = nullptr;
    this->renderer = nullptr;
    this->font = nullptr;
    this->width = 0;
    this->height = 0;
    this->was_successful = true;
}
Texture::Texture(SDL_Renderer* target_renderer) {
    this->texture = nullptr;
    this->renderer = target_renderer;
    this->font = nullptr;
    this->width = 0;
    this->height = 0;
    this->was_successful = true;
}

Texture::~Texture() {
    this->FreeTexture();
    this->renderer = nullptr;
    this->font = nullptr;
}

void Texture::FreeTexture() {
    if (this->texture != nullptr) {
        SDL_DestroyTexture(this->texture);
        this->texture = nullptr;
        this->width = 0;
        this->height = 0;
    }
    this->was_successful = true;
}

void Texture::SetRenderer(SDL_Renderer* target_renderer) {
    this->renderer = target_renderer;
    this->was_successful = true;
}

void Texture::SetFont(TTF_Font* font) {
    this->font = font;
    this->was_successful = true;
}

void Texture::LoadFromFile(std::string path) {
    if (this->renderer == nullptr) {
        std::cout << "Source image at \"" << path.c_str() << "\" not converted to texture, texture has no renderer" << std::endl;
        this->was_successful = false;
        return;
    }
    this->FreeTexture();
    SDL_Texture* new_texture = nullptr;
    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    if(loaded_surface == nullptr) {
        std::cout << "Source image at \"" << path.c_str() << "\" not loaded, SDL_image error: " << IMG_GetError() << std::endl;
        this->was_successful = false;
        return;
    }
    new_texture = SDL_CreateTextureFromSurface(this->renderer, loaded_surface);
    if(new_texture == nullptr) {
        std::cout << "Unable to create texture from \"" << path.c_str() << "\", SDL error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(loaded_surface);
        this->was_successful = false;
        return;
    }
    this->texture = new_texture;
    this->width = loaded_surface->w;
    this->height = loaded_surface->h;
    SDL_FreeSurface(loaded_surface);

    this->was_successful = true;
}

void Texture::LoadInRenderedText(std::string text, SDL_Color color) {
    if (this->renderer == nullptr) {
        std::cout << "Text \"" << text.c_str() << "\" not converted to texture, texture has no associated renderer" << std::endl;
        this->was_successful = false;
        return;
    }
    if (this->font == nullptr) {
        std::cout << "Text \"" << text.c_str() << "\" not converted to texture, texture has no associated font" << std::endl;
        this->was_successful = false;
        return;
    }
    this->FreeTexture();
//    std::cout << "Got to here now" << std::endl;
//    std::cout << text << std::endl;
//    std::cout << (int)color.r<<","<<(int)color.g<<","<<(int)color.b<<","<<(int)color.a << std::endl;
//    std::cout << this->font << std::endl;

    SDL_Surface* text_surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
//    std::cout << "and here" << std::endl;
    if(text_surface == nullptr) {
        std::cout << "Unable to create surface from text \"" << text.c_str() << "\", SDL_ttf error: " << TTF_GetError() << std::endl;
        this->was_successful = false;
        return;
    }
    SDL_Texture* new_texture = nullptr;
    new_texture = SDL_CreateTextureFromSurface(this->renderer, text_surface);
    if(new_texture == nullptr) {
        std::cout << "Unable to create texture from text \"" << text.c_str() << "\", SDL error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(text_surface);
        this->was_successful = false;
        return;
    }
    this->texture = new_texture;
    this->width = text_surface->w;
    this->height = text_surface->h;
    SDL_FreeSurface(text_surface);

    this->was_successful = true;
}

void Texture::Render(int x, int y) {
    SDL_Rect render_quad = {x, y, this->width, this->height};
    SDL_RenderCopy(this->renderer, this->texture, nullptr, &render_quad);
    this->was_successful = true;
}

void Texture::Render(SDL_Rect* render_quad) {
    SDL_RenderCopy(this->renderer, this->texture, nullptr, render_quad);
    this->was_successful = true;
}

int Texture::GetWidth() {
    return this->width;
}

int Texture::GetHeight() {
    return this->height;
}

bool Texture::WasSuccessful() {
    return this->was_successful;
}
