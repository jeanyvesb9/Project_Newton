#include "neuralnetworkplayer.h"

NeuralNetworkPlayer::NeuralNetworkPlayer(Game::Side side, NN::NeuralNetworkPointer nn, int maxSearchLevel, QObject *parent) : AbstractPlayer(side, parent),
   nn{nn}, maxSearchLevel{maxSearchLevel}
{
    opSide = side == Game::Side::PlayerSide ? Game::Side::OpponentSide : Game::Side::PlayerSide;
}

void NeuralNetworkPlayer::executeTurn()
{
    Game::MovePointer bestMove;
    long double bestValue = std::numeric_limits<long double>::min();
    QVector<Game::MovePointer> moves = board->getAllMoves(side);
    if(moves.isEmpty())
    {
        finishTurn(bestMove);
        return;
    }
    long double alpha = std::numeric_limits<long double>::max();
    for(auto &m : moves)
    {
        Game::BoardPointer nBoard = board->executeMove(m);
        alpha = -minimax(nBoard, std::numeric_limits<long double>::min(), alpha, maxSearchLevel - 1, false);
        if(alpha > bestValue)
        {
            bestValue = alpha;
            bestMove = m;
        }

        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            finishTurn(Game::MovePointer());
        }
        mutex.unlock();
        }

    finishTurn(bestMove);
}

long double NeuralNetworkPlayer::minimax(Game::BoardPointer board, long double alpha, long double beta, int ply, bool maximisingPlayer)
{
    QApplication::processEvents();
    mutex.lock();
    if(hasToStop)
    {
        return 0;
    }
    mutex.unlock();

    Game::Side mvSide = maximisingPlayer ? this->side : this->opSide;
    QVector<Game::MovePointer> moves = board->getAllMoves(mvSide);

    if(ply == 0)
    {
        Game::BoardData bd = board->getBoardData();
        return nn->processBoard(bd);
    }

    for(auto &mv : moves) //might crash due to empty moves vector
    {
        Game::BoardPointer nBoard = board->executeMove(mv);
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

void NeuralNetworkPlayer::backgroundTask()
{

}
