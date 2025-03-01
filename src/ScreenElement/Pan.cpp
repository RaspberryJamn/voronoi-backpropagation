#include "ScreenElement/Pan.h"
//#include <iostream>

namespace ScreenElement {
    Pan::Pan(SDL_Renderer* renderer) : ScreenElement(renderer) {
        this->internal_dims = {0};
        this->texture = new Texture(renderer);
    }

    Pan::~Pan() {}

    void Pan::SetPosition(SDL_Rect rect) {
        ScreenElement::ScreenElement::SetPosition(rect);
        if ((this->bounding_box.w != rect.w)||
            (this->bounding_box.h != rect.h)) {
            this->texture->NewBlankFromDims(rect.w, rect.h); // not handling mouse dragging but whatever
        }
    }

    void Pan::AddChild(ScreenElement* element) {
        ScreenElement::ScreenElement::AddChild(element);
        // grow internal_dims to contain element, preserving relative camera position
        SDL_Rect child_pos = element->GetPosition();
        int min_x = child_pos.x;
        int min_y = child_pos.y;
        int max_x = min_x+child_pos.w;
        int max_y = min_y+child_pos.h;
        int left = this->internal_dims.x-min_x;
        int right = max_x-(this->internal_dims.x+this->internal_dims.w);
        int up = this->internal_dims.y-min_y;
        int down = max_y-(this->internal_dims.y+this->internal_dims.h);
        bool update_necessary = false;
        if (left > 0) {
            this->internal_dims.x -= left;
            this->internal_dims.w += left;
            this->camera_corner.x += left;
            update_necessary = true;
        }
        if (right > 0) {
            this->internal_dims.w += right;
            update_necessary = true;
        }
        if (up > 0) {
            this->internal_dims.y -= up;
            this->internal_dims.h += up;
            this->camera_corner.y += up;
            update_necessary = true;
        }
        if (down > 0) {
            this->internal_dims.h += down;
            update_necessary = true;
        }
        if (update_necessary) {
            this->texture->NewBlankFromDims(this->internal_dims.w, this->internal_dims.h);
            this->image_updated = true;
        }
    }

    bool Pan::DrawIndividualUnder() {
        bool should_update = true;
        if (should_update) {
            SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(this->renderer);
        }
        return should_update;
    }
    bool Pan::DrawIndividualOver() {
        return false;
    }

    void Pan::HandleMouseEvent(MouseInfo mouse) {

    }
}
