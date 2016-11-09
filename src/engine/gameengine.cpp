#include "gameengine.h"

GameEngine::GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2, QObject *parent) : QObject(parent),
    player1_thread{new QThread(this)}, player2_thread{new QThread(this)},
    player1{p1}, player2{p2}
{
    QObject::connect(player1_thread, &QThread::started, player1.data(), &AbstractPlayer::launchBackgroundTask);
    QObject::connect(player1.data(), &AbstractPlayer::finishedTurn, this, &GameEngine::player1_hasFinished);
    QObject::connect(player1.data(), &AbstractPlayer::hasStopped, player1_thread, &QThread::quit);
    player1->moveToThread(player1_thread);
    player1_thread->start();

    QObject::connect(player2_thread, &QThread::started, player2.data(), &AbstractPlayer::launchBackgroundTask);
    QObject::connect(player2.data(), &AbstractPlayer::finishedTurn, this, &GameEngine::player2_hasFinished);
    QObject::connect(player2.data(), &AbstractPlayer::hasStopped, player2_thread, &QThread::quit);
    player2->moveToThread(player2_thread);
    player2_thread->start();
}

GameEngine::GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2,
                       Game::BoardData board, bool startingPlayer, int currentPlayer, int playtime,
                       QObject *parent) : QObject(parent),
    player1_thread{new QThread(this)}, player2_thread{new QThread(this)},
    player1{p1}, player2{p2}, board{Game::BoardPointer(new Game::Board(board))},
    startingPlayer{startingPlayer}, currentPlayer{currentPlayer}, playTime{playtime}
{
    time.restart();
}

void GameEngine::player1_hasFinished(Game::MovePointer move, int time)
{
    PlayerMovePointer pMove = PlayerMovePointer(new PlayerMove());
    pMove->player = 1;
    pMove->move = move;
    pMove->boardMdf = Game::BoardModificationPointer(new Game::BoardModification());
    board->executeMove(move, pMove->boardMdf);

    emit madeMove(pMove);
    currentPlayer = 2;
    player2->startTurn(board->getBoardData());
}

void GameEngine::player2_hasFinished(Game::MovePointer move, int time)
{
    PlayerMovePointer pMove = PlayerMovePointer(new PlayerMove());
    pMove->player = 2;
    pMove->move = move;
    pMove->boardMdf = Game::BoardModificationPointer(new Game::BoardModification());
    board->executeMove(move, pMove->boardMdf);

    emit madeMove(pMove);
    currentPlayer = 2;
    player2->startTurn(board->getBoardData());
}

int GameEngine::getPlayTime() const
{
    return playTime + time.elapsed();
}

int GameEngine::getPlayer1_removed() const
{
    return 12-(board->getBoardData().count(Game::Pieces::Player) + board->getBoardData().count(Game::Pieces::King));
}

int GameEngine::getPlayer1_kings() const
{
    return board->getBoardData().count(Game::Pieces::King);
}

int GameEngine::getPlayer2_removed() const
{
    return 12-(board->getBoardData().count(Game::Pieces::OpPlayer) + board->getBoardData().count(Game::Pieces::OpKing));
}

int GameEngine::getPlayer2_kings() const
{
    return board->getBoardData().count(Game::Pieces::OpKing);
}

bool GameEngine::getCurrentPlayer() const
{
    return currentPlayer;
}

bool GameEngine::getStartingPlayer() const
{
    return startingPlayer;
}

Game::BoardPointer GameEngine::getBoard() const
{
    return board;
}
