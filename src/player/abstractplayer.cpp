#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer(Game::Side side, QObject *parent) : QObject(parent),
    hasToStop{false}, paused{false}, side{side}, executing{false}, hasToBegin{false}, hasToPause{false}, hasToResume{false}, timerId{0}
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

bool AbstractPlayer::isExecuting()
{
    return executing;
}

bool AbstractPlayer::isPaused()
{
    return paused;
}

void AbstractPlayer::startTurn(Game::BoardData boardData)
{
    mutex.lock();
    if(!executing)
    {
        board = Game::BoardPointer(new Game::Board(boardData));
        hasToBegin = true;
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
        if(hasToPause)
        {
            hasToPause = false;
            paused = true;
        }
        if(hasToResume)
        {
            hasToResume = false;
            paused = false;
        }
        if(!paused)
        {
            if(hasToBegin)
            {
                hasToBegin = false;
                executing = true;
                time = 0;
                timerId = startTimer(1000);
                mutex.unlock();
                executeTurn();
                mutex.lock();
                executing = false;
            }
            mutex.unlock();
            backgroundTask();
            mutex.lock();
        }

        mutex.unlock();
    }
}

void AbstractPlayer::pause()
{
    mutex.lock();
    hasToPause = true;
    mutex.unlock();
}

void AbstractPlayer::resume()
{
    mutex.lock();
    hasToResume = true;
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

void AbstractPlayer::updatePaused()
{
    if(hasToPause)
    {
        if(timerId)
        {
            killTimer(timerId);
            timerId = 0;
        }
        paused = true;
    }
    if(hasToResume)
    {
        if(!timerId)
        {
            timerId = startTimer(1000);
        }
        paused = false;
    }
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
