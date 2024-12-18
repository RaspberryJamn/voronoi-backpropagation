#include "CApp.h"

CApp::CApp() {
    this->running = true;

    this->main_window = nullptr;
    this->main_renderer = nullptr;

    this->last_frametime = 0;
    this->average_frametime = 0;

    this->source_texture = nullptr;
    this->source_pixels = nullptr;

    this->media_texture = nullptr;
    this->voronoi_graph = nullptr;

    this->text_textures[0] = nullptr;
    this->text_textures[1] = nullptr;
    this->text_textures[2] = nullptr;
    this->text_textures[3] = nullptr;
//    AtlasNumberDrawer this->number_renderer;
    this->main_font = nullptr;
    this->refresh_period = 0;
    this->loop_advantage_factor = 0;
    this->error_logger = new ErrorLogger();
    this->error_logger->DisableLogging();

}

int CApp::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    Timer frametime_timer;
    LapCounter frametime_counter;
    frametime_counter.Reset();
    while(this->running) {
        frametime_timer.Reset();

        while(SDL_PollEvent(&Event)) {
            OnEvent(Event);
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
