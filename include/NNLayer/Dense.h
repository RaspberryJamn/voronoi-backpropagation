#ifndef NNLAYER_DENSE_H
#define NNLAYER_DENSE_H

#include <NNLayer.h>
#include <cmath>

namespace NNLayer {
    class Dense : virtual public NNLayer::NNLayer {
    public:
        Dense(size_t output_size);
        ~Dense();

        void CalculateParameterSize() override;

        void Init(double** write_weights_start) override;

        void Forward(double** io_values_start, double** read_weight_start) override;

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;
    };
}

#endif // NNLAYER_DENSE_H
