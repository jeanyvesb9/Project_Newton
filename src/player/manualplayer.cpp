#include "manualplayer.h"

ManualPlayer::ManualPlayer(Game::Side side, QObject *parent) : AbstractPlayer(side, parent)
{

}

void ManualPlayer::validateTurn(QVector<BoardPiece> rawBoardData)
{
    mutex.lock();
    move.reset();
    for(auto &m : possibleMoves)
    {
        if(convertBoardToBoardPieceVector(board->executeMove(m)) == rawBoardData)
        {
            move = m;
            break;
        }
    }
    if(move.isNull())
    {
        emit wrongMove();
    }
    else
    {
        hasFinished = true;
    }
    mutex.unlock();
}

void ManualPlayer::executeTurn()
{
    hasFinished = false;
    possibleMoves = board->getAllMoves(side);
    while(true)
    {
        QApplication::processEvents();
        mutex.lock();
        if(hasFinished)
        {
            finishTurn(move);
            mutex.unlock();
            return;
        }
        if(hasToStop)
        {
            mutex.unlock();
            return;
        }
        mutex.unlock();
    }
}

void ManualPlayer::backgroundTask()
{

}
