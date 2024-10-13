#include "CanvasWindow.h"

CanvasWindow::CanvasWindow() {
    this->window = nullptr;
    this->window_id = 0;
    this->renderer = nullptr;
    this->mouse_focus = false;
    this->keyboard_focus = false;
    this->fullscreen = false;
    this->minimized = false;
    this->width = 0;
    this->height = 0;
}
CanvasWindow::~CanvasWindow() {
    if(this->window != nullptr) {
		SDL_DestroyWindow(this->window);
	}
    this->mouse_focus = false;
    this->keyboard_focus = false;
    this->fullscreen = false;
    this->minimized = false;
    this->width = 0;
    this->height = 0;
}

bool CanvasWindow::Init() {
    int default_width = 400;
    int default_height = 400;
    const char* default_caption = "SDL2 window";
    return this->Init(default_caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, default_width, default_height, SDL_WINDOW_SHOWN);
}

bool CanvasWindow::Init(const char* window_caption, int x, int y, int w, int h, Uint32 flags) {

    this->window = SDL_CreateWindow("Default Caption", x, y, w, h, flags);
    if (this->window == nullptr) {
        return false;
    }
    this->window_id = SDL_GetWindowID(this->window);
    this->mouse_focus = true;
    this->keyboard_focus = true;
    this->fullscreen = false;
    this->minimized = false;
    this->width = w;
    this->height = h;

    return true;
}

SDL_Renderer* CanvasWindow::CreateRenderer() {
    if (this->renderer == nullptr) {
       this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
       SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    }
    return this->renderer;
}

void CanvasWindow::HandleEvent(SDL_Event& event) {
    if ((event.type == SDL_WINDOWEVENT) && (event.window.windowID == this->window_id)) {
        switch (event.window.event) {

            case SDL_WINDOWEVENT_SIZE_CHANGED:
                this->width = event.window.data1;
                this->height = event.window.data2;
                SDL_RenderPresent(this->renderer);
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(this->renderer);
                break;

            case SDL_WINDOWEVENT_ENTER:
                this->mouse_focus = true;
                break;
            case SDL_WINDOWEVENT_LEAVE:
                this->mouse_focus = false;
                break;

            case SDL_WINDOWEVENT_FOCUS_GAINED:
                this->keyboard_focus = true;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                this->keyboard_focus = false;
                break;

            case SDL_WINDOWEVENT_MINIMIZED:
                this->minimized = true;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                this->minimized = false;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                this->minimized = false;
                break;
        }
    }
}

int CanvasWindow::GetWidth() {
    return this->width;
}

int CanvasWindow::GetHeight() {
    return this->height;
}

bool CanvasWindow::HasMouseFocus() {
    return this->mouse_focus;
}

bool CanvasWindow::HasKeyboardFocus() {
    return this->keyboard_focus;
}

bool CanvasWindow::IsMinimized() {
    return this->minimized;
}
