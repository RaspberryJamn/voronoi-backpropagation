#ifndef PAN_H
#define PAN_H

#include "ScreenElement.h"
#include "AtlasNumberDrawer.h"
#include <string>

namespace ScreenElement {
    class LabeledNumber : virtual public ScreenElement::ScreenElement {
    public:
        LabeledNumber(const int* watched_number);
        LabeledNumber(const double* watched_number);
        ~LabeledNumber();
        virtual void SetPosition(SDL_Rect rect);

        void SetLabel(std::string label);
    private:
        bool IndividualTick() override;
        void DrawIndividualUnder() override;
        void DrawIndividualOver() override;
        void HandleMouseEvent(MouseInfo mouse) override;

//        void UpdateTextures();

        Texture* label;
        bool is_int;
        const int* watched_number_int;
        const double* watched_number_double;
        int last_number;
    };
}

#endif // PAN_H
