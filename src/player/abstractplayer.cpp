#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer(Game::Side side, QObject *parent) : QObject(parent),
    hasToStop{false}, side{side}, running{false}, hasToBegin{false}, backgroundTaskStatus{true}, timerId{0}
{

}

AbstractPlayer::~AbstractPlayer()
{
    if(timerId)
    {
        killTimer(timerId);
        timerId = 0;
    }
}

void AbstractPlayer::startTurn(Game::BoardData boardData)
{
    mutex.lock();
    if(!running)
    {
        board = Game::BoardPointer(new Game::Board(boardData));
        hasToBegin = true;
        time = 0;
        timerId = startTimer(1000);
    }
    mutex.unlock();
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
            if(timerId)
            {
                killTimer(timerId);
                timerId = 0;
            }
            moveToThread(qApp->thread());
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
    if(timerId)
    {
        killTimer(timerId);
        timerId = 0;
    }
    emit finishedTurn(move, time);
}

void AbstractPlayer::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    time += 1000;
    emit oneSecondtimerTick(time);
}
