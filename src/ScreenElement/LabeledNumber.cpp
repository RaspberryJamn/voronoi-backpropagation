#include "ScreenElement/LabeledNumber.h"
#include "Texture.h"
#include <string>
#include "StatisticsLibrary.h"
#include <iostream>

namespace ScreenElement {
    LabeledNumber::LabeledNumber(std::string statistic_name) : ScreenElement() {
        SDL_assert(this->stats_library != nullptr);

        this->label = new Texture(this->render.context, this->render.font);

        this->watched_number = this->stats_library->FindIdFromName(statistic_name);
        SDL_assert(this->watched_number != 0);

        this->last_number = 0;
    }

    LabeledNumber::~LabeledNumber() {}

    void LabeledNumber::SetPosition(SDL_Rect rect) {
        SDL_Rect edit_rect = rect;
        if (edit_rect.w < this->bounding_box.w) {edit_rect.w = this->bounding_box.w;}
        if (edit_rect.h < this->bounding_box.h) {edit_rect.h = this->bounding_box.h;}
        ScreenElement::ScreenElement::SetPosition(edit_rect);
    }

    void LabeledNumber::SetLabel(std::string label) {
        SDL_Color text_color = {0,0,0,255};
        this->label->LoadInRenderedText(label, text_color);
        if (this->bounding_box.w < this->label->GetWidth()) {this->bounding_box.w = this->label->GetWidth();}
        if (this->bounding_box.h < this->label->GetHeight()) {this->bounding_box.h = this->label->GetHeight();}
        this->image_dirty = true;
    }

    bool LabeledNumber::IndividualTick() {
//        std::cout << "LabeledNumber ticked" << std::endl;
        int number = this->stats_library->ReadIntFromId(this->watched_number);
        if (this->last_number != number) {
            this->last_number = number;
//            this->image_dirty = true;
            return true;
        }
        return false;
    }
    void LabeledNumber::DrawIndividualUnder() {}
    void LabeledNumber::DrawIndividualOver() {

        SDL_SetRenderDrawColor(this->render.context, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderFillRect(this->render.context, &this->bounding_box);

        SDL_Rect string_bounds = {this->bounding_box.x, this->bounding_box.y,0,0};
        this->label->RenderRTL(&string_bounds);
        this->render.number_drawer->DrawRTL(std::to_string(this->last_number), &string_bounds);

        if (this->bounding_box.w<string_bounds.w) {this->bounding_box.w=string_bounds.w;}
        if (this->bounding_box.h<string_bounds.h) {this->bounding_box.h=string_bounds.h;}
    }

    void LabeledNumber::HandleMouseEvent(MouseInfo mouse) {}
}
