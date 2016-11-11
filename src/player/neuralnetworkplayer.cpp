#include "neuralnetworkplayer.h"

NeuralNetworkPlayer::NeuralNetworkPlayer(Game::Side side, NN::NeuralNetworkPointer nn, int maxSearchLevel, QObject *parent) : AbstractPlayer(side, parent),
   nn{nn}, maxSearchLevel{maxSearchLevel}
{

}

void NeuralNetworkPlayer::executeTurn()
{
    Game::MovePointer bestMove;
    Game::BoardPointer nBoard = board->treeBranchGenerator();
    if(nBoard.isNull())
    {
        finishTurn(bestMove);
        return;
    }
    long double bestValue;
    while(1)
    {
        Game::BoardData bd = nBoard->getBoardData();
        long double childValue = minimax(nBoard, bestValue, 0, false, maxSearchLevel);
        bestValue = std::max(bestValue, childValue);
        if(beta <= bestValue)
        {
            break;
        }
        nBoard = board->treeBranchGenerator();
        if(nBoard.isNull())
        {
            break;
        }
    }

    QApplication::processEvents();
    mutex.lock();
    if(hasToStop)
    {
        emit hasStopped();
        return;
    }
    mutex.unlock();
}

void NeuralNetworkPlayer::backgroundTask()
{

}

long double NeuralNetworkPlayer::minimax(Game::BoardPointer board, long double alpha, long double beta, bool maximisingPlayer, int maxLevel, int level)
{
    QApplication::processEvents();
    mutex.lock();
    if(hasToStop)
    {
        return 0;
    }
    mutex.unlock();

    long double bestValue;
    Game::BoardPointer nBoard = board->treeBranchGenerator();
    if(nBoard.isNull() || level == maxLevel)
    {
        Game::BoardData bd = board->getBoardData();
        bestValue = nn->processBoard(bd);
    }
    else if(maximisingPlayer)
    {
        bestValue = alpha;
        while(1)
        {
            Game::BoardData bd = nBoard->getBoardData();
            long double childValue = minimax(nBoard, bestValue, beta, false, maxLevel, level + 1);
            bestValue = std::max(bestValue, childValue);
            if(beta <= bestValue)
            {
                break;
            }
            nBoard = board->treeBranchGenerator();
            if(nBoard.isNull())
            {
                break;
            }
        }
    }
    else
    {
        bestValue = beta;
        while(1)
        {
            Game::BoardData bd = nBoard->getBoardData();
            long double childValue = minimax(nBoard, alpha, bestValue, true, maxLevel, level + 1);
            bestValue = std::min(bestValue, childValue);
            if(bestValue <= alpha)
            {
                break;
            }
            nBoard = board->treeBranchGenerator();
            if(nBoard.isNull())
            {
                break;
            }
        }

    }
    return bestValue;
}
