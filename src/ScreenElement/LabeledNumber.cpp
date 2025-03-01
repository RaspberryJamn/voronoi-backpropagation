#include "ScreenElement/LabeledNumber.h"
//#include <iostream>

namespace ScreenElement {
    LabeledNumber::LabeledNumber(SDL_Renderer* renderer, AtlasNumberDrawer* number_drawer) {
        this->internal_dims = {0};
        this->texture = new Texture(renderer);
    }

    LabeledNumber::~LabeledNumber() {}

    void LabeledNumber::SetPosition(SDL_Rect rect) {
        ScreenElement::ScreenElement::SetPosition(rect);
    }

    bool LabeledNumber::DrawIndividualUnder() {
        bool should_update = true;
        if (should_update) {
            SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(this->renderer);
        }
        return should_update;
    }
    bool LabeledNumber::DrawIndividualOver() {
        return false;
    }

    void LabeledNumber::HandleMouseEvent(MouseInfo mouse) {

    }
}
