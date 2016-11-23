#ifndef GAMEFILE_H
#define GAMEFILE_H

#include <QObject>
#include <QSharedPointer>

#include "src/game/gamestructs.h"
#include "src/game/board.h"

namespace Game
{

class GameFile;
using GameFilePointer = QSharedPointer<GameFile>;

class GameFile : public QObject
{
    Q_OBJECT
public:
    explicit GameFile(QObject *parent = 0);
    explicit GameFile(QString file, QObject *parent = 0);

    Game::BoardData getBoardData();

    static GameFilePointer createNewGame(QString file);

private:
    Game::BoardData board;
};

}
#endif // GAMEFILE_H
