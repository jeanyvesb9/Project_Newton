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
    inline Cell(quint8 column, quint8 row) : column{column}, row{row} {}
    quint8 column;
    quint8 row;
    inline quint8 getCellNum() const { return column + row * 4; }
    inline static Cell fromNum(quint8 cellNum) { return Cell(cellNum % 4, cellNum / 8); }
};

enum class DirectionToken { FRight, FLeft, BRight, BLeft, JFRight, JFLeft, JBRight, JBLeft };

struct Move;
using MovePointer = QSharedPointer<Move>;

struct Move {
    Move() = default;
    inline Move(Cell cell, DirectionToken dir, MovePointer concat = MovePointer()) : cell{cell}, direction{dir}, concatenatedMove{concat} {}
    Cell cell;
    DirectionToken direction;
    MovePointer concatenatedMove;
};


}

#endif // MOVE_H
