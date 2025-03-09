#include "ScreenElement/Pan.h"
//#include <iostream>
#include "IdkFunctions.h"

namespace ScreenElement {
    Pan::Pan() : ScreenElement() {
        this->internal_dims = {0};
        this->texture = new Texture(this->render.context);
        this->camera_corner = {0,0};
        this->camera_corner_initial = {0,0};
    }

    Pan::~Pan() {}

    void Pan::SetPosition(SDL_Rect rect) {
        ScreenElement::ScreenElement::SetPosition(rect);
        if ((this->bounding_box.w != rect.w)||
            (this->bounding_box.h != rect.h)) {
            this->texture->NewBlankFromDims(rect.w, rect.h); // not handling mouse dragging but whatever
            this->DrawIndividualUnder();
            this->DrawIndividualOver();
        }
    }

    void Pan::AddChild(ScreenElement* element) {
        // grow internal_dims to contain element, preserving relative camera position
        SDL_Rect child_pos = element->GetPosition();
        int child_min_x = child_pos.x;
        int child_min_y = child_pos.y;
        int child_max_x = child_min_x+child_pos.w;
        int child_max_y = child_min_y+child_pos.h;
        int current_min_x = this->internal_dims.x;
        int current_min_y = this->internal_dims.y;
        int current_max_x = current_min_x+this->internal_dims.w;
        int current_max_y = current_min_y+this->internal_dims.h;

        int left = current_max_x-child_max_x;
        int right = child_max_x-current_max_x;
        int up = current_max_y-child_max_y;
        int down = child_max_y-current_max_y;

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

            this->prior_context = this->texture->SetSelfAsRenderTarget();
            SDL_SetRenderDrawColor(this->render.context, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(this->render.context);
            SDL_SetRenderTarget(this->render.context, this->prior_context);

            this->image_dirty = true;
        }
//        ScreenElement::ScreenElement::AddChild(element);
        this->child_elements.push_back(element);
    }

    bool Pan::IndividualTick() {
        return false;
    }

    void Pan::DrawIndividualUnder() {
        SDL_SetRenderDrawColor(this->render.context, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(this->render.context);
        this->prior_context = this->texture->SetSelfAsRenderTarget();
    }
    void Pan::DrawIndividualOver() {
        SDL_SetRenderTarget(this->render.context, this->prior_context);
        SDL_Rect cut = {this->camera_corner.x, this->camera_corner.y, this->bounding_box.w, this->bounding_box.h};
        SDL_Rect paste = {0, 0, this->bounding_box.w, this->bounding_box.h};
        if (this->internal_dims.w < paste.w) {paste.w = this->internal_dims.w;}
        if (this->internal_dims.h < paste.h) {paste.h = this->internal_dims.h;}
        this->texture->Render(&cut, &paste);
    }

    void Pan::HandleMouseEvent(MouseInfo& mouse) {
        if ((mouse.on_down) && (mouse.button_mask & SDL_BUTTON_MIDDLE)) {
            this->camera_corner_initial.x = this->camera_corner.x;
            this->camera_corner_initial.y = this->camera_corner.y;
        }
        if (mouse.drag_start.button_mask & SDL_BUTTON_MIDDLE) {
            int dx = mouse.x-mouse.drag_start.x;
            int dy = mouse.y-mouse.drag_start.y;
            this->camera_corner.x = this->camera_corner_initial.x-dx;
            this->camera_corner.y = this->camera_corner_initial.y-dy;
            if (this->camera_corner.x < 0) {this->camera_corner.x = 0;}
            if (this->camera_corner.y < 0) {this->camera_corner.y = 0;}
            if (this->camera_corner.x > this->internal_dims.w-this->bounding_box.w) {this->camera_corner.x = this->internal_dims.w-this->bounding_box.w;}
            if (this->camera_corner.y > this->internal_dims.h-this->bounding_box.h) {this->camera_corner.y = this->internal_dims.h-this->bounding_box.h;}


        }
    }
}
