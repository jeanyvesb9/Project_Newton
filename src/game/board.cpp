#include "board.h"

using namespace Game;

Board::Board(BoardData data, QObject *parent)
    : QObject(parent),
      data{data}, treeCellDirectionNumber{-1}
{

}

bool Board::isMoveValid(MovePointer move) const
{
    const Cell &cell = move->cell;
    const quint8 row = move->cell.row;
    const quint8 column = move->cell.column;
    const quint8 cellNum = move->cell.toCellNum();
    const Piece piece = data.at(cellNum);

    if(piece == Piece::Empty)
        return false;

    switch(move->direction)
    {
    case DirectionToken::FLeft:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row != 7)
        {
            if(!(row % 2) && data.at(cellNum + 4) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && column != 3 && data.at(cellNum + 5) == Piece::Empty)
            {
                return true;
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row != 0)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum - 5) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && data.at(cellNum - 4) == Piece::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::FRight:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row != 7)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum + 3) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && data.at(cellNum + 4) == Piece::Empty)
            {
                return true;
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row != 0)
        {
            if(!(row % 2) && data.at(cellNum - 4) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && column != 3 && data.at(cellNum - 3) == Piece::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::BLeft:
        if(piece == Piece::OpKing && row != 0)
        {
            if(!(row % 2) && data.at(cellNum - 4) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && column != 3 && data.at(cellNum - 3) == Piece::Empty)
            {
                return true;
            }
        }
        else if(piece == Piece::King && row != 7)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum + 3) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && data.at(cellNum + 4) == Piece::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::BRight:
        if(piece == Piece::OpKing && row != 0)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum - 5) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && data.at(cellNum - 4) == Piece::Empty)
            {
                return true;
            }
        }
        else if(piece == Piece::King && row != 7)
        {
            if(!(row % 2) && data.at(cellNum + 4) == Piece::Empty)
            {
                return true;
            }
            else if((row % 2) && column != 3 && data.at(cellNum + 5) == Piece::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::JFLeft:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row < 6 && column != 3)
        {
            if(piece == Piece::OpPlayer && getNewCell(move, false).row == 7 && !move->concatenatedMove.isNull())
            {
                return false;
            }

            if(!(row % 2) && (data.at(cellNum + 4) == Piece::Player || data.at(cellNum + 4) == Piece::King)
                    && data.at(cellNum + 9) == Piece::Empty)
            {

                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum + 5) == Piece::Player || data.at(cellNum + 5) == Piece::King)
                    && data.at(cellNum + 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row > 1 && column != 0)
        {
            if(piece == Piece::Player && getNewCell(move, false).row == 0 && !move->concatenatedMove.isNull())
            {
                return false;
            }
            if(!(row % 2) && (data.at(cellNum - 5) == Piece::OpPlayer || data.at(cellNum - 5) == Piece::OpKing)
                    && data.at(cellNum - 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum - 4) == Piece::OpPlayer || data.at(cellNum - 4) == Piece::OpKing)
                    && data.at(cellNum - 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JFRight:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row < 6 && column != 0)
        {
            if(piece == Piece::OpPlayer && getNewCell(move, false).row == 7 && !move->concatenatedMove.isNull())
            {
                return false;
            }

            if(!(row % 2) && (data.at(cellNum + 3) == Piece::Player || data.at(cellNum + 3) == Piece::King)
                    && data.at(cellNum + 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum + 4) == Piece::Player || data.at(cellNum + 4) == Piece::King)
                    && data.at(cellNum + 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row > 1 && column != 3)
        {
            if(piece == Piece::Player && getNewCell(move, false).row == 0 && !move->concatenatedMove.isNull())
            {
                return false;
            }
            if(!(row % 2) && (data.at(cellNum - 4) == Piece::OpPlayer || data.at(cellNum - 4) == Piece::OpKing)
                    && data.at(cellNum - 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum - 3) == Piece::OpPlayer || data.at(cellNum - 3) == Piece::OpKing)
                    && data.at(cellNum - 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JBLeft:
        if(piece == Piece::OpKing && row > 1 && column != 3)
        {
            if(!(row % 2) && (data.at(cellNum - 4) == Piece::Player || data.at(cellNum - 4) == Piece::King)
                    && data.at(cellNum - 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum - 3) == Piece::Player || data.at(cellNum - 3) == Piece::King)
                    && data.at(cellNum - 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if(piece == Piece::King && row < 6 && column != 0)
        {
            if(!(row % 2) && (data.at(cellNum + 3) == Piece::OpPlayer || data.at(cellNum + 3) == Piece::OpKing)
                    && data.at(cellNum + 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum + 4) == Piece::OpPlayer || data.at(cellNum + 4) == Piece::OpKing)
                    && data.at(cellNum + 7) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JBRight:
        if(piece == Piece::OpKing && row > 1 && column != 0)
        {
            if(!(row % 2) && (data.at(cellNum - 5) == Piece::Player || data.at(cellNum - 5) == Piece::King)
                    && data.at(cellNum - 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || move->concatenatedMove->direction == DirectionToken::JBLeft
                        || move->concatenatedMove->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum - 4) == Piece::Player || data.at(cellNum - 4) == Piece::King)
                    && data.at(cellNum - 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || move->concatenatedMove->direction == DirectionToken::JBLeft
                        || move->concatenatedMove->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if(piece == Piece::King && row < 6 && column != 3)
        {
            if(!(row % 2) && (data.at(cellNum + 4) == Piece::OpPlayer || data.at(cellNum + 4) == Piece::OpKing)
                    && data.at(cellNum + 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || move->concatenatedMove->direction == DirectionToken::JBLeft
                        || move->concatenatedMove->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((row % 2) && (data.at(cellNum + 5) == Piece::OpPlayer || data.at(cellNum + 5) == Piece::OpKing)
                    && data.at(cellNum + 9) == Piece::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || move->concatenatedMove->direction == DirectionToken::JBLeft
                        || move->concatenatedMove->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;
    }

    return false;
}

Cell Board::getNewCell(MovePointer move, bool recursive) const
{
    quint8 cellNum = move->cell.toCellNum();
    quint8 row = move->cell.row;
    quint8 column = move->cell.column;
    const Piece piece = data.at(cellNum);

    switch(move->direction)
    {
    case DirectionToken::FLeft:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row != 7)
        {
            if(!(row % 2))
            {
                return Cell(column, row + 1);
            }
            else if(column != 3)
            {
                return Cell(column + 1, row + 1);
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row != 0)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row - 1);
            }
            else if(row % 2)
            {
                return Cell(column, row - 1);
            }
        }
        break;

    case DirectionToken::FRight:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row != 7)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row + 1);
            }
            else if(row % 2)
            {
                return Cell(column, row + 1);
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row != 0)
        {
            if(!(row % 2))
            {
                return Cell(column, row - 1);
            }
            else if(column != 3)
            {
                return Cell(column + 1, row - 1);
            }
        }
        break;

    case DirectionToken::BLeft:
        if(piece == Piece::OpKing && row != 0)
        {
            if(!(row % 2))
            {
                return Cell(column, row - 1);
            }
            else if(column != 3)
            {
                return Cell(column + 1, row - 1);
            }
        }
        else if(piece == Piece::King && row != 7)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row + 1);
            }
            else if(row % 2)
            {
                return Cell(column, row + 1);
            }
        }
        break;

    case DirectionToken::BRight:
        if(piece == Piece::OpKing && row != 0)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row - 1);
            }
            else if(row % 2)
            {
                return Cell(column, row - 1);
            }
        }
        else if(piece == Piece::King && row != 7)
        {
            if(!(row % 2))
            {
                return Cell(column, row + 1);
            }
            else if(column != 3)
            {
                return Cell(column + 1, row + 1);
            }
        }
        break;

    case DirectionToken::JFLeft:
    case DirectionToken::JFRight:
    case DirectionToken::JBLeft:
    case DirectionToken::JBRight:
        try
        {
            QPair<qint8, qint8> jump = jumpNewCellModifier(move, recursive);
            return Cell(column + jump.first, row + jump.second);
        }
        catch (JumpException &e)
        {
            Q_UNUSED(e);
            throw newCellException();
        }
        break;
    }

    throw newCellException();
}

