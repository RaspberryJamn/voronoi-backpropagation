#include "CApp.h"

CApp::CApp() {
    this->running = true;

    this->main_window = nullptr;
    this->main_renderer = nullptr;
    this->root_screen_element = nullptr;

    this->last_frametime = 0;
    this->average_frametime = 0;

    this->source_texture = nullptr;
    this->source_pixels = nullptr;

//    this->media_texture = nullptr;
    this->voronoi_graph = nullptr;

    this->text_textures.clear();
//    AtlasNumberDrawer this->number_renderer;
    this->main_font = nullptr;
    this->refresh_period = 0;
    this->loop_advantage_factor = 0;
    this->mouse = {};
    this->last_keypress = SDLK_ESCAPE;
    this->training_stride = 4;

}

int CApp::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event event;
    Timer frametime_timer;
    LapCounter frametime_counter;
    frametime_counter.Reset();
    while(this->running) {
        frametime_timer.Reset();

        while(SDL_PollEvent(&event)) {
            OnEvent(event);
        }
        OnLoop();
        OnRender();

        this->last_frametime = frametime_timer.QueryTicks();
        frametime_counter.CallLap();
        this->average_frametime = frametime_counter.GetAverageLapTime();
    }

    OnCleanup();
    return 0;
}

int main(int argc, char* argv[]) {
    CApp the_app;

    return the_app.OnExecute();
}
