#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "src/game/gamestructs.h"

namespace Game {

class Board;
using BoardPointer = QSharedPointer<Board>;

class Board : public QObject
{
    Q_OBJECT
public:
    Board(QObject *parent = 0);



    static BoardPointer defaultBoard();

private:
    Game::BoardVector boardDiagram;

signals:

public slots:
};

}

#endif // BOARD_H
