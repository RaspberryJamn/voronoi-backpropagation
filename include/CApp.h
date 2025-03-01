#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "IdkFunctions.h"
#include "CanvasWindow.h"
#include "Timer.h"
#include "Texture.h"
#include "AtlasNumberDrawer.h"
#include "LapCounter.h"
#include "VoronoiGraph.h"
#include "VoronoiNode.h"
#include "MouseInfo.h"
#include "ScreenElement.h"

class CApp {
    public:
        CApp();

        int OnExecute();

        bool OnInit();

        void OnEvent(SDL_Event& event);

        void OnLoop();

        void OnRender();

        void OnCleanup();

    private:
        bool running;


        CanvasWindow* main_window;
        SDL_Renderer* main_renderer;
        ScreenElement::ScreenElement* root_screen_element;

        Uint32 last_frametime;
        double average_frametime;

        Texture* source_texture;
        Uint8* source_pixels;

//        Texture* media_texture;
        VoronoiGraph* voronoi_graph;

        std::vector<Texture*> text_textures;
        AtlasNumberDrawer number_renderer;

        TTF_Font* main_font;

        MouseInfo mouse;

        SDL_Keycode last_keypress;

    private:
        // OnInit
        bool ImportAssets();
        bool AddStringToTextTextures(std::string string);

        // OnEvent
        void OnMouseDown();
        void OnMouseUp();
        void OnMouseMoved();
        void OnMouseDragged();

        void OnKeyDown(SDL_Keycode key);

        // OnLoop
        void ProgressivePoke(double fraction);
        int training_stride;

        // OnRender
        void RenderFullFrameVoronoi(double* total_loss);
        RGBColor SampleSourceImage(int x, int y);
        int refresh_period; // 0 means redraw graph every frame, 1 every other, etc.
        double loop_advantage_factor;
};

#endif
