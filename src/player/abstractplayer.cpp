#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer(Game::Side side, QObject *parent) : QObject(parent),
    hasToStop{false}, side{side}, running{false}, hasToBegin{false}, backgroundTaskStatus{true}
{

}

bool AbstractPlayer::startTurn(Game::BoardData boardData)
{
    mutex.lock();
    board = Game::BoardPointer(new Game::Board(boardData));
    hasToBegin = true;
    time.restart();
    bool r = running;
    mutex.unlock();
    return !r;
}

void AbstractPlayer::launchBackgroundTask()
{
    while(true)
    {
        QApplication::processEvents();
        mutex.lock();
        if(hasToStop)
        {
            mutex.unlock();
            emit hasStopped();
            return;
        }
        if(hasToBegin)
        {
            hasToBegin = false;
            running = true;
            mutex.unlock();
            executeTurn();
            mutex.lock();
            running = false;
        }
        if(backgroundTaskStatus)
        {
            mutex.unlock();
            backgroundTask();
            mutex.lock();
        }
        mutex.unlock();
    }
}

void AbstractPlayer::pauseBackgroundTask()
{
    mutex.lock();
    backgroundTaskStatus = false;
    mutex.unlock();
}

void AbstractPlayer::resumeBackgroundTask()
{
    mutex.lock();
    backgroundTaskStatus = true;
    mutex.unlock();
}

void AbstractPlayer::stop()
{
    mutex.lock();
    hasToStop = true;
    mutex.unlock();
}

void AbstractPlayer::backgroundTask()
{

}

void AbstractPlayer::finishTurn(Game::MovePointer move)
{
    emit finishedTurn(move, time.elapsed());
}
