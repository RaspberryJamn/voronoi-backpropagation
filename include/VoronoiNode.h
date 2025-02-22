#ifndef VORONOINODE_H
#define VORONOINODE_H
#include "SDL.h"
#include <vector>
#include "AtlasNumberDrawer.h"
#include "NeuralNetwork.h"
#include "NNLayer/Input.h"
#include "NNLayer/Sigmoid.h"
#include "NNLayer/Dense.h"
#include "NNLayer/ODense.h"

struct VoronoiNode {
    VoronoiNode(double x, double y) : x(x), y(y) {
        this->model.x_grad = 0;
        this->model.y_grad = 0;
        this->model.mag = 0;
        this->model.exp = 0;
        this->model.m = 0;
        this->model.accum_loss = 0;
        this->model.prev_accum_loss = 0;
        this->model.network.AddLayer(new NNLayer::Input(2));
//        this->model.network.AddLayer(new NNLayer::ODense(8, 10.0, 12.0, 3.0, 0.2)); this->model.network.AddLayer(new NNLayer::Sigmoid(8));
//        this->model.network.AddLayer(new NNLayer::ODense(4, 1.0, 0.5, 1.0, 1.5)); this->model.network.AddLayer(new NNLayer::Sigmoid(4));
//        this->model.network.AddLayer(new NNLayer::Dense(4, 1.0, 0.0, 0.5, 1.5)); this->model.network.AddLayer(new NNLayer::Sigmoid(4));
        this->model.network.AddLayer(new NNLayer::ODense(1, 0.0, 0.0, 3.0, 0.2)); this->model.network.AddLayer(new NNLayer::Sigmoid(1));
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
        double accum_loss;
        double prev_accum_loss;
    } model;

    static void Print(VoronoiNode* node, int indent);
    static void PrintList(std::vector<VoronoiNode*> node_list, int indent);
    static void Render(VoronoiNode* node, SDL_Renderer* target_renderer, AtlasNumberDrawer* number_renderer);
};

#endif // VORONOINODE_H
