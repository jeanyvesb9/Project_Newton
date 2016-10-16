#include "board.h"

using namespace Game;

Board::Board(BoardData data, QObject *parent)
    : QObject(parent),
      data{data}
{

}

bool Board::isMoveValid(MovePointer move) const
{
    const Cell &cell = move->cell;
    const quint8 row = move->cell.row;
    const quint8 column = move->cell.column;
    const quint8 cellNum = move->cell.getCellNum();
    const Pieces piece = data.at(cellNum);

    if(piece == Pieces::Empty)
        return false;

    switch(move->direction)
    {
    case DirectionToken::FLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row != 7)
        {
            if(!(row % 2) && data.at(cellNum + 4) == Pieces::Empty)
            {
                return true;
            }
            else if(column != 3 && data.at(cellNum + 5) == Pieces::Empty)
            {
                return true;
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && row != 0)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum - 5) == Pieces::Empty)
            {
                return true;
            }
            else if(data.at(cellNum - 4) == Pieces::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::FRight:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row != 7)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum + 3) == Pieces::Empty)
            {
                return true;
            }
            else if(data.at(cellNum + 4) == Pieces::Empty)
            {
                return true;
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && row != 0)
        {
            if(!(row % 2) && data.at(cellNum - 4) == Pieces::Empty)
            {
                return true;
            }
            else if(column != 3 && data.at(cellNum - 3) == Pieces::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::BLeft:
        if(piece == Pieces::OpKing && row != 0)
        {
            if(!(row % 2) && data.at(cellNum - 4) == Pieces::Empty)
            {
                return true;
            }
            else if(column != 3 && data.at(cellNum - 3) == Pieces::Empty)
            {
                return true;
            }
        }
        else if(piece == Pieces::King && row != 7)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum + 3) == Pieces::Empty)
            {
                return true;
            }
            else if(data.at(cellNum + 4) == Pieces::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::BRight:
        if(piece == Pieces::OpKing && row != 0)
        {
            if(!(row % 2) && column != 0 && data.at(cellNum - 5) == Pieces::Empty)
            {
                return true;
            }
            else if(data.at(cellNum - 4) == Pieces::Empty)
            {
                return true;
            }
        }
        else if(piece == Pieces::King && row != 7)
        {
            if(!(row % 2) && data.at(cellNum + 4) == Pieces::Empty)
            {
                return true;
            }
            else if(column != 3 && data.at(cellNum + 5) == Pieces::Empty)
            {
                return true;
            }
        }
        break;

    case DirectionToken::JFLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row < 6 && column != 3)
        {
            if(!(row % 2) && (data.at(cellNum + 4) == Pieces::Player || data.at(cellNum + 4) == Pieces::King)
                    && data.at(cellNum + 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((data.at(cellNum + 5) == Pieces::Player || data.at(cellNum + 5) == Pieces::King)
                    && data.at(cellNum + 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && row > 1 && column != 0)
        {
            if(!(row % 2) && (data.at(cellNum - 5) == Pieces::OpPlayer || data.at(cellNum - 5) == Pieces::OpKing)
                    && data.at(cellNum - 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((data.at(cellNum - 4) == Pieces::OpPlayer || data.at(cellNum - 4) == Pieces::OpKing)
                    && data.at(cellNum - 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JFRight:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row < 6 && column != 0)
        {
            if(!(row % 2) && (data.at(cellNum + 3) == Pieces::Player || data.at(cellNum + 3) == Pieces::King)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((data.at(cellNum + 4) == Pieces::Player || data.at(cellNum + 4) == Pieces::King)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && row > 1 && column != 3)
        {
            if(!(row % 2) && (data.at(cellNum - 4) == Pieces::OpPlayer || data.at(cellNum - 4) == Pieces::OpKing)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((data.at(cellNum - 3) == Pieces::OpPlayer || data.at(cellNum - 3) == Pieces::OpKing)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JBLeft:
        if(piece == Pieces::OpKing && row > 1 && column != 3)
        {
            if(!(row % 2) && (data.at(cellNum - 4) == Pieces::Player || data.at(cellNum - 4) == Pieces::King)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((data.at(cellNum - 3) == Pieces::Player || data.at(cellNum - 3) == Pieces::King)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if(piece == Pieces::King && row < 6 && column != 0)
        {
            if(!(row % 2) && (data.at(cellNum + 3) == Pieces::OpPlayer || data.at(cellNum + 3) == Pieces::OpKing)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
            else if((data.at(cellNum + 4) == Pieces::OpPlayer || data.at(cellNum + 4) == Pieces::OpKing)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return true;
                }
                else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                        || move->concatenatedMove->direction == DirectionToken::JFRight
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                        || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JBRight:
        if(piece == Pieces::OpKing && row > 1 && column != 0)
        {
            if(!(row % 2) && (data.at(cellNum - 5) == Pieces::Player || data.at(cellNum - 5) == Pieces::King)
                    && data.at(cellNum - 9) == Pieces::Empty)
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
            else if((data.at(cellNum - 4) == Pieces::Player || data.at(cellNum - 4) == Pieces::King)
                    && data.at(cellNum - 9) == Pieces::Empty)
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
        else if(piece == Pieces::King && row < 6 && column != 3)
        {
            if(!(row % 2) && (data.at(cellNum + 4) == Pieces::OpPlayer || data.at(cellNum + 4) == Pieces::OpKing)
                    && data.at(cellNum + 9) == Pieces::Empty)
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
            else if((data.at(cellNum + 5) == Pieces::OpPlayer || data.at(cellNum + 5) == Pieces::OpKing)
                    && data.at(cellNum + 9) == Pieces::Empty)
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

Cell Board::getNewCell(MovePointer move) const
{
    quint8 cellNum = move->cell.getCellNum();
    quint8 row = move->cell.row;
    quint8 column = move->cell.column;
    const Pieces piece = data.at(cellNum);

    switch(move->direction)
    {
    case DirectionToken::FLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row != 7)
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
        else if((piece == Pieces::Player || piece == Pieces::King) && row != 0)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row - 1);
            }
            else
            {
                return Cell(column, row - 1);
            }
        }
        break;

    case DirectionToken::FRight:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row != 7)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row + 1);
            }
            else
            {
                return Cell(column, row - 1);
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && row != 0)
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
        if(piece == Pieces::OpKing && row != 0)
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
        else if(piece == Pieces::King && row != 7)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row + 1);
            }
            else
            {
                return Cell(column, row + 1);
            }
        }
        break;

    case DirectionToken::BRight:
        if(piece == Pieces::OpKing && row != 0)
        {
            if(!(row % 2) && column != 0)
            {
                return Cell(column - 1, row - 1);
            }
            else
            {
                return Cell(column, row - 1);
            }
        }
        else if(piece == Pieces::King && row != 7)
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
            QPair<qint8, qint8> jump = jumpNewCellModifier(move);
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

QVector<Cell> Board::getRemovedCells(MovePointer move) const
{
    quint8 cellNum = move->cell.getCellNum();
    quint8 column = move->cell.column;
    quint8 row = move->cell.row;
    Pieces piece = this->data.at(cellNum);

    switch(move->direction)
    {
    case DirectionToken::JFLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row < 6 && column != 3)
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
        else if((piece == Pieces::Player || piece == Pieces::King) && row > 1 && column != 0)
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
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row < 6 && column != 0)
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
        else if((piece == Pieces::Player || piece == Pieces::King) && row > 1 && column != 3)
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
        if(piece == Pieces::OpKing && row > 1 && column != 3)
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
        else if(piece == Pieces::King && row < 6 && column != 0)
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
        if(piece == Pieces::OpKing && row > 1 && column != 0)
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
        else if(piece == Pieces::King && row < 6 && column != 3)
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

QPair<qint8, qint8> Board::jumpNewCellModifier(MovePointer move) const
{
    quint8 cellNum = move->cell.getCellNum();
    quint8 row = move->cell.row;
    quint8 column = move->cell.column;
    const Pieces piece = data.at(cellNum);
    switch(move->direction)
    {
    case DirectionToken::JFLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row < 6 && column != 3)
        {
            if(move->concatenatedMove.isNull())
            {
                return QPair<qint8, qint8>(1, 2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
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
        else if((piece == Pieces::Player || piece == Pieces::King) && row > 1 && column != 0)
        {
            if(move->concatenatedMove.isNull())
            {
                return QPair<qint8, qint8>(-1, -2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
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
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && row < 6 && column != 0)
        {
            if(move->concatenatedMove.isNull())
            {
                return QPair<qint8, qint8>(-1, 2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Pieces::OpKing && move->concatenatedMove->direction == DirectionToken::JBRight))
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
        else if((piece == Pieces::Player || piece == Pieces::King) && row > 1 && column != 3)
        {
            if(move->concatenatedMove.isNull())
            {
                return QPair<qint8, quint8>(1, -2);
            }
            else if(move->concatenatedMove->direction == DirectionToken::JFLeft
                    || move->concatenatedMove->direction == DirectionToken::JFRight
                    || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBLeft)
                    || (piece == Pieces::King && move->concatenatedMove->direction == DirectionToken::JBRight))
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
        if(piece == Pieces::OpKing && row > 1 && column != 3)
        {
            if(move->concatenatedMove.isNull())
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
        else if(piece == Pieces::King && row < 6 && column != 0)
        {
            if(move->concatenatedMove.isNull())
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
        if(piece == Pieces::OpKing && row > 1 && column != 0)
        {
            if(move->concatenatedMove.isNull())
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
        else if(piece == Pieces::King && row < 6 && column != 3)
        {
            if(move->concatenatedMove.isNull())
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

QVector<MovePointer> Board::getAllMoves(Cell cell, bool allowSingleJumps, bool onlyJumps) const
{
    quint8 cellNum = cell.getCellNum();
    Pieces piece = data.at(cellNum);
    QVector<MovePointer> moves;

    if(piece == Pieces::Empty)
        return moves;

    if(!onlyJumps)
    {
        MovePointer move(new Move(cell, DirectionToken::FLeft));
        if(isMoveValid(move))
        {
            moves << move;
        }
        move.reset(new Move(cell, DirectionToken::FRight));
        if(isMoveValid(move))
        {
            moves << move;
        }
        move.reset(new Move(cell, DirectionToken::BLeft));
        if(isMoveValid(move))
        {
            moves << move;
        }
        move.reset(new Move(cell, DirectionToken::BRight));
        if(isMoveValid(move))
        {
            moves << move;
        }
    }

    MovePointer move(new Move(cell, DirectionToken::JFLeft));
    if(isMoveValid(move))
    {
        if(allowSingleJumps)
        {
            moves << move;
        }
        else
        {
            BoardPointer newBoard = this->executeMove(move);
            QVector<MovePointer> nm = newBoard->getAllMoves(this->getNewCell(move), false, true);
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
    }
    move.reset(new Move(cell, DirectionToken::JFRight));
    if(isMoveValid(move))
    {
        if(allowSingleJumps)
        {
            moves << move;
        }
        else
        {
            BoardPointer newBoard = this->executeMove(move);
            QVector<MovePointer> nm = newBoard->getAllMoves(this->getNewCell(move), false, true);
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
    }
    move.reset(new Move(cell, DirectionToken::JBLeft));
    if(isMoveValid(move))
    {
        if(allowSingleJumps)
        {
            moves << move;
        }
        else
        {
            BoardPointer newBoard = this->executeMove(move);
            QVector<MovePointer> nm = newBoard->getAllMoves(this->getNewCell(move), false, true);
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
    }
    move.reset(new Move(cell, DirectionToken::JBRight));
    if(isMoveValid(move))
    {
        if(allowSingleJumps)
        {
            moves << move;
        }
        else
        {
            BoardPointer newBoard = this->executeMove(move);
            QVector<MovePointer> nm = newBoard->getAllMoves(this->getNewCell(move), false, true);
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
    }

    return moves;
}

BoardPointer Board::executeMove(MovePointer move) const
{
    if(!this->isMoveValid(move))
    {
        return BoardPointer();
    }

    BoardData nbd = this->data;
    switch(move->direction)
    {
    case DirectionToken::FLeft:
    case DirectionToken::FRight:
    case DirectionToken::BLeft:
    case DirectionToken::BRight:
        nbd[getNewCell(move).getCellNum()] = nbd[move->cell.getCellNum()];
        nbd[move->cell.getCellNum()] = Pieces::Empty;
        break;
    case DirectionToken::JFLeft:
    case DirectionToken::JFRight:
    case DirectionToken::JBLeft:
    case DirectionToken::JBRight:
        nbd[getNewCell(move).getCellNum()] = nbd[move->cell.getCellNum()];
        nbd[move->cell.getCellNum()] = Pieces::Empty;
        for(Cell &cell : this->getRemovedCells(move))
        {
            nbd[cell.getCellNum()] = Pieces::Empty;
        }
        break;
    }
    return BoardPointer(new Board(nbd));
}

BoardPointer Board::treeBranchGenerator() const
{
    return BoardPointer();
}

BoardPointer Board::defaultBoard()
{
    BoardData data;
    for(int i = 0; i < 12; i++)
        data.append(Pieces::OpPlayer);
    for(int i = 0; i < 8; i++)
        data.append(Pieces::Empty);
    for(int i = 0; i < 12; i++)
        data.append(Pieces::Player);
    return BoardPointer(new Board(data));
}
