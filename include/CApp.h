#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "CanvasWindow.h"
#include "Timer.h"
#include "Texture.h"
#include "AtlasNumberDrawer.h"
#include "LapCounter.h"
#include "VoronoiGraph.h"
#include "VoronoiNode.h"

//class VoronoiGraph; // fuckass

class CApp {
    private:
        bool running;


        CanvasWindow* main_window;
        SDL_Renderer* main_renderer;

        Uint32 last_frametime;
        double average_frametime;

        Texture* source_texture;
        Uint8* source_pixels;

        Texture* media_texture;
        VoronoiGraph* voronoi_graph;

        Texture* text_textures[4];
        AtlasNumberDrawer number_renderer;

        TTF_Font* main_font;

        ErrorLogger* error_logger;

    public:
        CApp();

        int OnExecute();

    public:

//        bool g_start_logging_after_gradients_updated;
//        int g_stop_logging_after_100_logs;

        bool OnInit();
        bool ImportAssets();

        void OnEvent(SDL_Event& event);

        void OnLoop();

        void OnRender();
        void RenderFullFrameVoronoi();
        RGBColor SampleSourceImage(int x, int y);
        int refresh_period; // 0 means redraw graph every frame, 1 every other, etc.
        int refresh_counter;

        void OnCleanup();
};

#endif
