#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QThread>

#include "src/player/abstractplayer.h"
#include "src/game/board.h"

struct PlayerMove
{
    unsigned int player;
    Game::MovePointer move;
    Game::BoardModificationPointer boardMdf;
};
using PlayerMovePointer = QSharedPointer<PlayerMove>;

class GameEngine : public QObject
{
    Q_OBJECT
public:
    explicit GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2, QObject *parent = 0);
    explicit GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2,
                        Game::BoardData board, bool startingPlayer, int currentPlayer, int playtime,
                        QObject *parent = 0);

    Game::BoardPointer getBoard() const;
    QVector<PlayerMovePointer> getMoves() const;
    PlayerMovePointer getLastMove() const;

    bool getStartingPlayer() const;
    bool getCurrentPlayer() const;

    int getPlayer1_removed() const;
    int getPlayer1_kings() const;
    int getPlayer2_removed() const;
    int getPlayer2_kings() const;

    int getPlayTime() const;

signals:
    void madeMove(PlayerMovePointer move);

public slots:

private slots:
    void player1_hasFinished(Game::MovePointer move, int time);
    void player2_hasFinished(Game::MovePointer move, int time);

private:
    Game::BoardPointer board;

    AbstractPlayerPointer player1;
    QThread *player1_thread;
    AbstractPlayerPointer player2;
    QThread *player2_thread;

    bool startingPlayer;
    int currentPlayer;

    int playTime;
    QTime time;
};

#endif // GAMEENGINE_H
