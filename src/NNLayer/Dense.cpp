#include "NNLayer/Dense.h"
#include <cstdlib>
//#include <iostream>

namespace NNLayer {
    Dense::Dense(size_t output_size) : NNLayer(output_size) {}

    Dense::~Dense() {}

    void Dense::CalculateParameterSize() {
        this->parameter_size = this->output_size+this->output_size*this->input_size;
    }

    void Dense::Init(double** write_weights_start) {
        double* bias = (*write_weights_start);
        double* weights = bias+this->output_size;
        for (size_t i = 0; i < this->output_size; i++) {
            bias[i] = (std::rand()%100-50)*0.5;
        }
        for (size_t i = 0; i < this->output_size*this->input_size; i++) {
            weights[i] = (std::rand()%100-50)*0.1;
        }
        (*write_weights_start) = (*write_weights_start)+this->parameter_size;
    }

    void Dense::Forward(double** io_values_start, double** read_weight_start) {
        double* current_layer = (*io_values_start);
        double* next_layer = current_layer+this->input_size;
        double* bias = (*read_weight_start);
        double* weights = bias+this->output_size;

        size_t page = 0;
        for (size_t i = 0; i < this->output_size; i++) {
            double out_node = bias[i];
            for (size_t j = 0; j < this->input_size; j++) {
                out_node += current_layer[j]*weights[page+j];
            }
            page += this->input_size;
            next_layer[i] = out_node;
        }

        (*io_values_start) = next_layer;
        (*read_weight_start) = (*read_weight_start)+this->parameter_size;
    }

    void Dense::Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) { // ex: 3 -> 5 dense
//        std::cout << "got here" << std::endl;
        double* current_output = (*read_values_tail)-this->output_size; // read only 5
        double* preceding_input = current_output-this->input_size; // read only 3

        double* current_value_gradient = (*io_back_values_tail)-this->output_size; // read only 5
        double* preceding_value_gradient = current_value_gradient-this->input_size; // write only 3

        double* bias = (*read_weights_tail)-this->parameter_size; // read only 5
        double* weights = bias+this->output_size; // read only 15

        double* bias_gradient = (*write_gradient_tail)-this->parameter_size; // appended 5
        double* weights_gradient = bias_gradient+this->output_size; // appended 15

        for (size_t i = 0; i < this->output_size; i++) {
            bias_gradient[i] += current_value_gradient[i];
        }
        for (size_t i = 0; i < this->input_size; i++) {
            double in_node_grad = 0;
            size_t page = 0;
            for (size_t j = 0; j < this->output_size; j++) {
                weights_gradient[page+i] += preceding_input[i]*current_value_gradient[j];
                in_node_grad += weights[page+i]*current_value_gradient[j];
                page += this->input_size;
            }
            preceding_value_gradient[i] = in_node_grad;
        }

        (*read_weights_tail) = bias;
        (*write_gradient_tail) = bias_gradient;

        (*read_values_tail) = preceding_input+this->input_size;
        (*io_back_values_tail) = preceding_value_gradient+this->input_size;
    }
}