QPair<qint8, qint8> Board::jumpNewCellModifier(MovePointer move, bool recursive) const
{
    quint8 cellNum = move->cell.toCellNum();
    quint8 row = move->cell.row;
    quint8 column = move->cell.column;
    const Piece piece = data.at(cellNum);
    switch(move->direction)
    {
    case DirectionToken::JFLeft:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row < 6 && column != 3)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(1, 2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JFLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(1 + jumpAdder.first, 2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row > 1 && column != 0)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(-1, -2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JFLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(-1 + jumpAdder.first, -2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        break;

    case DirectionToken::JFRight:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row < 6 && column != 0)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(-1, 2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Piece::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JFRight));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(-1 + jumpAdder.first, 2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row > 1 && column != 3)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, quint8>(1, -2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Piece::King && move->concatenatedMove->direction == DirectionToken::JBRight))
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JFRight));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(1 + jumpAdder.first, -2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        break;

    case DirectionToken::JBLeft:
        if(piece == Piece::OpKing && row > 1 && column != 3)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(1, -2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || move->concatenatedMove->direction == DirectionToken::JBLeft
                    || move->concatenatedMove->direction == DirectionToken::JBRight)
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JBLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(1 + jumpAdder.first, -2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        else if(piece == Piece::King && row < 6 && column != 0)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(-1, 2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || move->concatenatedMove->direction == DirectionToken::JBLeft
                    || move->concatenatedMove->direction == DirectionToken::JBRight)
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JBLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(-1 + jumpAdder.first, 2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        break;

    case DirectionToken::JBRight:
        if(piece == Piece::OpKing && row > 1 && column != 0)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(-1, -2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || move->concatenatedMove->direction == DirectionToken::JBLeft
                    || move->concatenatedMove->direction == DirectionToken::JBRight)
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JBRight));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(-1 + jumpAdder.first, -2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        else if(piece == Piece::King && row < 6 && column != 3)
        {
            if(move->concatenatedMove.isNull() || !recursive)
            {
                return QPair<qint8, qint8>(1, 2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || move->concatenatedMove->direction == DirectionToken::JBLeft
                    || move->concatenatedMove->direction == DirectionToken::JBRight)
            {
                MovePointer newMove(new Move(move->cell, DirectionToken::JBRight));
                BoardPointer childBoard = this->executeMove(newMove);
                try
                {
                    QPair<qint8, qint8> jumpAdder = childBoard->jumpNewCellModifier(move->concatenatedMove);
                    return QPair<qint8, qint8>(1 + jumpAdder.first, 2 + jumpAdder.second);
                }
                catch (JumpException &e)
                {
                    Q_UNUSED(e);
                    throw;
                }
            }
        }
        break;

    default:
        throw JumpException();
        break;
    }

    throw JumpException();
}

QVector<Cell> Board::getRemovedCells(MovePointer move) const
{
    quint8 cellNum = move->cell.toCellNum();
    quint8 column = move->cell.column;
    quint8 row = move->cell.row;
    Piece piece = this->data.at(cellNum);

    switch(move->direction)
    {
    case DirectionToken::JFLeft:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row < 6 && column != 3)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 5)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row > 1 && column != 0)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 5)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        break;

    case DirectionToken::JFRight:
        if((piece == Piece::OpPlayer || piece == Piece::OpKing) && row < 6 && column != 0)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 3)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        else if((piece == Piece::Player || piece == Piece::King) && row > 1 && column != 3)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 3)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JFRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        break;

    case DirectionToken::JBLeft:
        if(piece == Piece::OpKing && row > 1 && column != 3)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 3)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        else if(piece == Piece::King && row < 6 && column != 0)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 3)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBLeft));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        break;

    case DirectionToken::JBRight:
        if(piece == Piece::OpKing && row > 1 && column != 0)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 5)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum - 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        else if(piece == Piece::King && row < 6 && column != 3)
        {
            if(!(row % 2))
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 4)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
            else
            {
                QVector<Cell> ret = {Cell::fromNum(cellNum + 5)};
                if(!move->concatenatedMove.isNull())
                {
                    MovePointer nm(new Move(move->cell, DirectionToken::JBRight));
                    ret << this->executeMove(nm)->getRemovedCells(move->concatenatedMove);
                }
                return ret;
            }
        }
        break;

    default:
        return QVector<Cell>();
        break;
    }

    return QVector<Cell>();
}

