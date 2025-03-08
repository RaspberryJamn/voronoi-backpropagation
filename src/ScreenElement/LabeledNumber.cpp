#include "ScreenElement/LabeledNumber.h"
#include "Texture.h"
#include <string>
#include "StatisticsLibrary.h"
#include <iostream>

namespace ScreenElement {
    LabeledNumber::LabeledNumber(std::string statistic_name) : ScreenElement() {
        SDL_assert(this->stats_library != nullptr);

        this->label_before = new Texture(this->render.context, this->render.font);
        this->label_after = new Texture(this->render.context, this->render.font);
        this->has_label_after = false;

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

    void LabeledNumber::SetLabel(std::string before, std::string after) {
        SDL_Color text_color = {0,0,0,255};
        int w = 0;
        int h = 0;
        this->label_before->LoadInRenderedText(before, text_color);
        w += this->label_before->GetWidth();
        if (h < this->label_before->GetHeight()) {h = this->label_before->GetHeight();}

        this->label_after->LoadInRenderedText(after, text_color);
        w += this->label_after->GetWidth();
        if (h < this->label_after->GetHeight()) {h = this->label_after->GetHeight();}

        if (this->bounding_box.w < w) {this->bounding_box.w = w;}
        if (this->bounding_box.h < h) {this->bounding_box.h = h;}
        this->image_dirty = true;
        this->has_label_after = true;
    }
    void LabeledNumber::SetLabel(std::string label) {
        SDL_Color text_color = {0,0,0,255};
        this->label_before->LoadInRenderedText(label, text_color);
        if (this->bounding_box.w < this->label_before->GetWidth()) {this->bounding_box.w = this->label_before->GetWidth();}
        if (this->bounding_box.h < this->label_before->GetHeight()) {this->bounding_box.h = this->label_before->GetHeight();}
        this->has_label_after = false;
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
        this->label_before->RenderRTL(&string_bounds);
        this->render.number_drawer->DrawRTL(std::to_string(this->last_number), &string_bounds);
        if (this->has_label_after) {
            this->label_after->RenderRTL(&string_bounds);
        }

        if (this->bounding_box.w<string_bounds.w) {this->bounding_box.w=string_bounds.w;}
        if (this->bounding_box.h<string_bounds.h) {this->bounding_box.h=string_bounds.h;}
    }

    void LabeledNumber::HandleMouseEvent(MouseInfo mouse) {}
}
