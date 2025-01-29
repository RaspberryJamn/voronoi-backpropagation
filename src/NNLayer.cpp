#include "NNLayer.h"
#include <cmath>

namespace NNLayer {
    NNLayer::NNLayer(size_t output_size) {
        this->input_size = 0;
        this->output_size = output_size;
        this->parameter_size = 0;
    }

    NNLayer::~NNLayer() {
        this->input_size = 0;
        this->output_size = 0;
        this->parameter_size = 0;
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
}
