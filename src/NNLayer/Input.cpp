#include "NNLayer/Input.h"
//#include <iostream>

namespace NNLayer {
    Input::Input(size_t output_size) : NNLayer(output_size) {}

    Input::~Input() {}

    void Input::CalculateParameterSize() {
        this->parameter_size = 0;
    }

    void Input::Init(double** write_weights_start) {
        return;
    }

    void Input::Forward(double** io_values_start, double** read_weight_start) {
        return; // just set the first few entries of the array kthx
    }

    void Input::Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) {
//        std::cout << "got here" << std::endl;
        return;
    }
}
