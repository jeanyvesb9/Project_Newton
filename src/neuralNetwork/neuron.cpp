#include "neuron.h"

using namespace NN;

Neuron::Neuron(const QList<Weight> *weights)
    : weights{weights}
{

}

long double Neuron::process(InputVector *inputVector)
{
    this->output = 0;
    for(int i = 0; i < inputVector->size(); i++)
    {
        this->output += inputVector->at(i) * weights->at(i).weight;
    }
    this->output = std::tanh(this->output);
    return this->output;
}
