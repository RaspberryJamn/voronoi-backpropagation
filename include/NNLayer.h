#ifndef NNLAYER_H
#define NNLAYER_H

#include <cstddef>

namespace NNLayer {
    class NNLayer {
        public:
            NNLayer(size_t output_size);
            virtual ~NNLayer();

            void SetInputSize(size_t size);
            void SetOutputSize(size_t size); // output size is set in initialization

            size_t GetInputSize();
            size_t GetOutputSize();

            size_t GetParameterSize();

            virtual void CalculateParameterSize() = 0;

            virtual void Init(double** write_weights_start) = 0;

            // input0-._          _,->hidden1-._          _,->hidden2
            //          `=>layer0/              `=>layer1/ <-This layer is the example for the following comments
            // weights0-/`             weights1-/`

            // =======================================================================================================
            //          V io_values_start, writes to (and will set io_values_start to) ---V
            // ...input0input1input1input1input1input1input1input1input1input1input1input1output2output2output2......

            //           V read_weight_start, will be set to ------V
            // ...weight0weight1weight1weight1weight1weight1weight1weight2weight2weight2weight2weight2weight2........
            virtual void Forward(double** io_values_start, double** read_weight_start) = 0;

            // =======================================================================================================
            //          V , then will move read_values_tail to here      V read_values_tail reads from here
            // ...input1output2output2output2output2output2output2output2value3value3value3value3value3value3........

            //            V , then writes to 1                    V io_back_values_tail given here, first reads 2
            // ...backval1backval2backval2backval2backval2backval2backval3backval3backval3backval3backval3backval3...

            //           V , then is set to this for layer 0       V read_weights_tail is given as this for layer 1
            // ...weight1weight2weight2weight2weight2weight2weight2weight3weight3weight3weight3weight3weight3........

            //         V , then is set to this for layer 0          V write_gradient_tail is given as this for layer 1
            // ...grad1grad2grad2grad2grad2grad2grad2grad2grad2grad2grad3grad3grad3grad3grad3grad3grad3grad3grad3....
            virtual void Backward(double** read_values_tail, double** io_back_values_tail, double** read_weights_tail, double** write_gradient_tail) = 0;

        protected:
            size_t input_size;
            size_t output_size;
            size_t parameter_size;
    };
}

#endif // NNLAYER_H
