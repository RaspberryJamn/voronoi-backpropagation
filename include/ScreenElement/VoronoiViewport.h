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
        void SetupChildElements();
        bool IndividualTick() override;
        void DrawIndividualUnder() override;
        void DrawIndividualOver() override;
        void HandleMouseEvent(MouseInfo mouse) override;

        void RenderFullFrameVoronoi(double* running_loss);

        VoronoiGraph* graph;
        int media_width;
        int media_height;
        int refresh_period;

        struct {
            int sample_x;
            int sample_y;
            int hits;
        } render_progress;

        struct {
            double running_loss;
            double last_full_frame_loss;
            int round;
        } render_stats;

        Texture* texture;
    };
}

#endif // VORONOIVIEWPORT_H
