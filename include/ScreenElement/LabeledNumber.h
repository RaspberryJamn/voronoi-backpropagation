#ifndef PAN_H
#define PAN_H

#include "ScreenElement.h"
#include "AtlasNumberDrawer.h"
#include <string>

namespace ScreenElement {
    class LabeledNumber : virtual public ScreenElement::ScreenElement {
    public:
        LabeledNumber(SDL_Renderer* renderer);
        ~LabeledNumber();
        virtual void SetPosition(SDL_Rect rect);

        void SetFont(TTF_Font* font);
        void SetNumberDrawer(AtlasNumberDrawer* number_drawer);
        void SetLabel(std::string label);
        void SetNumber(int number);
    private:
        bool IndividualTick() override;
        void DrawIndividualUnder() override;
        void DrawIndividualOver() override;
        void HandleMouseEvent(MouseInfo mouse) override;

//        void UpdateTextures();

        TTF_Font* font;
        Texture* label;
        AtlasNumberDrawer* number_drawer;
        int number;
    };
}

#endif // PAN_H
