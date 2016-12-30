#ifndef BOARD_H
#define BOARD_H

#include <exception>
#include <string>
#include <QObject>
#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include <QDebug>

#include "src/game/gamestructs.h"

namespace Game {

class JumpException {};
class newCellException {};

enum class MoveValidity { Valid, Invalid };

struct BoardModification
{
    CellPointer newCrownedCell;
    QVector<Cell> removedCells;
};
using BoardModificationPointer = QSharedPointer<BoardModification>;

class Board;
using BoardPointer = QSharedPointer<Board>;

class Board
{
public:
    Board(BoardData data);

    Cell getNewCell(MovePointer move, bool recursive = true) const;
    QVector<Cell> getRemovedCells(MovePointer move) const;
    QVector<MovePointer> getAllMovesForCell(Cell cell, bool onlyJumps = false) const;
    QVector<MovePointer> getCompleteMove(Cell cell, DirectionToken direction) const;
    QVector<Cell> getMovePathCells(MovePointer move) const;
    BoardPointer executeMove(MovePointer move, BoardModificationPointer mdf = BoardModificationPointer()) const;
    QVector<MovePointer> getAllMoves(Side side = Side::Both) const;

    inline BoardData getBoardData() const { return data; }

    QString toBoardString() const;

    static BoardPointer defaultBoard();

private:
    BoardData data;

    QPair<qint8, qint8> jumpNewCellModifier(MovePointer move, bool recursive = true) const; //QPair<column, row>
    bool isMoveValid(MovePointer move) const;

    QVector<Cell> __getMovePathCells_recursive(MovePointer move, BoardPointer board) const;
};

}

Q_DECLARE_METATYPE(Game::BoardPointer);

#endif // BOARD_H
