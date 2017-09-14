#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QObject>
#include <QApplication>
#include <QSharedPointer>
#include <QMutex>
#include <QTime>

#include "src/game/board.h"

class AbstractPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractPlayer(Game::Side side, QObject *parent = 0);
    ~AbstractPlayer();

    inline Game::Side getSide() const { return side; }
    bool isExecuting();
    bool isPaused();

signals:
    void finishedTurn(Game::MovePointer move, int time);
    void hasStopped();
    void oneSecondtimerTick(int time);

public slots:
    Q_INVOKABLE void startTurn(Game::BoardData boardData, bool pause);
    Q_INVOKABLE void startTurn(Game::BoardData boardData);

    void launchBackgroundTask();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();

    Q_INVOKABLE void stop();

protected:
    QMutex mutex;
    bool hasToStop;
    bool paused;

    Game::BoardPointer board;
    Game::Side side;

    virtual void executeTurn() = 0;
    virtual void backgroundTask(); //HAS TO BE ATOMIC!!

    void updatePaused();
    void finishTurn(Game::MovePointer move);

    void timerEvent(QTimerEvent *event) override;

private:
    bool executing;
    bool hasToBegin;
    bool hasToPause;
    bool hasToResume;

    int timerId;
    quint64 time;
};
using AbstractPlayerPointer = QSharedPointer<AbstractPlayer>;

#endif // ABSTRACTPLAYER_H
