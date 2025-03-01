#include "Texture.h"
#include <iostream>
#include "RGBColor.h"

Texture::Texture() {
    this->Init(nullptr, nullptr);
}
Texture::Texture(SDL_Renderer* target_renderer) {
    this->Init(target_renderer, nullptr);
}
Texture::Texture(SDL_Renderer* target_renderer, TTF_Font* font) {
    this->Init(target_renderer, font);
}
void Texture::Init(SDL_Renderer* target_renderer, TTF_Font* font) {
    this->surface = nullptr;
    this->texture = nullptr;
    this->renderer = target_renderer;
    this->font = font;
    this->width = 0;
    this->height = 0;
    this->was_successful = true;
}

Texture::~Texture() {
    this->FreeSurface();
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
void Texture::FreeSurface() {
    if (this->surface != nullptr) {
        SDL_FreeSurface(this->surface);
        this->surface = nullptr;
    }
    this->was_successful = true;
}

SDL_Texture* Texture::OrphanTexture() {
    SDL_Texture* result = this->texture;
    this->texture = nullptr;
    this->width = 0;
    this->height = 0;
    this->was_successful = true;
    return result;
}

void Texture::SetRenderer(SDL_Renderer* target_renderer) {
    this->renderer = target_renderer;
    this->was_successful = true;
}
void Texture::SetFont(TTF_Font* font) {
    this->font = font;
    this->was_successful = true;
}
void Texture::SetRendererAndFont(SDL_Renderer* target_renderer, TTF_Font* font) {
    this->renderer = target_renderer;
    this->font = font;
    this->was_successful = true;
}

void Texture::LoadFromFile(std::string path) {
    if (this->renderer == nullptr) {
        std::cout << "Source image at \"" << path.c_str() << "\" not converted to texture, texture has no renderer" << std::endl;
        this->was_successful = false;
        return;
    }
    this->FreeSurface();
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
    SDL_ConvertSurfaceFormat(loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0);
    this->surface = loaded_surface;
    this->texture = new_texture;
    this->width = loaded_surface->w;
    this->height = loaded_surface->h;

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
    this->FreeSurface();
    this->FreeTexture();

    SDL_Surface* text_surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
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
    SDL_ConvertSurfaceFormat(text_surface, SDL_PIXELFORMAT_RGBA8888, 0);
    this->surface = text_surface;
    this->texture = new_texture;
    this->width = text_surface->w;
    this->height = text_surface->h;

    this->was_successful = true;
}
void Texture::NewBlankFromDims(int x, int y) {
    if (this->renderer == nullptr) {
        std::cout << "Texture has no renderer, unable to create blank texture from dims \"" << x<<","<<y << std::endl;
        this->was_successful = false;
        return;
    }
    this->FreeSurface();
    this->FreeTexture();
    SDL_Texture* new_texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, x, y);
    if(new_texture == nullptr) {
        std::cout << "Unable to create blank texture from dims \"" << x<<","<<y << "\", SDL error: " << SDL_GetError() << std::endl;
        this->was_successful = false;
        return;
    }
    this->surface = nullptr;
    this->texture = new_texture;
    this->width = x;
    this->height = y;

    this->was_successful = true;
}

void Texture::SetSelfAsRenderTarget() {
    SDL_SetRenderTarget(this->renderer, this->texture);
    this->was_successful = true;
}

void Texture::Render(int x, int y) {
    SDL_Rect render_quad = {x, y, this->width, this->height};
    SDL_RenderCopy(this->renderer, this->texture, nullptr, &render_quad);
    this->was_successful = true;
}
void Texture::Render(SDL_Rect* paste) {
    SDL_RenderCopy(this->renderer, this->texture, nullptr, paste);
    this->was_successful = true;
}
void Texture::Render(SDL_Rect* cut, SDL_Rect* paste) {
    SDL_RenderCopy(this->renderer, this->texture, cut, paste);
    this->was_successful = true;
}
void Texture::RenderRTL(SDL_Rect* inout_bounds) {
    SDL_Rect bounds = *inout_bounds;
    SDL_Rect paste = {bounds.x+bounds.w,bounds.y,this->width,this->height};
    SDL_RenderCopy(this->renderer, this->texture, nullptr, &paste);
    inout_bounds->w = bounds.w+this->width;
    this->was_successful = true;
}

Uint8* Texture::GetPixels() {
    if(this->surface != nullptr) {
        this->was_successful = true;
        return (Uint8*)(this->surface->pixels);
    }
    this->was_successful = false;
    return nullptr;
}

RGBColor Texture::SampleColor(int xi, int yi) {
    int x = xi;
    int y = yi;
    if (x < 0) { x = 0; }
    if (x >= this->width) { x = this->width-1; }
    if (y < 0) { y = 0; }
    if (y >= this->height) { y = this->height-1; }
    size_t i = (y*this->width+x)*4;
    Uint8* pixels = this->GetPixels();
    return RGBColor(pixels[i],
                    pixels[i+1],
                    pixels[i+2]);
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
