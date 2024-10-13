#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
#include <SDL_image.h>
#include "CanvasWindow.h"

class CApp {
    private:
        bool running;

        CanvasWindow* main_window;

        SDL_Surface* display_surface;

        SDL_Renderer* main_renderer;

//        SDL_Surface* Source_Surface;
//        SDL_Surface* Media_Surfaces[];

        SDL_Texture* source_texture;

    public:
        CApp();

        int OnExecute();

    public:

        bool OnInit();
        bool ImportMedia();

        void OnEvent(SDL_Event& event);

        void OnLoop();

        void OnRender();

        void OnCleanup();
};

#endif
