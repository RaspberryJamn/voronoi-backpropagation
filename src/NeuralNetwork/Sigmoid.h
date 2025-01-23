#ifndef NNLAYER_SIGMOID
#define NNLAYER_SIGMOID

#include "NNLayer.h"
#include <cmath>

double Sigmoid(double input) {
    return 2.0/(1.0+std::exp(-input))-1.0;
}
namespace NNLayerNSP {
    class Sigmoid : public ::NNLayer {
    public:
        void CalculateParameterSize() override {
            this->parameter_size = 0;
        }

        void Init(double* write_weights_start) override {
            return;
        }

        void Forward(double** io_values_start, double** read_weight_start) override {
            double* current_layer = (*io_values_start);
            double* next_layer = current_layer+this->input_size;
            for (size_t i = 0; i < this->input_size; i++) {
                next_layer[i] = Sigmoid(current_layer[i]);
            }
            (*io_values_start) = next_layer;
        }

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override {
            double* current_output = (*read_values_tail)-this->output_size;
            double* preceding_input = current_output-this->input_size;

            double* current_value_gradient = (*io_back_values_tail)-this->output_size;
            double* preceding_value_gradient = current_value_gradient-this->input_size;

            for (size_t i = 0; i < this->input_size; i++) {
                preceding_value_gradient[i] = (1.0-current_output[i]*current_output[i]) * current_value_gradient[i];
            }

            (*read_values_tail) = preceding_input+this->input_size;
            (*io_back_values_tail) = preceding_input;
        }
    };
}
#endif // NNLAYER_SIGMOID
