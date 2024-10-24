#ifndef VORONOINODE_H
#define VORONOINODE_H

#include "VoronoiGraph.h"

class VoronoiNode
{
    public:
        VoronoiNode(double x, double y);
        ~VoronoiNode();

        void SetBounds(int min_x, int max_x, int min_y, int max_y);

        double GetX();
        double GetY();
        void UpdateSortingPos();
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
};

#endif // VORONOINODE_H
