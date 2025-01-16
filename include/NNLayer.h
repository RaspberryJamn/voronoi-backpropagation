#ifndef NNLAYER_H
#define NNLAYER_H

enum NNLayerType {
    NN_INPUT,
    NN_DENSE,
    NN_SIGMOID
};

class NNLayer {
    public:
        NNLayer(NNLayerType type);
        ~NNLayer();

        void SetOutputSize(size_t size);
        void SetInputSize(size_t size);

        size_t GetValueSize();
        size_t GetParameterSize();

        // input0-._          _,->hidden1-._          _,->hidden2
        //          `=>layer0/              `=>layer1/ <-This layer is the example for the following comments
        // weights0-/`             weights1-/`

        // =======================================================================================================
        //          V read_layer_start, writes to (and will set read_layer_start to) -V
        // ...input0input1input1input1input1input1input1input1input1input1input1input1output2output2output2......

        //           V read_weight_start, will be set to ------V
        // ...weight0weight1weight1weight1weight1weight1weight1weight2weight2weight2weight2weight2weight2........
        void Forward(double** read_layer_start, double** read_weight_start);

        // =======================================================================================================
        //            V , then writes here                    V first reads from here
        // ...backval0backval1backval1backval1backval1backval1backval2backval2backval2backval2backval2backval2...

        //           V , then is set to this for layer 0       V weights_tail is given as this for layer 1
        // ...weight0weight1weight1weight1weight1weight1weight1weight2weight2weight2weight2weight2weight2........

        //         V , then is set to this for layer 0          V gradient_tail is given as this for layer 1
        // ...grad0grad1grad1grad1grad1grad1grad1grad1grad1grad1grad2grad2grad2grad2grad2grad2grad2grad2grad2....
        void Backward(double** back_values_read, double** weights_tail, double** gradient_tail);

    private:
        void CalculateParameterSize();

        NNLayerType type;
        size_t input_size;
        size_t output_size;
        size_t parameter_size;

        void *forward_func;
        void *backward_func;
};

#endif // NNLAYER_H
