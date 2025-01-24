#ifndef NNLAYER_SIGMOID_H
#define NNLAYER_SIGMOID_H

#include <NNLayer.h>
#include <cmath>

namespace NNLayer {
    class Sigmoid : virtual public NNLayer::NNLayer {
    public:
        Sigmoid(size_t output_size);
        ~Sigmoid();

        void CalculateParameterSize() override;

        void Init(double** write_weights_start) override;

        void Forward(double** io_values_start, double** read_weight_start) override;

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;
    };
}

#endif // NNLAYER_SIGMOID_H
