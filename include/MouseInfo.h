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
    } drag_start;
};

#endif // MOUSEINFO_H_INCLUDED
