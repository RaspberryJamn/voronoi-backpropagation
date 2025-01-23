#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork() {
    this->layers.clear();

    this->built = false;

    this->forward_values = nullptr;
    this->backward_values = nullptr;
    this->values_size = 0;

    this->weights = nullptr;
    this->gradients = nullptr;
    this->parameters_size = 0;

//    std::srand(std::time(0));
}

NeuralNetwork::~NeuralNetwork() {
    std::for_each(this->layers.begin(), this->layers.end(), [&](NNLayer* layer) {
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
}

void NeuralNetwork::AddLayer(NNLayer* layer) {
    SDL_assert(this->built == false);

//    NNLayer::NNLayer layer = *layer_ptr;
//    delete layer_ptr;
    layer->SetInputSize(this->layers.back()->GetOutputSize());
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

    double* active_weights = this->weights;
    std::for_each(this->layers.begin(), this->layers.end(), [&](NNLayer* layer) {
        layer->Init(&active_weights);
    });
    this->ClearGradients();

    this->built = true;
}

void NeuralNetwork::ClearGradients() {
    for (size_t i = 0; i < this->parameters_size; i++) {
        this->gradients[i] = 0.0;
    }
}

void NeuralNetwork::ApplyGradients(double learning_rate) {
    for (size_t i = 0; i < this->parameters_size; i++) {
        this->weights[i] -= this->gradients[i]*learning_rate;
    }
    this->ClearGradients();
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

void NeuralNetwork::Forward() {
    SDL_assert(this->built == true);

    double* active_vector = this->forward_values;
    double* active_weights = this->weights;
    std::for_each(this->layers.begin(), this->layers.end(), [&](NNLayer* layer) {
        layer->Forward(&active_vector, &active_weights);
    });
}
void NeuralNetwork::Backward(double* label, size_t label_size) { // MSE
    SDL_assert(this->built == true);

    for (size_t i = 0; i < label_size; i++) {
        size_t j = this->values_size-i-1;
        this->backward_values[j] = 2.0*(this->forward_values[j]-label[i]); // d_loss_d_forwardvalues[-(0..label_size)]
    }

    double* active_vector = this->forward_values+this->values_size;
    double* active_back_vector = this->backward_values+this->values_size;
    double* active_weights = this->weights+this->parameters_size;
    double* active_gradients = this->gradients+this->parameters_size;
    std::for_each(this->layers.end(), this->layers.begin(), [&](NNLayer* layer) {
        layer->Backward(&active_vector, &active_back_vector, &active_weights, &active_gradients);
    });
}
