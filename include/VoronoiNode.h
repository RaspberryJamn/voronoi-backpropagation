#ifndef VORONOINODE_H
#define VORONOINODE_H
#include "SDL.h"
#include <cmath>
#include <vector>
#include "IdkFunctions.h"
#include "NeuralNetwork.h"
#include "NNLayer/Input.h"
#include "NNLayer/Sigmoid.h"
#include "NNLayer/Dense.h"
#include "NNLayer/ODense.h"

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
        RGBColor result = RGBColor(this->r*((std::abs(rhs.r)<0.0001) ? ((rhs.r>0)?1:-1)*10000.0 : 1.0/rhs.r),
                                   this->g*((std::abs(rhs.g)<0.0001) ? ((rhs.g>0)?1:-1)*10000.0 : 1.0/rhs.g),
                                   this->b*((std::abs(rhs.b)<0.0001) ? ((rhs.b>0)?1:-1)*10000.0 : 1.0/rhs.b));
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

struct VoronoiNode {
    VoronoiNode(double x, double y) : x(x), y(y) {
        this->model.x_grad = 0;
        this->model.y_grad = 0;
        this->model.mag = 0;
        this->model.exp = 0;
        this->model.m = 0;
        this->model.network.AddLayer(new NNLayer::Input(2));
        this->model.network.AddLayer(new NNLayer::ODense(8, 8.0, 6.0, 0.1)); this->model.network.AddLayer(new NNLayer::Sigmoid(8));
        this->model.network.AddLayer(new NNLayer::ODense(4, 1.0, 0.5, 1.5)); this->model.network.AddLayer(new NNLayer::Sigmoid(4));
        this->model.network.AddLayer(new NNLayer::Dense(4, 1.0, 0.0, 0.5, 1.5)); this->model.network.AddLayer(new NNLayer::Sigmoid(4));
        this->model.network.AddLayer(new NNLayer::Dense(3, 3.0, 0.5, 0.15, 1.2));
        this->model.network.Build();
    }
    struct {
        int x;
        int y;
        int x_min;
        int y_min;
        int x_max;
        int y_max;
        int box_radius;
        double mag;
    } sort;
    void CalculateBoxRadius(double from_x, double from_y);
    void CalculateMag(double from_x, double from_y, double gain);

    double x;
    double y;

    struct {
        NeuralNetwork network;
        double x_grad;
        double y_grad;
        double mag;
        double exp;
        double m;
    } model;

    static void Print(VoronoiNode* node, int indent);
    static void PrintList(std::vector<VoronoiNode*> node_list, int indent);
    static void Render(VoronoiNode* node, SDL_Renderer* target_renderer);
};

#endif // VORONOINODE_H
