#include "board.h"

using namespace Game;

Board::Board(BoardData data, QObject *parent)
    : QObject(parent),
      data{data}
{

}

MoveValidity Board::isMoveValid(MovePointer move, bool allowSingleJumps) const
{
    quint8 cell = move->cell.row * 4 + move->cell.column;
    Pieces piece = data.at(cell);

    if(piece == Pieces::Empty)
        return MoveValidity::Invalid;
    if(piece == Pieces::OpPlayer || piece == Pieces::OpKing)
    {

    }








}

QVector<MovePointer> Board::getAllMoves(Cell cell, bool allowSingleJumps, bool onlyJumps) const
{
    quint8 cellNum = cell.row * 4 + cell.column;
    Pieces piece = data.at(cellNum);
    QVector<MovePointer> moves;
    QVector<MovePointer> jumpMoves;

    if(piece == Pieces::Empty)
        return moves;

    if((piece == Pieces::OpPlayer || piece == Pieces::OpKing) && cell.row != 7)
    {
        if(!cell.row % 2)
        {
            if(data.at(cellNum + 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::FLeft));
            }
            else if((data.at(cellNum + 4) == Pieces::Player || data.at(cellNum + 4) == Pieces::King)
                    && data.at(cellNum + 9) == Pieces::Empty)
            {
                MovePointer move(new Move(cell, DirectionToken::JFLeft));
                if(!allowSingleJumps)
                {
                    BoardPointer childBoard = this->executeMove(move);
                    childBoard->getAllMoves();
                }

            }

            if(cell.column != 0)
            {
                if(data.at(cellNum + 3) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::FRight));
                }
                else if(data.at(cellNum + 3) == Pieces::Player || data.at(cellNum + 3) == Pieces::King)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
        else
        {
            if(data.at(cellNum + 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::FRight));
            }
            else if(data.at(cellNum + 4) == Pieces::Player || data.at(cellNum + 4) == Pieces::King)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 3)
            {
                if(data.at(cellNum + 5) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::FLeft));
                }
                else if(data.at(cellNum + 5) == Pieces::Player || data.at(cellNum + 5) == Pieces::King)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
    }
    if(piece == Pieces::OpKing && cell.row != 0)
    {
        if(!cell.row % 2)
        {
            if(data.at(cellNum - 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::BLeft));
            }
            else if(data.at(cellNum - 4) == Pieces::Player || data.at(cellNum - 4) == Pieces::King)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 0)
            {
                if(data.at(cellNum - 5) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::BRight));
                }
                else if(data.at(cellNum - 5) == Pieces::Player || data.at(cellNum - 5) == Pieces::King)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
        else
        {
            if(data.at(cellNum - 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::BRight));
            }
            else if(data.at(cellNum - 4) == Pieces::Player || data.at(cellNum - 4) == Pieces::King)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 3)
            {
                if(data.at(cellNum - 3) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::BLeft));
                }
                else if(data.at(cellNum - 3) == Pieces::Player || data.at(cellNum - 3) == Pieces::King)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
    }

    if((piece == Pieces::Player || piece == Pieces::King) && cell.row != 0)
    {
        if(!cell.row % 2)
        {
            if(data.at(cellNum - 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::FRight));
            }
            else if(data.at(cellNum - 4) == Pieces::OpPlayer || data.at(cellNum - 4) == Pieces::OpKing)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 0)
            {
                if(data.at(cellNum - 5) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::FLeft));
                }
                else if(data.at(cellNum - 5) == Pieces::OpPlayer || data.at(cellNum - 5) == Pieces::OpKing)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
        else
        {
            if(data.at(cellNum - 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::FLeft));
            }
            else if(data.at(cellNum - 4) == Pieces::OpPlayer || data.at(cellNum - 4) == Pieces::OpKing)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 3)
            {
                if(data.at(cellNum - 3) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::FRight));
                }
                else if(data.at(cellNum - 3) == Pieces::OpPlayer || data.at(cellNum - 3) == Pieces::OpKing)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
    }
    if(piece == Pieces::King && cell.row != 7)
    {
        if(!cell.row % 2)
        {
            if(data.at(cellNum + 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::BRight));
            }
            else if(data.at(cellNum + 4) == Pieces::OpPlayer || data.at(cellNum + 4) == Pieces::OpKing)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 0)
            {
                if(data.at(cellNum + 3) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::BLeft));
                }
                else if(data.at(cellNum + 3) == Pieces::OpPlayer || data.at(cellNum + 3) == Pieces::OpKing)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
        else
        {
            if(data.at(cellNum + 4) == Pieces::Empty)
            {
                moves << MovePointer(new Move(cell, DirectionToken::BLeft));
            }
            else if(data.at(cellNum + 4) == Pieces::OpPlayer || data.at(cellNum + 4) == Pieces::OpKing)
            {
                //USE RECURSIVE FOR JUMPS
            }

            if(cell.column != 3)
            {
                if(data.at(cellNum + 5) == Pieces::Empty)
                {
                    moves << MovePointer(new Move(cell, DirectionToken::BRight));
                }
                else if(data.at(cellNum + 5) == Pieces::OpPlayer || data.at(cellNum + 5) == Pieces::OpKing)
                {
                    //USE RECURSIVE FOR JUMPS
                }
            }
        }
    }

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

}

BoardPointer Board::treeBranchGenerator() const
{

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
