#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QThread>

#include <random>

#include "src/utilityfunctions.h"
#include "src/player/abstractplayer.h"
#include "src/game/board.h"

enum class Player { None = 0, Player1, Player2 };

struct PlayerMove
{
    Player player;
    Game::MovePointer move;
    qint64 time;
    Game::BoardModificationPointer boardMdf;
};
using PlayerMovePointer = QSharedPointer<PlayerMove>;

class GameEngine : public QObject
{
    Q_OBJECT
public:
    enum TieValue { NoTie = -1 };

    explicit GameEngine(AbstractPlayer *p1, AbstractPlayer *p2, int tie = 100, QObject *parent = 0);
    explicit GameEngine(AbstractPlayer *p1, AbstractPlayer *p2,
                        Game::BoardData board, int turnNumber, Player currentPlayer, quint64 playtime,
                        int tie = TieValue::NoTie, bool startPaused = false,
                        QObject *parent = 0);

    ~GameEngine();

    bool isPaused();

    Game::BoardPointer getBoard() const;
    Game::BoardPointer* getBoardPtr();

    Player getCurrentPlayer() const;

    int getPlayer1_removed() const;
    int getPlayer1_kings() const;
    int getPlayer2_removed() const;
    int getPlayer2_kings() const;

    int getPlayTime() const;
    Player getWinningPlayer() const;

signals:
    void madeMove(PlayerMovePointer move, Game::BoardData board);
    void hasTied();
    void hasWon(Player winningPlayer);
    void hasEnded();
    void oneSecondTimerTick(int time);

public slots:
    void pauseGame();
    void resumeGame();
    void stopGame();

protected:
    void timerEvent(QTimerEvent *event) override;

private slots:
    void player1_hasFinished(Game::MovePointer move, int time);
    void player2_hasFinished(Game::MovePointer move, int time);

private:
    bool paused;

    Game::BoardPointer board;
    int tieValue;
    int turnNumber;

    AbstractPlayer *player1;
    QThread *player1_thread;
    AbstractPlayer *player2;
    QThread *player2_thread;

    Player currentPlayer;

    int timerId;
    quint64 playTime;
};
using GameEnginePointer = QSharedPointer<GameEngine>;

#endif // GAMEENGINE_H
