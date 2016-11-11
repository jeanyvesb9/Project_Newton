#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <random>
#include <cmath>
#include <tgmath.h>

#include <QObject>
#include <QList>
#include <QPair>
#include <QSharedPointer>
#include <QDebug>

#include "src/neuralNetwork/neuron.h"
#include "src/game/gamestructs.h"

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

    long double processBoard(Game::BoardData &board);

    NeuralNetworkPointer createChild() const;

    static NeuralNetworkPointer createGenerationZeroNN(InternalTopology *topology);

    inline NNTopologyDataPointer getDataStruct() const { return this->data; }

private:
    NNTopologyDataPointer data;

    QList<Layer> hiddenLayers;
    Neuron outputNeuron;
    QList<Weight> outputNeuronWeights;

    InputVector createInputVector(const Game::BoardData &boardVector) const;

};

struct NNContainer {
    NeuralNetworkPointer neuralNetwork;
    quint64 id;
    quint64 parent;
    quint64 generation;
    bool active;
    qint64 dateCreated;
};
using NNContainerPointer = QSharedPointer<NNContainer>;

}

#endif // NEURALNETWORK_H
