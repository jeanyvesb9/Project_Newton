#include "gameengine.h"

GameEngine::GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2, int tie, QObject *parent) : QObject(parent),
    board{Game::Board::defaultBoard()}, tieValue{tie}, turnNumber{1}, player1{p1}, player1_thread{new QThread(this)}, player2{p2},
    player2_thread{new QThread(this)}, time{new QTime()}
{
    startingPlayer = std::rand() + 1;
    currentPlayer = startingPlayer;

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

    if(startingPlayer == 1)
    {
        player1->startTurn(board->getBoardData());
    }
    else
    {
        player2->startTurn(board->getBoardData());
    }

    time->restart();
}

GameEngine::GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2,
                       Game::BoardData board, int turnNumber, int startingPlayer, int currentPlayer, int playtime,
                       int tie,
                       QObject *parent) : QObject(parent),
    board{Game::BoardPointer(new Game::Board(board))}, tieValue{tie}, turnNumber{turnNumber},
    player1{p1}, player1_thread{new QThread(this)}, player2{p2}, player2_thread{new QThread(this)},
    startingPlayer{startingPlayer}, currentPlayer{currentPlayer}, playTime{playtime}, time{new QTime()}
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

    if(startingPlayer == 1)
    {
        player1->startTurn(this->board->getBoardData());
    }
    else
    {
        player2->startTurn(this->board->getBoardData());
    }

    time->restart();}

GameEngine::~GameEngine()
{
    if(time)
    {
        delete time;
    }
}

void GameEngine::player1_hasFinished(Game::MovePointer move, int time)
{
    QVector<Game::MovePointer> mvs;
    mvs <<move;
    qDebug() <<"P1:";
    qDebug().noquote() <<printMoveTree(mvs);

    Q_UNUSED(time);
    PlayerMovePointer pMove = PlayerMovePointer(new PlayerMove());
    pMove->player = 1;
    pMove->move = move;
    pMove->boardMdf = Game::BoardModificationPointer(new Game::BoardModification());
    board->executeMove(move, pMove->boardMdf);
    emit madeMove(pMove);

    if(getPlayer2_removed() == 12)
    {
        stopGame();
        emit hasWon(1);
    }
    turnNumber++;
    if(turnNumber == tieValue)
    {
        emit hasTied();
    }
    currentPlayer = 2;
    player2->startTurn(board->getBoardData());
}

void GameEngine::player2_hasFinished(Game::MovePointer move, int time)
{
    QVector<Game::MovePointer> mvs;
    mvs <<move;
    qDebug() <<"P2:";
    qDebug().noquote() <<printMoveTree(mvs);

    Q_UNUSED(time);
    PlayerMovePointer pMove = PlayerMovePointer(new PlayerMove());
    pMove->player = 2;
    pMove->move = move;
    pMove->boardMdf = Game::BoardModificationPointer(new Game::BoardModification());
    board->executeMove(move, pMove->boardMdf);
    emit madeMove(pMove);

    if(getPlayer1_removed() == 12)
    {
        stopGame();
        emit hasWon(2);
    }
    turnNumber++;
    if(turnNumber == tieValue)
    {
        emit hasTied();
    }
    currentPlayer = 1;
    player1->startTurn(board->getBoardData());
}

int GameEngine::getPlayTime() const
{
    return time ? playTime + time->elapsed() : playTime;
}

int GameEngine::getWinningPlayer() const
{
    if(getPlayer1_removed() < getPlayer2_removed())
    {
        return 1;
    }
    else if(getPlayer2_removed() < getPlayer1_removed())
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

void GameEngine::stopGame()
{
    player1->stopAndDelete();
    player2->stopAndDelete();
    playTime = playTime + time->elapsed();
    delete time;
    time = nullptr;
}

int GameEngine::getPlayer1_removed() const
{
    return 12-(board->getBoardData().count(Game::Piece::Player) + board->getBoardData().count(Game::Piece::King));
}

int GameEngine::getPlayer1_kings() const
{
    return board->getBoardData().count(Game::Piece::King);
}

int GameEngine::getPlayer2_removed() const
{
    return 12-(board->getBoardData().count(Game::Piece::OpPlayer) + board->getBoardData().count(Game::Piece::OpKing));
}

int GameEngine::getPlayer2_kings() const
{
    return board->getBoardData().count(Game::Piece::OpKing);
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
