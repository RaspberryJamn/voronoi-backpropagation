#ifndef NNLAYER_ODENSE_H
#define NNLAYER_ODENSE_H

#include <NNLayer.h>
#include <cmath>

namespace NNLayer {
    class ODense : virtual public NNLayer::NNLayer {
    public:
        ODense(size_t output_size, double bias_speed, double bias_variance, double weight_speed, double weight_variance);
        ~ODense();

        void CalculateParameterSize() override;

        void Init(double** write_weights_start) override;

        void Forward(double** io_values_start, double** read_weight_start) override;

        void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) override;
    private:
        double bias_speed;
        double bias_variance;
        double weight_speed;
        double weight_variance;
    };
}

#endif // NNLAYER_ODENSE_H
