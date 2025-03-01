#include "ScreenElement.h"
#include <vector>
#include <algorithm>
#include <iostream>

namespace ScreenElement {
    ScreenElement::ScreenElement(SDL_Renderer* renderer) {
        this->ZeroMemberVariables();
        this->renderer = renderer;
    }

    ScreenElement::~ScreenElement() {
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            delete current_element;
        });
        this->ZeroMemberVariables();
    }

    void ScreenElement::ZeroMemberVariables() {
        this->renderer = nullptr;
        this->bounding_box = {0};
        this->image_dirty = false;
        this->child_elements.clear();
    }

    void ScreenElement::SetPosition(SDL_Rect rect) {
        this->bounding_box = rect;
    }

    void ScreenElement::AddChild(ScreenElement* element) {
        this->child_elements.push_back(element);
    }
    SDL_Rect ScreenElement::GetPosition() {
        return this->bounding_box;
    }

    bool ScreenElement::Tick() {
        bool any_update = false;
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            any_update |= current_element->Tick();
        });
        any_update |= this->IndividualTick();
        this->image_dirty |= any_update;
        return any_update;
    }

    void ScreenElement::Draw() {
        if (this->image_dirty) {
            this->DrawIndividualUnder();
            std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
                current_element->Draw();
            });
            this->DrawIndividualOver();
            this->image_dirty = false;
        }
    }

    void ScreenElement::MouseEvent(MouseInfo mouse) {
        this->HandleMouseEvent(mouse);
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            current_element->MouseEvent(mouse);
        });
    }
}

