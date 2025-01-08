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
        this->mouse.drag_start.x = this->mouse.x;
        this->mouse.drag_start.y = this->mouse.y;
        this->OnMouseDown();
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        this->mouse.pressed = false;
        this->mouse.started_dragging = false;
        this->mouse.drag_start.x = 0;
        this->mouse.drag_start.y = 0;
        SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->OnMouseUp();
    }
    if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        if (this->mouse.pressed) {
            this->mouse.started_dragging = true;
        }
        this->OnMouseMoved();
        if (this->mouse.pressed) {
            this->OnMouseDragged();
        }
    }
}

void CApp::OnMouseDown() {
    this->ProgressivePoke(1.0);
}

void CApp::OnMouseUp() {
//    std::cout << "up" << std::endl;
}

void CApp::OnMouseMoved() {

}

void CApp::OnMouseDragged() {

}
