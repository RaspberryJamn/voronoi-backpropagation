#include "NeuralNetwork.h"
#include <cmath>
//#include <iostream>

NeuralNetwork::NeuralNetwork() {
    this->layers.clear();

    this->built = false;

    this->forward_values = nullptr;
    this->backward_values = nullptr;
    this->values_size = 0;

    this->weights = nullptr;
    this->gradients = nullptr;
    this->parameters_size = 0;

    this->backward_count = 0;

    this->optimizer.b1 = 0.9;
    this->optimizer.b2 = 0.9;
    this->optimizer.e = 0.000001;
    this->optimizer.m = nullptr;
    this->optimizer.v = nullptr;
}

NeuralNetwork::~NeuralNetwork() {
    std::for_each(this->layers.begin(), this->layers.end(), [&](NNLayer::NNLayer* layer) {
        delete layer;
    });
    this->layers.clear();
    if (this->built) {

        free(this->forward_values);
        free(this->backward_values);

        free(this->weights);
        free(this->gradients);

    }
    this->built = false;
    this->values_size = 0;
    this->parameters_size = 0;
    this->backward_count = 0;
}

void NeuralNetwork::AddLayer(NNLayer::NNLayer* layer) { // output size is set in initialization
    SDL_assert(this->built == false);

    if (!this->layers.empty()) {
        layer->SetInputSize(this->layers.back()->GetOutputSize());
    } else {
        layer->SetInputSize(0);
    }
//    new_layer->SetOutputSize(size);
    layer->CalculateParameterSize();
    this->values_size += layer->GetOutputSize();
    this->parameters_size += layer->GetParameterSize();
    this->layers.push_back(layer);
}

void NeuralNetwork::Build() {
    if (this->built == true) {return;}

    this->forward_values = (double*)malloc( sizeof(double)*this->values_size );
    this->backward_values = (double*)malloc( sizeof(double)*this->values_size );

    this->weights = (double*)malloc( sizeof(double)*this->parameters_size );
    this->gradients = (double*)malloc( sizeof(double)*this->parameters_size );
    this->optimizer.m = (double*)malloc( sizeof(double)*this->parameters_size );
    this->optimizer.v = (double*)malloc( sizeof(double)*this->parameters_size );
    for (size_t i = 0; i < this->parameters_size; i++) {
        this->optimizer.m[i] = 0.0;
        this->optimizer.v[i] = 1.0;
    }

    double* active_weights = this->weights;
    std::for_each(this->layers.begin(), this->layers.end(), [&](NNLayer::NNLayer* layer) {
        layer->Init(&active_weights);
    });
    this->ClearGradients();

    this->built = true;
}
bool NeuralNetwork::IsBuilt() {
    return this->built;
}

void NeuralNetwork::ClearGradients() {
    for (size_t i = 0; i < this->parameters_size; i++) {
        this->gradients[i] = 0.0;
    }
    this->backward_count = 0;
}

void NeuralNetwork::ApplyGradients(double learning_rate) {
    for (size_t i = 0; i < this->parameters_size; i++) {
        double averaged_gradient = this->gradients[i]/(this->backward_count+1);
        this->optimizer.m[i] = this->optimizer.b1*this->optimizer.m[i] + (1.0-this->optimizer.b1)*averaged_gradient;
        this->optimizer.v[i] = this->optimizer.b2*this->optimizer.v[i] + (1.0-this->optimizer.b2)*averaged_gradient*averaged_gradient;
        this->weights[i] -= ( (this->optimizer.m[i]/(1.0-this->optimizer.b1)) / (std::sqrt(this->optimizer.v[i]/(1.0-this->optimizer.b2))+this->optimizer.e) )*learning_rate;
    }
//    for (size_t i = 0; i < this->parameters_size; i++) {
//        this->weights[i] -= this->gradients[i]*learning_rate;
//    }
//    this->ClearGradients();
}

// std::memcpy(new_array, old_array, length);
void NeuralNetwork::Input(double* input, size_t input_size) {
    SDL_assert(this->built == true);

//    std::memcpy(this->forward_values, input, input_size); // 'o~o`
    for (size_t i = 0; i < input_size; i++) {
        this->forward_values[i] = input[i];
    }
}

void NeuralNetwork::Output(double* output, size_t output_size) {
    SDL_assert(this->built == true);

//    std::memcpy(output, this->forward_values+this->values_size-output_size, output_size); // 'O~O`
    for (size_t i = 0; i < output_size; i++) {
        output[i] = this->forward_values[this->values_size-output_size+i];
    }
}

void NeuralNetwork::SetOutputGradient(double* output, size_t output_size) {
    SDL_assert(this->built == true);

    for (size_t i = 0; i < output_size; i++) {
        this->backward_values[this->values_size-output_size+i] = output[i];
    }
}

void NeuralNetwork::SetOutputGradientFromLabel(double* label, size_t label_size) {
    SDL_assert(this->built == true);

    for (size_t i = 0; i < label_size; i++) {
        size_t j = this->values_size-label_size+i;
        this->backward_values[j] = 2.0*(this->forward_values[j]-label[i]); // d_loss_d_forwardvalues[-(label_size..1)]
    }
}

void NeuralNetwork::GetInputGradient(double* input, size_t input_size) {
    SDL_assert(this->built == true);

    for (size_t i = 0; i < input_size; i++) {
        input[i] = this->backward_values[i];
    }
}

void NeuralNetwork::Forward() {
    SDL_assert(this->built == true);

    double* active_vector = this->forward_values;
    double* active_weights = this->weights;
    std::for_each(this->layers.begin(), this->layers.end(), [&](NNLayer::NNLayer* layer) {
        layer->Forward(&active_vector, &active_weights);
    });
}
void NeuralNetwork::Backward() { // MSE, called after NeuralNetwork::Forward()
    SDL_assert(this->built == true);

    double* active_vector = this->forward_values+this->values_size;
    double* active_back_vector = this->backward_values+this->values_size;
    double* active_weights = this->weights+this->parameters_size;
    double* active_gradients = this->gradients+this->parameters_size;
//    std::cout << "NeuralNetwork::Backward" << std::endl;
//    std::cout << "this->layers length: " << this->layers.size() << std::endl;
    for (unsigned i = this->layers.size(); i-- > 0; ) {
        NNLayer::NNLayer* layer = this->layers.at(i);
//    std::for_each(this->layers.end(), this->layers.begin(), [&](NNLayer::NNLayer* layer) {
//        std::cout << "layer: " << layer << std::endl;
//        std::cout << layer->GetInputSize() << " " << layer->GetOutputSize() << " " << layer->GetParameterSize() << " " << std::endl;
//        std::cout << "starting layer->Backawrd" << std::endl;
        layer->Backward(&active_vector, &active_back_vector, &active_weights, &active_gradients);
//        std::cout << "finished layer->Backawrd" << std::endl;
//    });
    }
    this->backward_count++;
}
