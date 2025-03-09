#include "CApp.h"
#include <iostream>
#include "IdkFunctions.h"

void CApp::OnEvent(SDL_Event& event) {

    if (event.type == SDL_QUIT) {
        this->running = false;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        this->mouse.button_mask = SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->mouse.on_down = true;
        this->mouse.status_captured = false;
        this->mouse.pressed = true;
        if (this->mouse.started_dragging == false) {
            this->mouse.drag_start.x = this->mouse.x;
            this->mouse.drag_start.y = this->mouse.y;
        }
        this->OnMouseDown();
    }
    if (event.type == SDL_MOUSEBUTTONUP) {
        this->mouse.button_mask = SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->mouse.status_captured = false;
        if (this->mouse.button_mask == 0) {
            this->mouse.pressed = false;
            this->mouse.on_up = true;
            this->mouse.started_dragging = false;
            this->mouse.drag_start.x = 0;
            this->mouse.drag_start.y = 0;
            this->mouse.drag_start.button_mask = 0;
        }
        this->OnMouseUp();
    }
    if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&(this->mouse.x),&(this->mouse.y));
        this->mouse.status_captured = false;
        if (this->mouse.pressed) {
            if (this->mouse.started_dragging == false) {
                this->mouse.started_dragging = true;
                this->mouse.drag_start.button_mask = this->mouse.button_mask;
            }
        }
        this->OnMouseMoved();
        if (this->mouse.pressed) {
            this->OnMouseDragged();
        }
    }

    if (event.type == SDL_KEYDOWN) {
        this->last_keypress = event.key.keysym.sym;
        this->OnKeyDown(this->last_keypress);
    }

    this->main_window->HandleEvent(event, this->mouse);
    this->mouse.on_down = false;
    this->mouse.on_up = false;
    this->mouse.status_captured = true;
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

void CApp::OnKeyDown(SDL_Keycode key) {
    switch (key) {
    case SDLK_UP:
        this->training_stride *= 2;
        G_Clamp<int>(&this->training_stride, 1, 32);
//        std::cout << this->training_stride << std::endl;
        break;
    case SDLK_DOWN:
        this->training_stride /= 2;
        G_Clamp<int>(&this->training_stride, 1, 32);
//        std::cout << this->training_stride << std::endl;
        break;
    }

}
