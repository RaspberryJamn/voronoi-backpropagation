#include "NNLayer.h"
#include <cmath>

NNLayer::NNLayer(NNLayerType type) {

    this->type = type;
    this->input_size = 0;
    this->output_size = 0;
    this->parameter_size = 0;

    switch (this->type) {
    case NN_INPUT;
        this->init_func = &NNLayer::NNInputInit;
        this->forward_func = &NNLayer::NNInputForward;
        this->backward_func = &NNLayer::NNInputBackward;
        break;
    case NN_SIGMOID;
        this->init_func = &NNLayer::NNSigmoidInit;
        this->forward_func = &NNLayer::NNSigmoidForward;
        this->backward_func = &NNLayer::NNSigmoidBackward;
        break;
    case NN_DENSE;
        this->init_func = &NNLayer::NNSigmoidInit;
        this->forward_func = &NNLayer::NNSigmoidForward;
        this->backward_func = &NNLayer::NNSigmoidBackward;
        break;
    }
}
void NNLayer::CalculateParameterSize() {
    switch (this->type) {
    case NN_INPUT;
        this->parameter_size = 0;
        break;
    case NN_SIGMOID;
        this->parameter_size = 0;
        break;
    case NN_DENSE;
        this->parameter_size = this->output_size+this->output_size*this->input_size;
        break;
    }
}

NNLayer::~NNLayer() {
    this->type = 0;
    this->input_size = 0;
    this->output_size = 0;
    this->parameter_size = 0;
    this->forward_func = nullptr;
    this->backward_func = nullptr;
}

void NNLayer::SetInputSize(size_t size) {
    this->input_size = size;
}
void NNLayer::SetOutputSize(size_t size) {
    this->output_size = size;
}
void NNLayer::GetParameterSize() {
    return this->parameter_size;
}

void NNLayer::Forward(double** io_values_start, double** read_weight_start) {
    (this->forward_func)(io_values_start, read_weight_start);
}

void NNLayer::Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) {
    (this->backward_func)(read_values_tail, io_back_values_tail, read_weights_tail, write_gradient_tail);
}

void NNLayer::Init(double* write_weights_start) {
    (this->backward_func)(write_weights_start);
}

// ================================================================================= Input
void NNLayer::NNInputInit(double* write_weights_start) {
    return;
}
void NNLayer::NNInputForward(double** io_values_start, double** read_weight_start) {
    return; // just set the first few entries of the array kthx
}
void NNLayer::NNInputBackward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) {
    return;
}

// ================================================================================= Sigmoid
double Sigmoid(double input) {
    return 2.0/(1.0+std::exp(-input))-1.0;
}
void NNLayer::NNSigmoidInit(double* write_weights_start) {
    return;
}
void NNLayer::NNSigmoidForward(double** io_values_start, double** read_weight_start) {
    double* current_layer = (*io_values_start);
    double* next_layer = current_layer+this->input_size;
    for (size_t i = 0; i < this->input_size; i++) {
        next_layer[i] = Sigmoid(current_layer[i]);
    }
    (*io_values_start) = next_layer;
}
void NNLayer::NNSigmoidBackward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) {
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

// ================================================================================= Dense
void NNLayer::NNDenseInit(double* write_weights_start) {
    double* bias = write_weights_start;
    double* weights = bias+this->output_size;
    for (size_t i = 0; i < this->output_size; i++) {
        bias[i] = (std::rand()%100-50)*0.5;
    }
    for (size_t i = 0; i < this->output_size*this->input_size; i++) {
        weights[i] = (std::rand()%100-50)*0.1;
    }
    return;
}
void NNLayer::NNDenseForward(double** io_values_start, double** read_weight_start) {
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
void NNLayer::NNDenseBackward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) { // ex: for a 3x5 linear
    double* current_output = (*read_values_tail)-this->output_size; // read only 5
    double* preceding_input = current_output-this->input_size; // read only 3

    double* current_value_gradient = (*io_back_values_tail)-this->output_size; // read only 5
    double* preceding_value_gradient = current_value_gradient-this->input_size; // this gets set 3

    double* bias = (*read_weights_tail)-this->parameter_size; // read only 5
    double* weights = bias+this->output_size; // read only 15

    double* bias_gradient = (*write_gradient_tail)-this->parameter_size; // appended 5
    double* weights_gradient = bias_gradient+this->output_size; // appended 15

    for (size_t i = 0; i < this->output_size; i++) {
        bias_gradient += current_value_gradient[i];
    }
    for (size_t i = 0; i < this->input_size; i++) {
        double in_node_grad = 0;
        size_t page = 0;
        for (size_t j = 0; j < this->output_size; j++) {
            weights_gradient[page+i] += preceding_input[i]*current_value_gradient[j];
            in_node_grad += weights[page+i]*current_value_gradient[j]
            page += this->input_size;
        }
        preceding_value_gradient[i] = in_node_grad;
    }

    (*read_weights_tail) = bias;
    (*write_gradient_tail) = bias_gradient;

    (*read_values_tail) = preceding_input+this->input_size;
    (*io_back_values_tail) = preceding_input;
}
