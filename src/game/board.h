#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include "src/game/gamestructs.h"

namespace Game {

enum class MoveValidity { Valid, Invalid };

class Board;
using BoardPointer = QSharedPointer<Board>;

class Board : public QObject
{
    Q_OBJECT
public:
    Board(BoardData data, QObject *parent = 0);

    MoveValidity isMoveValid(MovePointer move) const;
    MovePointer continueJump(MovePointer move) const;
    QVector<MovePointer> getAllMoves(Cell cell, bool allowSingleJumps = false, bool onlyJumps = false) const; //allowSingleJumps allows for single jump in multiple jump context
    BoardPointer executeMove(MovePointer move) const;

    BoardPointer treeBranchGenerator() const;

    static BoardPointer defaultBoard();

private:
    BoardData data;
};

}

#endif // BOARD_H
