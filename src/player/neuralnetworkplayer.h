#ifndef NEURALNETWORKPLAYER_H
#define NEURALNETWORKPLAYER_H

#include <QObject>

#include <algorithm>
#include <limits>

#include "src/player/abstractplayer.h"
#include "src/game/board.h"
#include "src/neuralNetwork/neuralnetwork.h"
#include "src/camera/cameraanalyser.h"

class NeuralNetworkPlayer : public AbstractPlayer
{
public:
    explicit NeuralNetworkPlayer(Game::Side side, NN::NeuralNetworkPointer nn, int maxSearchLevel, QObject *parent = 0);

protected:
    void executeTurn() override;
    void backgroundTask() override;

private:
    NN::NeuralNetworkPointer nn;
    int maxSearchLevel;
    Game::Side opSide;

    long double minimax(Game::BoardPointer board, long double alpha, long double beta, int ply, bool maximisingPlayer);
};
using NeuralNetworkPlayerPointer = QSharedPointer<NeuralNetworkPlayer>;

#endif // NEURALNETWORKPLAYER_H
