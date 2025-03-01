#ifndef VORONOIVIEWPORT_H
#define VORONOIVIEWPORT_H

#include "ScreenElement.h"
#include "VoronoiGraph.h"

namespace ScreenElement {
    class VoronoiViewport : virtual public ScreenElement::ScreenElement {
    public:
        VoronoiViewport(SDL_Renderer* renderer, VoronoiGraph* graph);
        ~VoronoiViewport();
    private:
        bool DrawIndividual() override;
        void HandleMouseEvent(MouseInfo mouse) override;

        void RenderFullFrameVoronoi(double* running_loss);

        VoronoiGraph* graph;
        int media_width;
        int media_height;
        int refresh_period;
    };
}

#endif // VORONOIVIEWPORT_H
