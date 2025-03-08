#ifndef SCREENELEMENT_H
#define SCREENELEMENT_H
#include "MouseInfo.h"
#include "Texture.h"
#include "AtlasNumberDrawer.h"
#include <vector>
#include "StatisticsLibrary.h"

namespace ScreenElement {
    struct RenderUtilType {
        SDL_Renderer* context;
        TTF_Font* font;
        AtlasNumberDrawer* number_drawer;
    };

    class ScreenElement {
        public:
            static void FillInRenderUtils(SDL_Renderer* context, TTF_Font* font, AtlasNumberDrawer* number_drawer, StatisticsLibrary* library);

            ScreenElement();
            virtual ~ScreenElement();

            virtual void SetPosition(SDL_Rect rect);
            virtual void AddChild(ScreenElement* element);
            SDL_Rect GetPosition();

            bool Tick();
            void Draw();

            void MouseEvent(MouseInfo mouse);

            static StatisticsLibrary* stats_library;
            static RenderUtilType render;

        private:
            void ZeroMemberVariables();
            virtual bool IndividualTick() = 0;
            virtual void DrawIndividualUnder() = 0;
            virtual void DrawIndividualOver() = 0;
            virtual void HandleMouseEvent(MouseInfo mouse) = 0;

        protected:

            SDL_Rect bounding_box; // functions as a hitbox for mouse events and as a render rect on most recent parent texture

            bool image_dirty;

            std::vector<ScreenElement*> child_elements;
    };
}


#endif // SCREENELEMENT_H
