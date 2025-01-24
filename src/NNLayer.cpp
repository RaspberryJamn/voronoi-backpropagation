#include "NNLayer.h"
#include <cmath>

namespace NNLayer {
    NNLayer::NNLayer(size_t output_size) {//NNLayerType type) {

    //    this->type = type;
        this->input_size = 0;
        this->output_size = output_size;
        this->parameter_size = 0;

    //    switch (this->type) {
    //    case NN_INPUT;
    //        this->init_func = &NNLayer::NNInputInit;
    //        this->forward_func = &NNLayer::NNInputForward;
    //        this->backward_func = &NNLayer::NNInputBackward;
    //        break;
    //    case NN_SIGMOID;
    //        this->init_func = &NNLayer::NNSigmoidInit;
    //        this->forward_func = &NNLayer::NNSigmoidForward;
    //        this->backward_func = &NNLayer::NNSigmoidBackward;
    //        break;
    //    case NN_DENSE;
    //        this->init_func = &NNLayer::NNSigmoidInit;
    //        this->forward_func = &NNLayer::NNSigmoidForward;
    //        this->backward_func = &NNLayer::NNSigmoidBackward;
    //        break;
    //    }
    }
    //void NNLayer::CalculateParameterSize() {
    //    switch (this->type) {
    //    case NN_INPUT;
    //        this->parameter_size = 0;
    //        break;
    //    case NN_SIGMOID;
    //        this->parameter_size = 0;
    //        break;
    //    case NN_DENSE;
    //        this->parameter_size = this->output_size+this->output_size*this->input_size;
    //        break;
    //    }
    // }

    NNLayer::~NNLayer() {
    //    this->type = 0;
        this->input_size = 0;
        this->output_size = 0;
        this->parameter_size = 0;
    //    this->forward_func = nullptr;
    //    this->backward_func = nullptr;
    }

    void NNLayer::SetInputSize(size_t size) {
        this->input_size = size;
    }
    void NNLayer::SetOutputSize(size_t size) {
        this->output_size = size;
    }

    size_t NNLayer::GetInputSize() {
        return this->input_size;
    }
    size_t NNLayer::GetOutputSize() {
        return this->output_size;
    }

    size_t NNLayer::GetParameterSize() {
        return this->parameter_size;
    }

    //void NNLayer::Forward(double** io_values_start, double** read_weight_start) {
    //    (this->forward_func)(io_values_start, read_weight_start);
    // }
    //
    //void NNLayer::Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) {
    //    (this->backward_func)(read_values_tail, io_back_values_tail, read_weights_tail, write_gradient_tail);
    // }
    //
    //void NNLayer::Init(double* write_weights_start) {
    //    (this->backward_func)(write_weights_start);
    // }
}
