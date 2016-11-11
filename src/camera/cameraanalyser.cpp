#include "cameraanalyser.h"

CameraAnalyser::CameraAnalyser(QObject *parent) : QObject(parent)
{

}

QVector<BoardPiece> convertBoardToBoardPieceVector(Game::BoardPointer board)
{
    QVector<BoardPiece> ret;
    for(auto &d : board->getBoardData())
    {
        switch(d)
        {
        case Game::Piece::Player:
        case Game::Piece::King:
            ret << BoardPiece::PlayerPiece;
            break;
        case Game::Piece::OpPlayer:
        case Game::Piece::OpKing:
            ret << BoardPiece::OpPlayerPiece;
            break;
        case Game::Piece::Empty:
            ret << BoardPiece::Empty;
            break;
        }
    }
    return ret;
}
