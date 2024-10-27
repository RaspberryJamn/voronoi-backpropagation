#include "CApp.h"

CApp::CApp() {
    this->running = true;
}

int CApp::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    Timer frametime_timer;
    LapCounter lap_counter;
    lap_counter.Reset();
    while(this->running) {
        frametime_timer.Reset();
        while(SDL_PollEvent(&Event)) {
            OnEvent(Event);
        }
        OnLoop();
        OnRender();
        this->last_frametime = frametime_timer.QueryTicks();
        lap_counter.CallLap();
        this->average_frametime = lap_counter.GetAverageLapTime();
    }

    OnCleanup();
    return 0;
}

int main(int argc, char* argv[]) {
    CApp the_app;

    return the_app.OnExecute();
}
