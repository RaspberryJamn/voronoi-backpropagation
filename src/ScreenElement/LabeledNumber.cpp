#include "ScreenElement/LabeledNumber.h"
#include "Texture.h"
#include <string>

namespace ScreenElement {
    LabeledNumber::LabeledNumber(SDL_Renderer* renderer) : ScreenElement(renderer) {
        this->font = nullptr;
        this->number_drawer = nullptr;
        this->label = new Texture(renderer);
        this->number = 0;
    }

    LabeledNumber::~LabeledNumber() {}

    void LabeledNumber::SetPosition(SDL_Rect rect) {
        ScreenElement::ScreenElement::SetPosition(rect);
    }

    void LabeledNumber::SetFont(TTF_Font* font) {
        this->font = font;
        this->label->SetFont(this->font);
    }
    void LabeledNumber::SetNumberDrawer(AtlasNumberDrawer* number_drawer) {
        this->number_drawer = number_drawer;
    }
    void LabeledNumber::SetLabel(std::string label) {
        SDL_assert(this->font != nullptr);
        SDL_Color text_color = {0,0,0,255};
        this->label->LoadInRenderedText(label, text_color);
        this->image_dirty = true;
    }
    void LabeledNumber::SetNumber(int number) {
        if (this->number != number) {
            this->image_dirty = true;
        }
        this->number = number;
    }

    bool LabeledNumber::IndividualTick() {
        return false;
    }
    void LabeledNumber::DrawIndividualUnder() {}
    void LabeledNumber::DrawIndividualOver() {
        SDL_assert(this->font != nullptr);
        SDL_assert(this->number_drawer != nullptr);

        SDL_Rect string_bounds = {this->bounding_box.x, this->bounding_box.y,0,0};
        this->label->RenderRTL(&string_bounds);
        this->number_drawer.DrawRTL(std::to_string(this->number), &string_bounds);
    }

    void LabeledNumber::HandleMouseEvent(MouseInfo mouse) {}
}