QVector<MovePointer> Board::getAllMovesForCell(Cell cell, bool onlyJumps) const
{
    quint8 cellNum = cell.toCellNum();
    Piece piece = data.at(cellNum);
    QVector<MovePointer> moves;

    if(piece == Piece::Empty)
        return moves;

    if(!onlyJumps)
    {
        moves <<getCompleteMove(cell, DirectionToken::FLeft);
        moves <<getCompleteMove(cell, DirectionToken::FRight);
        moves <<getCompleteMove(cell, DirectionToken::BLeft);
        moves <<getCompleteMove(cell, DirectionToken::BRight);
    }

    moves <<getCompleteMove(cell, DirectionToken::JFLeft);
    moves <<getCompleteMove(cell, DirectionToken::JFRight);
    moves <<getCompleteMove(cell, DirectionToken::JBLeft);
    moves <<getCompleteMove(cell, DirectionToken::JBRight);

    return moves;
}

QVector<MovePointer> Board::getCompleteMove(Cell cell, DirectionToken direction) const
{
    quint8 cellNum = cell.toCellNum();
    Piece piece = data.at(cellNum);
    QVector<MovePointer> moves;

    if(piece == Piece::Empty)
        return moves;

    MovePointer move;
    switch(direction)
    {
    case DirectionToken::FLeft:
        move.reset(new Move(cell, DirectionToken::FLeft));
        if(isMoveValid(move))
        {
            moves << move;
        }
        break;
    case DirectionToken::FRight:
        move.reset(new Move(cell, DirectionToken::FRight));
        if(isMoveValid(move))
        {
            moves << move;
        }
        break;
    case DirectionToken::BLeft:
        move.reset(new Move(cell, DirectionToken::BLeft));
        if(isMoveValid(move))
        {
            moves << move;
        }
        break;
    case DirectionToken::BRight:
        move.reset(new Move(cell, DirectionToken::BRight));
        if(isMoveValid(move))
        {
            moves << move;
        }
        break;
    case DirectionToken::JFLeft:
        move.reset(new Move(cell, DirectionToken::JFLeft));
        if(isMoveValid(move))
        {
            BoardModificationPointer bmp = BoardModificationPointer(new BoardModification());
            BoardPointer newBoard = this->executeMove(move, bmp);
            if(bmp->newCrownedCell.isNull())
            {
                QVector<MovePointer> nm = newBoard->getAllMovesForCell(this->getNewCell(move), true);
                if(nm.empty())
                {
                    moves << move;
                }
                else
                {
                    for(auto &m : nm)
                    {
                        moves << MovePointer(new Move(cell, DirectionToken::JFLeft, m));
                    }
                }
            }
            else
            {
                moves << move;
            }
        }
        break;
    case DirectionToken::JFRight:
        move.reset(new Move(cell, DirectionToken::JFRight));
        if(isMoveValid(move))
        {
            BoardModificationPointer bmp = BoardModificationPointer(new BoardModification());
            BoardPointer newBoard = this->executeMove(move, bmp);
            if(bmp->newCrownedCell.isNull())
            {
                QVector<MovePointer> nm = newBoard->getAllMovesForCell(this->getNewCell(move), true);
                if(nm.empty())
                {
                    moves << move;
                }
                else
                {
                    for(auto &m : nm)
                    {
                        moves << MovePointer(new Move(cell, DirectionToken::JFRight, m));
                    }
                }
            }
            else
            {
                moves << move;
            }
        }
        break;
    case DirectionToken::JBLeft:
        move.reset(new Move(cell, DirectionToken::JBLeft));
        if(isMoveValid(move))
        {
            BoardPointer newBoard = this->executeMove(move);
            QVector<MovePointer> nm = newBoard->getAllMovesForCell(this->getNewCell(move), true);
            if(nm.empty())
            {
                moves << move;
            }
            else
            {
                for(auto &m : nm)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::JBLeft, m));
                }
            }
        }
        break;
    case DirectionToken::JBRight:
        move.reset(new Move(cell, DirectionToken::JBRight));
        if(isMoveValid(move))
        {
            BoardPointer newBoard = this->executeMove(move);
            QVector<MovePointer> nm = newBoard->getAllMovesForCell(this->getNewCell(move), true);
            if(nm.empty())
            {
                moves << move;
            }
            else
            {
                for(auto &m : nm)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::JBRight, m));
                }
            }
        }
        break;
    }

    return moves;
}

