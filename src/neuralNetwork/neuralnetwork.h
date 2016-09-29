#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QObject>

class NeuralNetwork : public QObject
{
    Q_OBJECT
public:
    explicit NeuralNetwork(QObject *parent = 0);

signals:

public slots:
};

#endif // NEURALNETWORK_H