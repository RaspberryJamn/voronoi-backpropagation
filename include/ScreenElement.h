#ifndef SCREENELEMENT_H
#define SCREENELEMENT_H
#include "MouseInfo.h"
#include "Texture.h"
#include <vector>

class ScreenElement {
    public:
        ScreenElement(SDL_Renderer* renderer, bool owns_texture);
        virtual ~ScreenElement();

//        void UpdateContainerRect(SDL_Rect container);
//        void Assemble();
        void SetPosition(SDL_Rect rect);

        // returns whether there was any change in the image.
        // really, this function ensures that the image is in sync with the most modern notion of what it should be
        // (whether that means actually "drawing" anything or not just happens to be up to the current situation)
        bool Draw();

        void MouseEvent(MouseInfo mouse);

    private:
        void ZeroMemberVariables();

    protected:
        virtual bool DrawIndividual() = 0;
        virtual void HandleMouseEvent(MouseInfo mouse) = 0;

        SDL_Renderer* renderer;

//        SDL_Rect container_box;
        SDL_Rect bounding_box; // internal_rect is always within container_box

        int min_width;
        int min_height;

        Texture* texture;
        bool owns_texture;

        bool image_updated;

        std::vector<ScreenElement*> child_elements;
};

#endif // SCREENELEMENT_H
