#ifndef MOVE_H
#define MOVE_H

#include <QVector>
#include <QtGlobal>

namespace Game {

enum class Places { OpKing, OpPlayer, Empty, Player, King };
using BoardVector = QVector<Places>;

struct Cell {
    inline Cell() : column{-1}, row{-1} {};
    inline Cell(qint8 column, qint8 row) : column{column}, row{row} {};
    qint8 column;
    qint8 row;
};

enum class DirectionToken { FRight, FLeft, BRight, BLeft, JFRight, JFLeft, JBRight, JBLeft };

struct Move {
    Cell cell;
    DirectionToken direction;
    Cell concatenatedMove;
};

}

#endif // MOVE_H
