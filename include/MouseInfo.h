#ifndef MOUSEINFO_H_INCLUDED
#define MOUSEINFO_H_INCLUDED
#include <SDL_stdinc.h>

struct MouseInfo {
    int x;
    int y;
    bool pressed;
    bool started_dragging;
    uint32_t button_mask;
    struct DragStart {
        int x;
        int y;
        int32_t button_mask;
    } drag_start;
    bool on_down;
    bool on_up;
    bool status_captured; // set to false on all mouse events, set to true when consumed by someone, or just when everyone says no thanks
};

#endif // MOUSEINFO_H_INCLUDED
