#ifndef NEURALNETWORKENGINE_H
#define NEURALNETWORKENGINE_H

#include <QObject>
#include "src/engine/abstractengine.h"

class NeuralNetworkEngine : public AbstractEngine
{
    Q_OBJECT
public:
    NeuralNetworkEngine();
};

#endif // NEURALNETWORKENGINE_H
