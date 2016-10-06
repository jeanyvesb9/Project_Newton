#include "board.h"

using namespace Game;

Board::Board(BoardData data, QObject *parent)
    : QObject(parent),
      data{data}
{

}

MoveValidity Board::isMoveValid(MovePointer move) const
{
    Cell &cell = move->cell;
    quint8 cellNum = cell.row * 4 + cell.column;
    const Pieces piece = data.at(cellNum);

    if(piece == Pieces::Empty)
        return MoveValidity::Invalid;

    switch(move->direction)
    {
    case DirectionToken::FLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && cell.row != 7)
        {
            if(!(cell.row % 2) && data.at(cellNum + 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(cell.column != 3 && data.at(cellNum + 5) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && cell.row != 0)
        {
            if(!(cell.row % 2) && cell.column != 0 && data.at(cellNum - 5) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(data.at(cellNum - 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        break;

    case DirectionToken::FRight:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && cell.row != 7)
        {
            if(!(cell.row % 2) && cell.column != 0 && data.at(cellNum + 3) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(data.at(cellNum + 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && cell.row != 0)
        {
            if(!(cell.row % 2) && data.at(cellNum - 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(cell.column != 3 && data.at(cellNum - 3) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        break;

    case DirectionToken::BLeft:
        if(piece == Pieces::OpKing && cell.row != 0)
        {
            if(!(cell.row % 2) && data.at(cellNum - 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(cell.column != 3 && data.at(cellNum - 3) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        else if(piece == Pieces::King && cell.row != 7)
        {
            if(!(cell.row % 2) && cell.column != 0 && data.at(cellNum + 3) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(data.at(cellNum + 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        break;

    case DirectionToken::BRight:
        if(piece == Pieces::OpKing && cell.row != 0)
        {
            if(!(cell.row % 2) && cell.column != 0 && data.at(cellNum - 5) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(data.at(cellNum - 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        else if(piece == Pieces::King && cell.row != 7)
        {
            if(!(cell.row % 2) && data.at(cellNum + 4) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
            else if(cell.column != 3 && data.at(cellNum + 5) == Pieces::Empty)
            {
                return MoveValidity::Valid;
            }
        }
        break;

    case DirectionToken::JFLeft:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && cell.row < 6 && cell.column != 3)
        {
            if(!(cell.row % 2) && (data.at(cellNum + 4) == Pieces::Player || data.at(cellNum + 4) == Pieces::King)
                    && data.at(cellNum + 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBRight)
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
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && cell.row > 1 && cell.column != 0)
        {
            if(!(cell.row % 2) && (data.at(cellNum - 5) == Pieces::OpPlayer || data.at(cellNum - 5) == Pieces::OpKing)
                    && data.at(cellNum - 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::King && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::King && move->direction == DirectionToken::JBRight)
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
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::King && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::King && move->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFLeft));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JFRight:
        if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && cell.row < 6 && cell.column != 0)
        {
            if(!(cell.row % 2) && (data.at(cellNum + 3) == Pieces::Player || data.at(cellNum + 3) == Pieces::King)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBRight)
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
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::OpKing && move->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        else if((piece == Pieces::Player || piece == Pieces::King) && cell.row > 1 && cell.column != 3)
        {
            if(!(cell.row % 2) && (data.at(cellNum - 4) == Pieces::OpPlayer || data.at(cellNum - 4) == Pieces::OpKing)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::King && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::King && move->direction == DirectionToken::JBRight)
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
                    return MoveValidity::Valid;
                }
                else if(move->direction == DirectionToken::JFLeft || move->direction == DirectionToken::JFRight
                        || piece == Pieces::King && move->direction == DirectionToken::JBLeft
                        || piece == Pieces::King && move->direction == DirectionToken::JBRight)
                {
                    MovePointer newMove(new Move(cell, DirectionToken::JFRight));
                    BoardPointer childBoard = this->executeMove(newMove);
                    return childBoard->isMoveValid(move->concatenatedMove);
                }
            }
        }
        break;

    case DirectionToken::JBLeft:
        if(piece == Pieces::OpKing && cell.row > 1 && cell.column != 3)
        {
            if(!(cell.row % 2) && (data.at(cellNum - 4) == Pieces::Player || data.at(cellNum - 4) == Pieces::King)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
            else if((data.at(cellNum - 3) == Pieces::Player || data.at(cellNum - 3) == Pieces::King)
                    && data.at(cellNum - 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
        }
        else if(piece == Pieces::King && cell.row < 6 && cell.column != 0)
        {
            if(!(cell.row % 2) && (data.at(cellNum + 3) == Pieces::OpPlayer || data.at(cellNum + 3) == Pieces::OpKing)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
            else if((data.at(cellNum + 4) == Pieces::OpPlayer || data.at(cellNum + 4) == Pieces::OpKing)
                    && data.at(cellNum + 7) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBLeft));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
        }
        break;

    case DirectionToken::JBRight:
        if(piece == Pieces::OpKing && cell.row > 1 && cell.column != 0)
        {
            if(!(cell.row % 2) && (data.at(cellNum - 5) == Pieces::Player || data.at(cellNum - 5) == Pieces::King)
                    && data.at(cellNum - 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
            else if((data.at(cellNum - 4) == Pieces::Player || data.at(cellNum - 4) == Pieces::King)
                    && data.at(cellNum - 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
        }
        else if(piece == Pieces::King && cell.row < 6 && cell.column != 3)
        {
            if(!(cell.row % 2) && (data.at(cellNum + 4) == Pieces::OpPlayer || data.at(cellNum + 4) == Pieces::OpKing)
                    && data.at(cellNum + 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
            else if((data.at(cellNum + 5) == Pieces::OpPlayer || data.at(cellNum + 5) == Pieces::OpKing)
                    && data.at(cellNum + 9) == Pieces::Empty)
            {
                if(move->concatenatedMove.isNull())
                {
                    return MoveValidity::Valid;
                }
                MovePointer newMove(new Move(cell, DirectionToken::JBRight));
                BoardPointer childBoard = this->executeMove(newMove);
                return childBoard->isMoveValid(move->concatenatedMove);
            }
        }
        break;
    }

    return MoveValidity::Invalid;
}

QVector<MovePointer> Board::getAllMoves(Cell cell, bool allowSingleJumps, bool onlyJumps) const
{
    quint8 cellNum = cell.row * 4 + cell.column;
    Pieces piece = data.at(cellNum);
    QVector<MovePointer> moves;
    QVector<MovePointer> jumpMoves;

    if(piece == Pieces::Empty)
        return moves;



    if(onlyJumps)
    {
        return jumpMoves;
    }
    else
    {
        moves << jumpMoves;
        return moves;
    }
}

BoardPointer Board::executeMove(MovePointer move) const
{
    return BoardPointer();
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
