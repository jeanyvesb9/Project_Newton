#include "gamefile.h"

Game::GameFile::GameFile(QObject *parent) : QObject(parent)
{

}

Game::GameFile::GameFile(QString file, QObject *parent)
{

}

Game::GameFilePointer Game::GameFile::createNewGame(QString file)
{
    GameFilePointer ret = GameFilePointer(new GameFile());
    ret->board = Game::Board::defaultBoard()->getBoardData();
    return ret;
}
