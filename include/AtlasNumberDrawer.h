#ifndef ATLASNUMBERDRAWER_H
#define ATLASNUMBERDRAWER_H

#include "Texture.h"
#include "SDL.h"

class AtlasNumberDrawer
{
    public:
        AtlasNumberDrawer();
        ~AtlasNumberDrawer();

        void BuildAtlas(SDL_Renderer* target_renderer, TTF_Font* font, SDL_Color text_color);

        void DrawRTL(SDL_Renderer* target_renderer, std::string text, SDL_Rect* inout_bounds);

    private:
        Texture* atlas;
        SDL_Rect number_locations[10];

        SDL_Renderer* renderer;
};

#endif // ATLASNUMBERDRAWER_H
