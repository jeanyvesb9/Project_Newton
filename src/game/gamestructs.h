#ifndef MOVE_H
#define MOVE_H

#include <QVector>
#include <QtGlobal>
#include <QSharedPointer>

namespace Game {

enum class Pieces { OpKing, OpPlayer, Empty, Player, King };
using BoardData = QVector<Pieces>;

struct Cell {
    Cell() = default;
    inline Cell(qint8 column, qint8 row) : column{column}, row{row} {};
    qint8 column;
    qint8 row;
};

enum class DirectionToken { FRight, FLeft, BRight, BLeft, JFRight, JFLeft, JBRight, JBLeft };

struct Move;
using MovePointer = QSharedPointer<Move>;

struct Move {
    Move() = default;
    inline Move(Cell cell, DirectionToken dir, MovePointer concat = MovePointer()) : cell{cell}, direction{dir}, concatenatedMove{concat} {};
    Cell cell;
    DirectionToken direction;
    MovePointer concatenatedMove;
};


}

#endif // MOVE_H
