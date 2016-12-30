#ifndef GAMEFILE_H
#define GAMEFILE_H

#include <random>

#include <QObject>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "src/engine/gameengine.h"
#include "src/game/gamestructs.h"
#include "src/game/board.h"

namespace Game
{

enum class GameStatus { Playing = 0, Player1Won, Player2Won };

class GameFile;
using GameFilePointer = QSharedPointer<GameFile>;

class GameFile
{
    GameFile();
public:
    GameFile(QString file);
    ~GameFile();

    bool saveAndClose();

    void finishGame(Player winningPlayer);
    bool hasFinished() const;
    Player getWinnerPlayer() const;

    QString getPlayerName() const;
    quint64 getDateCreated() const;
    quint8 getDifficulty() const;

    void addPlayTime(quint64 time);
    void setPlayTime(quint64 time);
    quint64 getPlayTime() const;

    Game::BoardData getBoardData() const;
    void addMove(PlayerMovePointer move, BoardData newBoard);
    QVector<PlayerMovePointer> getMoves() const;
    quint64 getCurrentMoveNumber() const;
    Player getActivePlayer() const;

    inline bool isValid() const { return valid; }

    static GameFilePointer createNewGame(QString file, QString name, quint8 difficulty);

private:
    QSqlDatabase db;
    bool valid;

    GameStatus gameStatus;
    QString playerName;
    qint64 dateCreated;
    qint64 gameTime;
    quint8 difficulty;
    Player activePlayer;
    Game::BoardData board;
    quint64 currentMoveNumber;

    bool loadData();

    Game::MovePointer __getMove(quint64 id) const;
    quint64 __addMove(Game::MovePointer move) const;

    static Game::BoardData stringToBoardData(QString raw);
    static QString boardDataToString(Game::BoardData board);
};

}
#endif // GAMEFILE_H