BoardPointer Board::executeMove(MovePointer move, BoardModificationPointer mdf) const
{
    if(!this->isMoveValid(move))
    {
        return BoardPointer();
    }

    BoardData nbd = this->data;
    Cell nCell = getNewCell(move);
    switch(move->direction)
    {
    case DirectionToken::FLeft:
    case DirectionToken::FRight:
    case DirectionToken::BLeft:
    case DirectionToken::BRight:
        nbd[nCell.toCellNum()] = nbd[move->cell.toCellNum()];
        nbd[move->cell.toCellNum()] = Piece::Empty;
        break;
    case DirectionToken::JFLeft:
    case DirectionToken::JFRight:
    case DirectionToken::JBLeft:
    case DirectionToken::JBRight:
        nbd[nCell.toCellNum()] = nbd[move->cell.toCellNum()];
        nbd[move->cell.toCellNum()] = Piece::Empty;
        for(Cell &cell : this->getRemovedCells(move))
        {
            nbd[cell.toCellNum()] = Piece::Empty;
            if(!mdf.isNull())
            {
                mdf->removedCells.append(cell);
            }
        }
        break;
    }

    Piece piece = data.at(move->cell.toCellNum());
    if((piece == Piece::Player || piece == Piece::OpPlayer)
            && (nCell.row == 0 || nCell.row == 7))
    {
        if(!mdf.isNull())
        {
            mdf->newCrownedCell = CellPointer(new Cell(nCell));
        }
        nbd[nCell.toCellNum()] = piece == Piece::Player ? Piece::King : Piece::OpKing;
    }
    return BoardPointer(new Board(nbd));
}

