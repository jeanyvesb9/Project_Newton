#include "neuralnetworkplayer.h"

NeuralNetworkPlayer::NeuralNetworkPlayer(Game::Side side, NN::NeuralNetworkPointer nn, int maxSearchLevel, QObject *parent) : AbstractPlayer(side, parent),
   nn{nn}, maxSearchLevel{maxSearchLevel}
{
    opSide = side == Game::Side::PlayerSide ? Game::Side::OpponentSide : Game::Side::PlayerSide;
}

void NeuralNetworkPlayer::executeTurn()
{
    Game::MovePointer bestMove;
    long double bestValue = std::numeric_limits<long double>::lowest();

    QVector<Game::MovePointer> moves = board->getAllMoves(side);
    if(moves.isEmpty())
    {
        finishTurn(bestMove);
        return;
    }
    long double alpha = std::numeric_limits<long double>::max();
    for(auto &move :moves)
    {
        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            mutex.unlock();
            return;
        }
        mutex.unlock();

        Game::BoardPointer nBoard = board->executeMove(move);
        alpha = -minimax(nBoard, std::numeric_limits<long double>::lowest(), alpha, maxSearchLevel - 1, false);

        if(alpha > bestValue)
        {
            bestValue = alpha;
            bestMove = move;
        }
    }
    pauseLoop([this, &bestMove](){ finishTurn(bestMove); });
}

long double NeuralNetworkPlayer::minimax(Game::BoardPointer board, long double alpha, long double beta, int ply, bool maximisingPlayer)
{
    Game::Side mvSide = maximisingPlayer ? this->side : this->opSide;
    QVector<Game::MovePointer> moves = board->getAllMoves(mvSide);
    if(ply == 0 || moves.isEmpty())
    {
        Game::BoardData bd = board->getBoardData();
        if(side == Game::Side::PlayerSide)
        {
            return maximisingPlayer ? nn->processBoard(bd) : -nn->processBoard(bd);
        }
        else
        {
            return maximisingPlayer ? -nn->processBoard(bd) : nn->processBoard(bd);
        }
    }
    for(auto &move : moves)
    {
        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            mutex.unlock();
            return 0;
        }
        mutex.unlock();
        pauseLoop();

        Game::BoardPointer nBoard = board->executeMove(move);
        long double currentEval = -minimax(nBoard, -beta, -alpha, ply - 1, !maximisingPlayer);

        if(currentEval >= beta)
        {
            return beta;
        }
        if(currentEval > alpha)
        {
            alpha = currentEval;
        }
    }

    return alpha;
}

void NeuralNetworkPlayer::pauseLoop(std::function<void ()> lambda)
{
    while(1)
    {
        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            mutex.unlock();
            return;
        }
        updatePaused();
        if(!paused)
        {
            mutex.unlock();
            lambda();
            return;
        }
        mutex.unlock();
    }
}
