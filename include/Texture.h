#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

class Texture {
    public:
        Texture();
        Texture(SDL_Renderer* target_renderer);
        ~Texture();

        void FreeTexture();
        SDL_Texture* OrphanTexture();

        void SetRenderer(SDL_Renderer* target_renderer);
        void SetFont(TTF_Font* font);
        void SetRendererAndFont(SDL_Renderer* target_renderer, TTF_Font* font);

        //Loads image at specified path
        void LoadFromFile(std::string path);
        //Creates image from font string
        void LoadInRenderedText(std::string texture_text, SDL_Color text_color);

        void NewBlankFromDims(int x, int y);

        void SetSelfAsRenderTarget();


//        //Set color modulation
//        void SetColor(Uint8 red, Uint8 green, Uint8 blue);
//        //Set blending
//        void SetBlendMode(SDL_BlendMode blending);
//        //Set alpha modulation
//        void SetAlpha(Uint8 alpha);

        //Renders texture at given point
        void Render(int x, int y);//, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Render(SDL_Rect* paste);//, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void Render(SDL_Rect* cut, SDL_Rect* paste);//, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        //Gets image dimensions
        int GetWidth();
        int GetHeight();
        bool WasSuccessful();

    private:
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        TTF_Font* font;

        int width;
        int height;

        bool was_successful;
};

#endif // TEXTURE_H
