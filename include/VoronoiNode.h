#ifndef VORONOINODE_H
#define VORONOINODE_H

#include "SDL.h"
struct NodeLinkedList;

struct RGBColor {
    double r;
    double g;
    double b;
};

class VoronoiNode {
    public:
        VoronoiNode(double x, double y);
        VoronoiNode(double x, double y, double r, double g, double b);
        ~VoronoiNode();

        void SetResidence(NodeLinkedList* home);
        NodeLinkedList* GetResidence();
        void SetTreeSlot(NodeLinkedList* location);
        NodeLinkedList* GetTreeSlot();

        void SetPosition(double x, double y);
        void SetBounds(int min_x, int max_x, int min_y, int max_y);
        bool IsBounded();

        double GetX();
        double GetY();

        RGBColor SampleColor(double sample_x, double sample_y);
        void ForwardPass(double sample_x, double sample_y);
        void BackwardPass(double sample_x, double sample_y, double m, double dldm, double dldr, double dldg, double dldb);
        void CalculateExp(double offset);
        double GetExp();

        void CalculateDist(double from_x, double from_y, double gain);
        double GetDist();
        void CalculateSortingDist();
        int GetSortingDist();

        void CalculateSortingPos();
        int GetSortingPosX();
        int GetSortingPosY();

    private:
        void Init(double x, double y, double r, double g, double b);

        NodeLinkedList* residential_slot;
        NodeLinkedList* tree_slot;

        double x;
        double y;
        double mag;
        double exp;
        double color[3];

        double x_grad;
        double y_grad;
        double color_grad[3];

        int sorting_x;
        int sorting_y;
        int sorting_dist;
        int sorting_x_min;
        int sorting_x_max;
        int sorting_y_min;
        int sorting_y_max;
};

#endif // VORONOINODE_H
