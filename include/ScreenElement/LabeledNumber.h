#ifndef PAN_H
#define PAN_H

#include "ScreenElement.h"
#include "AtlasNumberDrawer.h"
#include <string>
#include "StatisticsLibrary.h"

namespace ScreenElement {
    class LabeledNumber : virtual public ScreenElement::ScreenElement {
    public:
        LabeledNumber(std::string statistic_name);
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
        statid_t watched_number;
        int last_number;
    };
}

#endif // PAN_H
