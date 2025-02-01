#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#include "NNLayer.h"
#include <cstdlib>
#include "SDL.h"
#include <algorithm>
#include <vector>

class NeuralNetwork
{
    public:
        NeuralNetwork();
        ~NeuralNetwork();

        void AddLayer(NNLayer::NNLayer* layer);

        void Build();
        bool IsBuilt();
        void ClearGradients();
        void ApplyGradients(double learning_rate);

        void Input(double* input, size_t input_size); // ([set,set,set], 3) => [set,set,set,0,0,0...]

        void Forward();

        void Output(double* output, size_t output_size); // ([0,0], 2) | [...blah,blah,out1,out2] => [out1,out2]

        void SetOutputGradient(double* output, size_t output_size);
        void SetOutputGradientFromLabel(double* label, size_t label_size); // MSE

        void Backward();

        void GetInputGradient(double* input, size_t input_size); // ([0,0,0], 3) | [grad1,grad2,grad3,blah,blah...] => [grad1,grad2,grad3]

    private:
        std::vector<NNLayer::NNLayer*> layers;

        bool built;

        double* forward_values;
        double* backward_values;
        size_t values_size;

        double* weights;
        double* gradients;
        size_t parameters_size;

        struct {
            double b1;
            double b2;
            double e;
            double* m;
            double* v;
        } optimizer;
};

#endif // NEURALNETWORK_H
