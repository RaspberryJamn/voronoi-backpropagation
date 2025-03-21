#ifndef PAN_H
#define PAN_H

#include "ScreenElement.h"

namespace ScreenElement {
    class Pan : virtual public ScreenElement::ScreenElement {
    public:
        Pan();
        ~Pan();
        virtual void SetPosition(SDL_Rect rect);
        void AddChild(ScreenElement* element) override;

    private:
        bool IndividualTick() override;
        void DrawIndividualUnder() override;
        void DrawIndividualOver() override;
        void HandleMouseEvent(MouseInfo& mouse) override;

        Texture* texture;
        SDL_Texture* prior_context;

        SDL_Rect internal_dims;
        SDL_Point camera_corner_initial;
        SDL_Point camera_corner;
    };
}

#endif // PAN_H
