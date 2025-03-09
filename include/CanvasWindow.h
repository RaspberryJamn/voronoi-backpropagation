#ifndef CANVASWINDOW_H
#define CANVASWINDOW_H

#include <SDL.h>
#include <ScreenElement.h>
#include "MouseInfo.h"

class CanvasWindow {
    public:
        CanvasWindow();
        //Deallocates internals
        ~CanvasWindow();

        bool Init();
        bool Init(const char* window_caption, int x, int y, int w, int h, Uint32 flags);

        //Creates renderer from internal window
        SDL_Renderer* CreateRenderer();

        //Handles window events
        void HandleEvent(SDL_Event& event, MouseInfo& mouse);

        ScreenElement::ScreenElement* SetRootElement(ScreenElement::ScreenElement* element);

        //Window dimensions
        int GetWidth();
        int GetHeight();

        //Window focii
        bool HasMouseFocus();
        bool HasKeyboardFocus();
        bool IsMinimized();

    private:
        //Window data
        SDL_Window* window;
        SDL_Renderer* renderer;
        Uint32 window_id;

        ScreenElement::ScreenElement* root_element;

        //Window dimensions
        int width;
        int height;

        //Window focus
        bool mouse_focus;
        bool keyboard_focus;
        bool fullscreen;
        bool minimized;
};

#endif // CANVASWINDOW_H
