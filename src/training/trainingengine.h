#ifndef TRAININGENGINE_H
#define TRAININGENGINE_H

#include <cstdlib>
#include <QObject>
#include <QGuiApplication>
#include <QVector>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMutex>

#include "src/training/trainingstructs.h"
#include "src/neuralNetwork/neuralnetworkmanager.h"

namespace NN {

class TrainingEngine : public QObject
{
    Q_OBJECT
public:
    TrainingEngine(NeuralNetworkManagerPointer manager, QObject *parent = 0);

signals:
    void status(unsigned int percentage);
    void hasCanceled();
    void finished(TrainingDataPointer ptr);

public slots:
    void startTraining();
    void cancelTraining();

private:
    QMutex mutex;
    bool state;

    QVector<NNContainerPointer> *originalNNs;
    TrainingDataPointer trainingData;

    QElapsedTimer timer;

    void work();
};

}

#endif // TRAININGENGINE_H
