#ifndef PAN_H
#define PAN_H

#include "ScreenElement.h"

namespace ScreenElement {
    class Pan : virtual public ScreenElement::ScreenElement {
    public:
        Pan(SDL_Renderer* renderer);
        ~Pan();
        void AddChild(ScreenElement* element) override;

    private:
        bool DrawIndividual() override;
        void HandleMouseEvent(MouseInfo mouse) override;

        SDL_Rect internal_dims;
        SDL_Point camera_corner;
    };
}

#endif // PAN_H
