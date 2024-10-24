#ifndef VORONOINODE_H
#define VORONOINODE_H

#include "VoronoiGraph.h"

class VoronoiGraph;

class VoronoiNode
{
    public:
        VoronoiNode(double x, double y);
        ~VoronoiNode();

        void SetParent(VoronoiGraph* parent, int min_x, int max_x, int min_y, int max_y);
        VoronoiGraph* GetParent();

        double GetX();
        double GetY();
        int GetSortingPosX();
        int GetSortingPosY();

    private:
        double x;
        double y;
        int sorting_x;
        int sorting_y;
        int sorting_x_min;
        int sorting_x_max;
        int sorting_y_min;
        int sorting_y_max;
        VoronoiGraph* parent;
};

#endif // VORONOINODE_H
