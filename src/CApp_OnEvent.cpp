#include "CApp.h"
#include <iostream>

void CApp::OnEvent(SDL_Event& event) {

    if (event.type == SDL_QUIT) {
        this->running = false;
    }

    this->main_window->HandleEvent(event);

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        this->mouse.pressed = true;
        SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->OnMouseDown();
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        this->mouse.pressed = false;
        SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->OnMouseUp();
    }
    if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->OnMouseMoved();
        if (this->mouse.pressed) {
            this->OnMouseDragged();
        }
    }
}

void CApp::OnMouseDown() {
//    std::cout << "down" << std::endl;
}

void CApp::OnMouseUp() {
//    std::cout << "up" << std::endl;
}

void CApp::OnMouseMoved() {

}

void CApp::OnMouseDragged() {

}
