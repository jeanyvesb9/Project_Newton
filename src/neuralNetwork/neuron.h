#ifndef NEURON_H
#define NEURON_H

#include <cmath>

#include <QList>

namespace NN {

using InputVector = QList<long double>;

struct Weight {
    Weight() = default;
    Weight(long double weight, long double sigma) : weight{weight}, sigma{sigma} { }
    long double weight;
    long double sigma;
};

using NeuronData = QList<Weight>;

class Neuron
{
public:
    Neuron() = default;
    Neuron(const QList<Weight> *weights);

    long double process(InputVector *inputVector);
    inline long double getOutput() const { return this->output; }

private:
    long double output;
    const QList<Weight> *weights;
};

}

#endif // NEURON_H
