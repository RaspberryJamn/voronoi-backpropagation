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

        void AddLayer(NNLayer* layer);

        void Build();
        void ClearGradients();
        void ApplyGradients(double learning_rate);

        void Input(double* input, size_t input_size); // ([set,set,set], 3) => [set,set,set,0,0,0...]

        void Forward();
        void Backward(double* label, size_t label_size); // MSE on last few

        void Output(double* output, size_t output_size); // ([0,0], 2) | [...blah,blah,out1,out2] => [out1,out2]

    private:
        std::vector<NNLayer*> layers;

        bool built;

        double* forward_values;
        double* backward_values;
        size_t values_size;

        double* weights;
        double* gradients;
        size_t parameters_size;
};

#endif // NEURALNETWORK_H
