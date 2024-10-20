#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "CanvasWindow.h"
#include "Timer.h"
#include "Texture.h"
#include "AtlasNumberDrawer.h"

class CApp {
    private:
        bool running;

        CanvasWindow* main_window;
        SDL_Renderer* main_renderer;

        Uint32 last_frametime;

//        SDL_Surface* Source_Surface;
//        SDL_Surface* Media_Surfaces[];

        Texture* source_texture;

        Texture* text_texture;
        Texture* text_textures[2];
        AtlasNumberDrawer number_renderer;

        TTF_Font* main_font;

    public:
        CApp();

        int OnExecute();

    public:

        bool OnInit();
        bool ImportAssets();

        void OnEvent(SDL_Event& event);

        void OnLoop();

        void OnRender();

        void OnCleanup();
};

#endif
