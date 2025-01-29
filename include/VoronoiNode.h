#ifndef VORONOINODE_H
#define VORONOINODE_H
#include "SDL.h"
#include "IdkFunctions.h"
#include <cmath>
#include "NeuralNetwork.h"

struct NodeLinkedList;

struct RGBColor {
    double r;
    double g;
    double b;
    RGBColor() : r(0), g(0), b(0) {}
    RGBColor(double r, double g, double b) : r(r), g(g), b(b) {}
    RGBColor operator+(RGBColor const& rhs) const {
        return RGBColor(r+rhs.r,g+rhs.g,b+rhs.b);
    }
    void operator+=(RGBColor const& rhs) {
        r += rhs.r; g += rhs.g; b += rhs.b;
    }
    RGBColor operator-(RGBColor const& rhs) const {
        return RGBColor(r-rhs.r,g-rhs.g,b-rhs.b);//RGBColor result = RGBColor(r-rhs.r,g-rhs.g,b-rhs.b); return result; //
    }
    void operator-=(RGBColor const& rhs) {
        r -= rhs.r; g -= rhs.g; b -= rhs.b;
    }
    RGBColor operator*(RGBColor const& rhs) const {
        return RGBColor(r*rhs.r,g*rhs.g,b*rhs.b);//RGBColor result = RGBColor(r*rhs.r,g*rhs.g,b*rhs.b); return result; //
    }
    RGBColor operator/(RGBColor const& rhs) const {
        RGBColor result = RGBColor((std::abs(rhs.r)<0.0001) ? 0 : this->r/rhs.r,
                                   (std::abs(rhs.g)<0.0001) ? 0 : this->g/rhs.g,
                                   (std::abs(rhs.b)<0.0001) ? 0 : this->b/rhs.b);
        return result; //return RGBColor(r/rhs.r,g/rhs.g,b/rhs.b);
    }
    RGBColor operator*(double const& rhs) const {
        return RGBColor(r*rhs,g*rhs,b*rhs);//RGBColor result = RGBColor(r*rhs,g*rhs,b*rhs); return result; //
    }
    static double Trace(RGBColor const& col) {
        return col.r+col.g+col.b;
    }
    void Clamp() {
        G_Clamp<double>(&this->r, 0.01, 255);
        G_Clamp<double>(&this->g, 0.01, 255);
        G_Clamp<double>(&this->b, 0.01, 255);
    }
};

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

        void SetPosition(double x, double y);
        void SetBounds(int min_x, int min_y, int max_x, int max_y);
        bool IsBounded(int x, int y);
        void Clamp(int min_x, int min_y, int max_x, int max_y);

        void CalculateDist(double from_x, double from_y, double gain);
        double GetDist();
        void CalculateSortingDist(double from_x, double from_y);
        int GetSortingDist();

        void CalculateSortingPos();
        int GetSortingPosX();
        int GetSortingPosY();

        double GetX();
        double GetY();


        void Print(int indent);
        void Render(SDL_Renderer* target_renderer);
//        void RenderLoggedGradient(SDL_Renderer* target_renderer);

//        RGBColor SampleColor(double sample_x, double sample_y);
        RGBColor ForwardPass(double sample_x, double sample_y);
        void BackwardPass(double sample_x, double sample_y, double gain,  double* gain_grad, RGBColor finalcolor, RGBColor d_loss_d_finalcolor);
        void ApplyGradients(double learning_rate);
//        void LogGradients();
        void ClearGradients();
        void CalculateExp(double offset);
        double GetExp();
        void SetM(double m);
        double GetM();
        double GetGainGradient();
//        void GetLastGradients(double* x_grad, double* y_grad, RGBColor* color_grad);


    private:
        void Init(double x, double y, double r, double g, double b);

        struct {
            int x;
            int y;
            int x_min;
            int y_min;
            int x_max;
            int y_max;
            int dist;
        } sort;

        double x;
        double y;
//        double gain;
//        double mag;
//        double exp;
//        double m_value;
//        RGBColor color;

//        double x_grad;
//        double y_grad;
//        RGBColor color_grad;
//        double gain_grad;
//        double recent_gain_grad;

//        double* weights; // forward_parameters
//        double* forward_values; // hidden layers
//        double* gradients; // backward_parameters
//        double* backward_values; // layer gradients
        struct {
            NeuralNetwork network;
            double x_grad;
            double y_grad;
            double mag;
            double exp;
            double m;
        } model;
//        RGBColor log_d_loss_d_finalcolor;
//        double last_x_grad;
//        double last_y_grad;
//        RGBColor last_color_grad;
//        int poke_count;
//        int last_poke_count;

//        int sorting_x;
//        int sorting_y;
//        int sorting_dist;
//        int sorting_x_min;
//        int sorting_y_min;
//        int sorting_x_max;
//        int sorting_y_max;
};

#endif // VORONOINODE_H
