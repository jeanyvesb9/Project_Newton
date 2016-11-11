#include "manualplayer.h"

ManualPlayer::ManualPlayer(Game::Side side, QObject *parent) : AbstractPlayer(side, parent)
{

}

void ManualPlayer::validateTurn(QVector<BoardPiece> rawBoardData)
{
    mutex.lock();
    Game::MovePointer move;
    for(auto &m : possibleMoves)
    {
        if(convertBoardToBoardPieceVector(m) == rawBoardData)
        {
            move = m;
        }
    }
    if(move.isNull())
    {
        emit wrongMove();
    }
    else
    {
        finishTurn(move);
    }
    mutex.unlock();
}

void ManualPlayer::executeTurn()
{
    possibleMoves = board->getAllMoves(side);
    while(true)
    {
        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            emit hasStopped();
            return;
        }
        mutex.unlock();
    }
}

void ManualPlayer::backgroundTask()
{

}