QVector<MovePointer> Board::getAllMoves(Side side) const
{
    QVector<MovePointer> ret;
    for(int i = 0; i < 32; i++)
    {
        Cell cell = Cell::fromNum(i);
        if((side == Side::PlayerSide || side == Side::Both) && (data.at(cell.toCellNum()) == Piece::Player || data.at(cell.toCellNum()) == Piece::King))
        {
            ret << getAllMovesForCell(cell);
        }
        else if ((side == Side::OpponentSide || side == Side::Both) && (data.at(cell.toCellNum()) == Piece::OpPlayer || data.at(cell.toCellNum()) == Piece::OpKing))
        {
            ret << getAllMovesForCell(cell);
        }
    }
    return ret;
}

BoardPointer Board::treeBranchGenerator()
{
    constexpr static DirectionToken directions[8] = { DirectionToken::FLeft, DirectionToken::FRight,
                                            DirectionToken::BLeft, DirectionToken::BRight,
                                            DirectionToken::JFLeft, DirectionToken::JFRight,
                                            DirectionToken::JBLeft, DirectionToken::JBRight };
    while(1)
    {
        if(treeJumpBuffer.isEmpty() && treeCellDirectionNumber != 256)
        {
            treeCellDirectionNumber++;
        }
        if(treeCellDirectionNumber == 256)
        {
            break;
        }
        int directionNumber = treeCellDirectionNumber % 8;
        Cell cell = Cell::fromNum(treeCellDirectionNumber / 8);
        //qDebug() <<treeCellDirectionNumber <<" " <<cell.column <<" " <<cell.row;
        if(directionNumber < 4)
        {
            QVector<MovePointer> moves = getCompleteMove(cell, directions[directionNumber]);
            if(!moves.isEmpty())
            {
                return executeMove(moves.at(0));
            }
        }
        else
        {
            if(treeJumpBuffer.isEmpty())
            {
                treeJumpBuffer = getCompleteMove(cell, directions[directionNumber]);
            }
            else
            {
                MovePointer move = treeJumpBuffer.first();
                treeJumpBuffer.removeFirst();
                return executeMove(move);
            }
        }
    }
    return BoardPointer();
}

void Board::resetTreeBranchGenerator()
{
    treeCellDirectionNumber = -1;
    treeJumpBuffer.clear();
}

QString Board::getBoardString() const
{
    std::wstring ret;
    ret.append(L"\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\n");
    for(int i = 0; i < 8; i++)
    {
        ret.append(L"\u2588");
        if(i % 2 == 1)
        {
            ret.append(L"#");
        }
        for(int j = 0; j < 4; j++)
        {
            switch(this->data.at(i * 4 + j))
            {
            case Piece::Empty:
                ret.append(L" ");
                break;

            case Piece::Player:
                ret.append(L"X");
                break;

            case Piece::King:
                ret.append(L"K");
                break;

            case Piece::OpPlayer:
                ret.append(L"O");
                break;

            case Piece::OpKing:
                ret.append(L"H");
                break;
            }
            if(!(i % 2 == 1 && j == 3))
            {
                ret.append(L"#");
            }
        }
        ret.append(L"\u2588\n");
    }
    ret.append(L"\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588\u2588");
    return QString::fromWCharArray(ret.data());
}

BoardPointer Board::defaultBoard()
{
    BoardData data;
    for(int i = 0; i < 12; i++)
        data.append(Piece::OpPlayer);
    for(int i = 0; i < 8; i++)
        data.append(Piece::Empty);
    for(int i = 0; i < 12; i++)
        data.append(Piece::Player);
    return BoardPointer(new Board(data));
}
