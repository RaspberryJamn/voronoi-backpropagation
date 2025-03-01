#include "ScreenElement.h"
#include <vector>
#include <algorithm>
#include <iostream>

namespace ScreenElement {
    ScreenElement::ScreenElement(SDL_Renderer* renderer) {
        this->ZeroMemberVariables();

//        if (owns_texture) {
//            this->texture = new Texture(renderer);
//            this->owns_texture = true;
//        } else {
//            this->texture = nullptr;
//            this->owns_texture = false;
//        }

        this->renderer = renderer;
    }

    ScreenElement::~ScreenElement() {
//        if (this->owns_texture) {
//            delete this->texture;
//        }
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            delete current_element;
        });
        this->ZeroMemberVariables();
    }

    void ScreenElement::ZeroMemberVariables() {
        this->renderer = nullptr;

        this->bounding_box = {0};

//        this->texture = nullptr;
//        this->owns_texture = false;

        this->image_updated = false;

        this->child_elements.clear();
    }

    void ScreenElement::SetPosition(SDL_Rect rect) {
//        if (this->owns_texture) {
//            if ((this->bounding_box.w != rect.w)||
//                (this->bounding_box.h != rect.h)) {
////                this->texture->NewBlankFromDims(rect.w, rect.h); // not handling mouse dragging but whatever
//                // salvage what was previously rendered onto the new frame
//                Texture* new_texture = new Texture(this->renderer);
//                new_texture->NewBlankFromDims(rect.w, rect.h);
//                SDL_Texture* previous_target = SDL_GetRenderTarget(this->renderer);
//                new_texture->SetSelfAsRenderTarget();
//                this->texture->Render(0,0);
//                SDL_SetRenderTarget(this->renderer, previous_target);
//                delete this->texture;
//                this->texture = new_texture;
//            }
//        }
        this->bounding_box = rect;
    }

    void ScreenElement::AddChild(ScreenElement* element) {
        this->child_elements.push_back(element);
    }
    SDL_Rect ScreenElement::GetPosition() {
        return this->bounding_box;
    }

    bool ScreenElement::Draw() {
        bool any_update = false;
//        SDL_Texture* previous_target = nullptr;
//        if (this->owns_texture) {
//            previous_target = SDL_GetRenderTarget(this->renderer);
//            this->texture->SetSelfAsRenderTarget();
//        }

        any_update |= this->DrawIndividualUnder();
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            any_update |= current_element->Draw();
        });
        any_update |= this->DrawIndividualOver();

//        if (this->owns_texture) {
//            SDL_SetRenderTarget(this->renderer, previous_target);
//            if (any_update) {
//                SDL_Rect cut = this->bounding_box;
//                cut.x = 0;
//                cut.y = 0;
//                this->texture->Render(&cut, &this->bounding_box);
//            }
//        }
        return any_update;
    }

    void ScreenElement::MouseEvent(MouseInfo mouse) {
        this->HandleMouseEvent(mouse);
//        MouseInfo child_mouse = mouse;
//        if (this->owns_texture) {
//            child_mouse.x -= this->bounding_box.x;
//            child_mouse.y -= this->bounding_box.y;
//            child_mouse.drag_start.x -= this->bounding_box.x;
//            child_mouse.drag_start.y -= this->bounding_box.y;
//        }
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            current_element->MouseEvent(mouse);
        });
    }
}

