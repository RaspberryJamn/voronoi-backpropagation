#ifndef VORONOINODE_H
#define VORONOINODE_H

#include "SDL.h"
struct NodeLinkedList;

struct RGBColor {
    double r;
    double g;
    double b;
    RGBColor() : r(0), g(0), b(0) {}
    RGBColor(double r, double g, double b) : r(r), g(g), b(b) {}
    RGBColor operator+(RGBColor const& rhs) {
        return RGBColor(r+rhs.r,g+rhs.g,b+rhs.b);
    }
    void operator+=(RGBColor const& rhs) {
        r += rhs.r; g += rhs.g; b += rhs.b;
    }
    RGBColor operator-(RGBColor const& rhs) {
        return RGBColor(r-rhs.r,g-rhs.g,b-rhs.b);
    }
    void operator-=(RGBColor const& rhs) {
        r -= rhs.r; g -= rhs.g; b -= rhs.b;
    }
    RGBColor operator*(RGBColor const& rhs) {
        return RGBColor(r*rhs.r,g*rhs.g,b*rhs.b);
    }
    RGBColor operator/(RGBColor const& rhs) {
        return RGBColor(r/rhs.r,g/rhs.g,b/rhs.b);
    }
    RGBColor operator*(double const& rhs) {
        return RGBColor(r*rhs,g*rhs,b*rhs);
    }
    static double Trace(RGBColor const& col) {
        return col.r+col.g+col.b;
    }
};

// node1.mag = (node1.x*node1.x+node1.y*node1.y)
// e1 = exp(node1.mag)
// z = e1+e2+e3
// m1 = e1/z
// final_color = m1*node1.color+m2*node2.color+m3*node3.color
// loss = (final_color-desired_color)^2

// dnode1.mag/dnode1.x = node1.x
// dm1/dnode1.mag = m1*(1-m1)
// dfinal_color/dm1 = node1.color
// dnode1.color/dnode1.x = something
// dfinal_color/dnode1.color = m1
// dloss/dfinal_color = 2(final_color-desired_color)

// nope. too complicated. I may revisit you later but for now Im doing it simpler
//struct SerialVoronoiNode {
//    int id; // maybe a pointer?
//    double x;
//    double y;
//
//    double mag;
//    double exp;
//    double m; // sort of like a final layer input
//
//    double x_grad; // += dloss/dfinal_color * output_color * m*(1-m) * x
//    double y_grad; // += dloss/dfinal_color * m1 * dnode1.color/dnode1.y
//
//    struct VoronoiNodeForward forward;
//
//    struct VoronoiNodeBackward backward;
//};
//
//struct VoronoiNodeForward { // data needed to derive pixel color from sample xy (technically the node's own xy is part of this but thats special)
//    struct inputs { // more like intermediate values
//        double x; // set
//        double y; // set
//        double sample_x; // set
//        double sample_y; // set
//        double local_x;
//        double local_y;
//
//        RGBColor output_color;
//    };
//    struct weights {
//        RGBColor color; // just the one, effectively the bias for a 0x3 linear layer
//    };
//};
//
//struct VoronoiNodeBackward {
//    struct outputs { // more like intermediate gradients
//        double x_grad; // this'll get yoinked
//        double y_grad; // this too
//        double sample_x_grad;
//        double sample_y_grad;
//        double local_x_grad;
//        double local_y_grad;
//
//        RGBColor color_grad; // del loss del rgb, with m factored in
//    };
//    struct gradients { // every weight has a corresponding gradient. accumulates
//        RGBColor color_grad;
//    };
//};

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
        void SetBounds(int min_x, int min_y, int max_x, int max_y);
        bool IsBounded();

        double GetX();
        double GetY();

//        RGBColor SampleColor(double sample_x, double sample_y);
        RGBColor ForwardPass(double sample_x, double sample_y);
        void BackwardPass(double sample_x, double sample_y, RGBColor rgb, RGBColor dldrgb);
        void ApplyGradients(double learning_rate);
        void CalculateExp(double offset);
        double GetExp();
        void SetM(double m);
        double GetM();

        void CalculateDist(double from_x, double from_y, double gain);
        double GetDist();
        void CalculateSortingDist(double from_x, double from_y);
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
        double m_value;
        RGBColor color;

        double x_grad;
        double y_grad;
        RGBColor color_grad;

        int sorting_x;
        int sorting_y;
        int sorting_dist;
        int sorting_x_min;
        int sorting_y_min;
        int sorting_x_max;
        int sorting_y_max;
};

#endif // VORONOINODE_H
