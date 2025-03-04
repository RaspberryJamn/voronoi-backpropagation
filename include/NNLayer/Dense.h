#ifndef NNLAYER_DENSE_H
#define NNLAYER_DENSE_H

#include <NNLayer.h>
#include <cmath>

namespace NNLayer {
    class Dense : virtual public NNLayer::NNLayer {
    public:
        Dense(size_t output_size, double speed, double bias_mean, double bias_variance, double weight_variance);
        ~Dense();

        void CalculateParameterSize() override;

        void Init(double** write_weights_start) override;

        void Forward(double** io_values_start, double** read_weight_start) override;

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;
    private:
        double bias_mean;
        double bias_variance;
        double weight_variance;
    };
}

#endif // NNLAYER_DENSE_H
