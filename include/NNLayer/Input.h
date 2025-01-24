#ifndef NNLAYER_INPUT_H
#define NNLAYER_INPUT_H

#include <NNLayer.h>
#include <cmath>

namespace NNLayer {
    class Input : virtual public NNLayer::NNLayer {
    public:
        Input(size_t output_size);
        ~Input();

        void CalculateParameterSize() override;

        void Init(double** write_weights_start) override;

        void Forward(double** io_values_start, double** read_weight_start) override;

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;
    };
}

#endif // NNLAYER_INPUT_H
