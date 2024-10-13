#include "CApp.h"

void CApp::OnEvent(SDL_Event& event) {

    if (event.type == SDL_QUIT) {
        this->running = false;
    }

    this->main_window->HandleEvent(event);

//    if (Event->type == SDL_WINDOWEVENT_RESIZED) {
//        this->Running = false;
//    }
}
