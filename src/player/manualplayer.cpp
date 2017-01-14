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
        wasWrongMove = true;
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
    wasWrongMove = false;
    possibleMoves = board->getAllMoves(side);
    while(true)
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
            if(hasFinished)
            {
                finishTurn(move);
                mutex.unlock();
                return;
            }
            else if(wasWrongMove)
            {
                emit wrongMove();
            }
        }

        mutex.unlock();
    }
}

void ManualPlayer::backgroundTask()
{

}
