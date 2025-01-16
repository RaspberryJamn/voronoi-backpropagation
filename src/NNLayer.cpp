#include "NNLayer.h"

NNLayer::NNLayer(NNLayerType type) {

    this->type = type;
    this->input_size = 0;
    this->output_size = 0;
    this->parameter_size = 0;

    switch (this->type) {
    case NN_INPUT;
        this->forward_func = &NNInputForward;
        this->backward_func = &NNInputBackward;
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

void NNInputForward(double** read_layer_start, double** read_weight_start) {
    return; // just set the first few entries of the array kthx
}
void NNInputBackward(double** back_values_read, double** weights_tail, double** gradient_tail) {
    return;
}
