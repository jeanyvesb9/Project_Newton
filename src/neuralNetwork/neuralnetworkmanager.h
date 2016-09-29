#ifndef NEURALNETWORKMANAGER_H
#define NEURALNETWORKMANAGER_H

#include <QObject>

#include "src/neuralNetwork/neuralnetwork.h"

class NeuralNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NeuralNetworkManager(QObject *parent = 0);

signals:

public slots:
};

#endif // NEURALNETWORKMANAGER_H
