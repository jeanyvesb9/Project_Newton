#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <random>
#include <cmath>

#include <QObject>
#include <QList>
#include <QPair>
#include <QSharedPointer>
#include <QDebug>

#include "src/neuralNetwork/neuron.h"
#include "src/checkers/piecestype.h"

namespace NN {

using InternalTopology = QList<unsigned int>; //ONLY Hidden Layers
using Layer = QList<Neuron>;
using LayerData = QList<NeuronData>;

struct NNTopologyData {
    NNTopologyData() = default;
    NNTopologyData(QVector<long double> *rawWeightsSigmas, const InternalTopology *topology, long double king);
    QList<LayerData> weights; //Includes Output Layer
    unsigned int Nw; //Number of weights
    long double king;
};
using NNTopologyDataPointer = QSharedPointer<NNTopologyData>;

class NeuralNetwork;
using NeuralNetworkPointer = QSharedPointer<NeuralNetwork>;

class NeuralNetwork : public QObject
{
    Q_OBJECT
public:
    NeuralNetwork(NNTopologyDataPointer data, QObject *parent = 0);
    ~NeuralNetwork();

    long double processBoard(Checkers::BoardVector &board);

    NeuralNetworkPointer createChild() const;
    static NeuralNetworkPointer createGenrationZeroNN(InternalTopology *topology);

    inline NNTopologyDataPointer getDataStruct() const { return this->data; }

private:
    NNTopologyDataPointer data;

    QList<Layer> hiddenLayers;
    Neuron outputNeuron;
    QList<Weight> outputNeuronWeights;

    InputVector createInputVector(const Checkers::BoardVector &boardVector) const;

};

}

#endif // NEURALNETWORK_H
