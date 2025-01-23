#ifndef NNLAYER_INPUT
#define NNLAYER_INPUT

#include "NNLayer.h"
#include <cmath>

namespace NNLayerNSP {
    class Input : public ::NNLayer {
    public:
        void CalculateParameterSize() override {
            this->parameter_size = 0;
        }

        void Init(double* write_weights_start) override {
            return;
        }

        void Forward(double** io_values_start, double** read_weight_start) override {
            return; // just set the first few entries of the array kthx
        }

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override {
            return;
        }
    };
}
#endif // NNLAYER_INPUT
