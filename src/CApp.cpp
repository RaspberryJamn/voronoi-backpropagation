#include "CApp.h"

CApp::CApp() {
    this->running = true;

    this->main_window = nullptr;
    this->main_renderer = nullptr;

    this->last_frametime = 0;
    this->average_frametime = 0;
    this->target_frametime = 1000.0/(60); // 60fps
    this->average_full_frametime = 0;

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
    this->refresh_counter = 0;
    this->error_logger = new ErrorLogger();
    this->error_logger->DisableLogging();

//    this->g_start_logging_after_gradients_updated = false;
//    this->g_stop_logging_after_100_logs = 0;
}

int CApp::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    Timer frametime_timer;
    LapCounter frametime_counter;
    LapCounter fps_counter;
    int offset = 0;
    frametime_counter.Reset();
    fps_counter.Reset();
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

        fps_counter.CallLap();
        this->average_full_frametime = fps_counter.GetAverageLapTime();

        frametime_counter.Pause();
        if (this->last_frametime < this->target_frametime) {
            double drift = this->average_full_frametime - this->target_frametime;
            if (drift < -0.5) { // we're running fast
                offset++;
            }
            if (drift > 0.5) { // we're running slow
                offset--;
            }
            if (offset < -2) {
                offset = -2;
            }
            if (offset > 2) {
                offset = 2;
            }
            if (offset > 0) {
                SDL_Delay((Uint32)(this->target_frametime-this->last_frametime + offset));
            }
        }
        frametime_counter.Resume();
    }

    OnCleanup();
    return 0;
}

int main(int argc, char* argv[]) {
    CApp the_app;

    return the_app.OnExecute();
}
