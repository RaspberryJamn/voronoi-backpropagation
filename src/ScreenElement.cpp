#include "ScreenElement.h"
#include <vector>
#include <algorithm>
#include <iostream>

namespace ScreenElement {
    RenderUtilType ScreenElement::render = {nullptr, nullptr, nullptr};
    StatisticsLibrary* ScreenElement::stats_library = nullptr;

    void ScreenElement::FillInRenderUtils(SDL_Renderer* context, TTF_Font* font, AtlasNumberDrawer* number_drawer, StatisticsLibrary* library) {
        SDL_assert(ScreenElement::render.context == nullptr);
        ScreenElement::render.context = context;
        ScreenElement::render.font = font;
        ScreenElement::render.number_drawer = number_drawer;
        ScreenElement::stats_library = library;
    }

    ScreenElement::ScreenElement() {
        SDL_assert(this->render.context != nullptr);
        this->ZeroMemberVariables();
        this->image_dirty = true; // we have to render at least once to start out
    }

    ScreenElement::~ScreenElement() {
        std::for_each(this->child_elements.begin(), this->child_elements.end(), [&](ScreenElement* current_element) {
            delete current_element;
        });
        this->ZeroMemberVariables();
    }

    void ScreenElement::ZeroMemberVariables() {
        this->bounding_box = {0};
        this->image_dirty = false;
        this->child_elements.clear();
    }

    void ScreenElement::SetPosition(SDL_Rect rect) {
        this->bounding_box = rect;
    }

    void ScreenElement::AddChild(ScreenElement* element) {
        SDL_Rect child_pos = element->GetPosition();
        int child_min_x = child_pos.x;
        int child_min_y = child_pos.y;
        int child_max_x = child_min_x+child_pos.w;
        int child_max_y = child_min_y+child_pos.h;
        int current_min_x = this->bounding_box.x;
        int current_min_y = this->bounding_box.y;
        int current_max_x = current_min_x+this->bounding_box.w;
        int current_max_y = current_min_y+this->bounding_box.h;
        int min_x = current_min_x; if (child_min_x < min_x) {min_x = child_min_x;}
        int min_y = current_min_y; if (child_min_y < min_y) {min_y = child_min_y;}
        int max_x = current_max_x; if (child_max_x > max_x) {max_x = child_max_x;}
        int max_y = current_max_y; if (child_max_y > max_y) {max_y = child_max_y;}
        this->bounding_box = {min_x, min_y, max_x-min_x, max_y-min_y};

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

