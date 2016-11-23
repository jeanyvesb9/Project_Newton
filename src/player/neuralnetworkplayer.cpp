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

    Game::MovePointer move = board->treeBranchGenerator(side);
    if(move.isNull())
    {
        finishTurn(bestMove);
        return;
    }
    long double alpha = std::numeric_limits<long double>::max();
    while(1)
    {
        Game::BoardPointer nBoard = board->executeMove(move);
        alpha = -minimax(nBoard, std::numeric_limits<long double>::lowest(), alpha, maxSearchLevel - 1, false);

        if(alpha > bestValue)
        {
            bestValue = alpha;
            bestMove = move;
        }

        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            mutex.unlock();
            finishTurn(Game::MovePointer());
            return;
        }
        mutex.unlock();

        move = board->treeBranchGenerator(side);
        if(move.isNull())
        {
            break;
        }
    }

    finishTurn(bestMove);
}

long double NeuralNetworkPlayer::minimax(Game::BoardPointer board, long double alpha, long double beta, int ply, bool maximisingPlayer)
{
    QApplication::processEvents();
    mutex.lock();
    if(hasToStop)
    {
        mutex.unlock();
        return 0;
    }
    mutex.unlock();

    Game::Side mvSide = maximisingPlayer ? this->side : this->opSide;

    Game::MovePointer move = board->treeBranchGenerator(mvSide);
    if(ply == 0 || move.isNull())
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

    while(1)
    {
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

        move = board->treeBranchGenerator(mvSide);
        if(move.isNull())
        {
            break;
        }
    }

    return alpha;
}

void NeuralNetworkPlayer::backgroundTask()
{

}
