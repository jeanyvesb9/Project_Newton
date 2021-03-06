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
#include "src/engine/gameengine.h"
#include "src/game/board.h"
#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/player/neuralnetworkplayer.h"

namespace NN {

enum class Status { Stopped, Running, StopRequested };

class TrainingEngine : public QObject
{
    Q_OBJECT
public:
    TrainingEngine(NeuralNetworkManagerPointer manager, int depth, QObject *parent = 0);

    Status getStatus() const;

signals:
    void statusUpdate(Status statusUpdate, Game::BoardData board, int iteration, int totalIterations, int moves, int maxMoves, int games);
    void beginDBSave();
    void hasFinished(int iterationsCompleted);
    void hasCanceled(int iterationsCompleted);

public slots:
    void startTraining(int iterations, int maxMoves, int searchDepth);
    void cancelTraining();

private slots:
    void madeMove(PlayerMovePointer move, Game::BoardData board);
    void gameWon(Player winner);
    void hasTied();

private:
    Status status;

    int iteration;
    int totalIterations;
    int moves;

    int depth;
    int tieMaxMoves;

    NeuralNetworkManagerPointer manager;
    QVector<NNContainerPointer> *originalNNs;
    TrainingDataPointer trainingData;

    NNContainerPointer player1;
    NNContainerPointer player2;
    GameEnginePointer engine;
    NeuralNetworkPlayer *nnp1;
    NeuralNetworkPlayer *nnp2;

    int i = 0;
    int j = 0;
    QVector<bool> pool;

    QElapsedTimer timer;

    void newIteration();
    void startNewGame();
    void hasFinishedIteration();
    void nextTrain();

    inline int getGames() const { return i * 5 + j + 1; }
};
using TrainingEnginePointer = QSharedPointer<TrainingEngine>;
}

Q_DECLARE_METATYPE(NN::Status)

#endif // TRAININGENGINE_H
