#ifndef TRAININGSTRUCTS_H
#define TRAININGSTRUCTS_H

#include <QtGlobal>
#include <QVector>
#include <QSharedPointer>

#include "src/neuralNetwork/neuralnetwork.h"

namespace NN {

struct TrainingGame {
    NNContainerPointer nn1;
    NNContainerPointer nn2;
    quint32 score;
};
using TrainingGamePointer = QSharedPointer<TrainingGame>;

struct TrainingData {
    QVector<NNContainerPointer> newNNs;
    QVector<bool> originalsToDeactivate;
    quint64 trainingTime;
    QVector<TrainingGamePointer> games;
    NNContainerPointer bestPerforming;
};
using TrainingDataPointer = QSharedPointer<TrainingData>;

}

#endif // TRAININGSTRUCTS_H
