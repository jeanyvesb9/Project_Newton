#ifndef BOARD_H
#define BOARD_H

#include <exception>
#include <QObject>
#include <QVector>
#include <QPair>
#include <QSharedPointer>

#include "src/game/gamestructs.h"

namespace Game {

class JumpException {};
class newCellException {};

enum class MoveValidity { Valid, Invalid };

struct BoardModification
{
    CellPointer crownedCell;
    QVector<Cell> removedCells;
};
using BoardModificationPointer = QSharedPointer<BoardModification>;

class Board;
using BoardPointer = QSharedPointer<Board>;

class Board : public QObject
{
    Q_OBJECT
public:
    Board(BoardData data, QObject *parent = 0);

    bool isMoveValid(MovePointer move) const;
    Cell getNewCell(MovePointer move) const;
    QVector<Cell> getRemovedCells(MovePointer move) const;
    QVector<MovePointer> getAllMoves(Cell cell, bool allowSingleJumps = false, bool onlyJumps = false) const; //allowSingleJumps allows for single jump in multiple jump context
    BoardPointer executeMove(MovePointer move) const;
    BoardPointer treeBranchGenerator() const;

    static BoardPointer defaultBoard();

private:
    BoardData data;
    QPair<qint8, qint8> jumpNewCellModifier(MovePointer move) const; //QPair<column, row>
};

}

#endif // BOARD_H
