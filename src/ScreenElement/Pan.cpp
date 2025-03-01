#include "ScreenElement/Pan.h"
//#include <iostream>

namespace ScreenElement {
    Pan::Pan(SDL_Renderer* renderer) : ScreenElement(renderer, true) {
        this->internal_dims = {0};
    }

    Pan::~Pan() {}

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
        if (left > 0) {
            this->internal_dims.x -= left;
            this->internal_dims.w += left;
            this->camera_corner.x += left;
        }
        if (right > 0) {
            this->internal_dims.w += right;
        }
        if (up > 0) {
            this->internal_dims.y -= up;
            this->internal_dims.h += up;
            this->camera_corner.y += up;
        }
        if (down > 0) {
            this->internal_dims.h += down;
        }
    }

    bool Pan::DrawIndividual() {
////        std::cout << this->bounding_box.x << ", " << this->bounding_box.y << ", " << this->bounding_box.w << ", " << this->bounding_box.h << std::endl;
//        SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0xFF, 0xFF);
////        SDL_Rect dest = {5,5,10,10};
//        SDL_RenderDrawRect(this->renderer, &this->bounding_box);
        return false;
    }

    void Pan::HandleMouseEvent(MouseInfo mouse) {

    }
}
