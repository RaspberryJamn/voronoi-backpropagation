#include "CApp.h"
#include <iostream>
CApp::CApp() {
    this->running = true;
}

int CApp::OnExecute() {

    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    while(this->running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(Event);
        }
        OnLoop();
        OnRender();
    }

    OnCleanup();
    return 0;
}

int main(int argc, char* argv[]) {
    CApp the_app;

    return the_app.OnExecute();
}
