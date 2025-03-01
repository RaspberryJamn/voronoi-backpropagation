#ifndef SCREENELEMENT_H
#define SCREENELEMENT_H
#include "MouseInfo.h"
#include "Texture.h"
#include <vector>

namespace ScreenElement {
    class ScreenElement {
        public:
            ScreenElement(SDL_Renderer* renderer);
            virtual ~ScreenElement();

            virtual void SetPosition(SDL_Rect rect);
            virtual void AddChild(ScreenElement* element);
            SDL_Rect GetPosition();

            // returns whether there was any change in the image.
            // really, this function ensures that the image is in sync with the most modern notion of what it should be
            // (whether that means actually "drawing" anything or not just happens to be up to the current situation)
            bool Draw();

            void MouseEvent(MouseInfo mouse);

        private:
            void ZeroMemberVariables();
            virtual bool DrawIndividualUnder() = 0;
            virtual bool DrawIndividualOver() = 0;
            virtual void HandleMouseEvent(MouseInfo mouse) = 0;

        protected:
            SDL_Renderer* renderer;

            SDL_Rect bounding_box; // functions as a hitbox for mouse events and as a render rect on most recent parent texture

            bool image_updated;

            std::vector<ScreenElement*> child_elements;
    };
}


#endif // SCREENELEMENT_H
