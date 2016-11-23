#ifndef TRAININGSTRUCTS_H
#define TRAININGSTRUCTS_H

#include <QtGlobal>
#include <QVector>
#include <QMap>
#include <QSharedPointer>

#include "src/neuralNetwork/neuralnetwork.h"

namespace NN {

struct TrainingGame {
    NNContainerPointer nn1;
    NNContainerPointer nn2;
    qint32 score;
};
using TrainingGamePointer = QSharedPointer<TrainingGame>;

struct TrainingData {
    TrainingData() { originalsToDeactivate.fill(0, 15); }
    QVector<NNContainerPointer> newNNs;
    QVector<bool> originalsToDeactivate;
    quint64 trainingTime;
    QVector<TrainingGamePointer> games;
    NNContainerPointer bestPerforming;
};
using TrainingDataPointer = QSharedPointer<TrainingData>;

}

Q_DECLARE_METATYPE(NN::TrainingDataPointer)

#endif // TRAININGSTRUCTS_H
