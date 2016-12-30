#include "gameengine.h"

GameEngine::GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2, int tie, QObject *parent) : QObject(parent),
    board{Game::Board::defaultBoard()}, tieValue{tie}, turnNumber{1},
    player1{p1}, player1_thread{new QThread()}, player2{p2}, player2_thread{new QThread()}, timerId{0}, playTime{0}
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, 2);
    currentPlayer = static_cast<Player>(uni(rng));

    QObject::connect(player1_thread, &QThread::started, player1.data(), &AbstractPlayer::launchBackgroundTask);
    QObject::connect(player1.data(), &AbstractPlayer::finishedTurn, this, &GameEngine::player1_hasFinished);
    QObject::connect(player1.data(), &AbstractPlayer::hasStopped, player1_thread, &QThread::quit);
    QObject::connect(player1_thread, &QThread::finished, player1_thread, &QThread::deleteLater);
    player1->moveToThread(player1_thread);
    player1_thread->start(QThread::TimeCriticalPriority);

    QObject::connect(player2_thread, &QThread::started, player2.data(), &AbstractPlayer::launchBackgroundTask);
    QObject::connect(player2.data(), &AbstractPlayer::finishedTurn, this, &GameEngine::player2_hasFinished);
    QObject::connect(player2.data(), &AbstractPlayer::hasStopped, player2_thread, &QThread::quit);
    QObject::connect(player2_thread, &QThread::finished, player2_thread, &QThread::deleteLater);
    player2->moveToThread(player2_thread);
    player2_thread->start(QThread::TimeCriticalPriority);

    timerId = startTimer(1000);

    if(currentPlayer == Player::Player1)
    {
        QMetaObject::invokeMethod(player1.data(), "startTurn", Qt::QueuedConnection, Q_ARG(Game::BoardData, board->getBoardData()));
    }
    else
    {
        QMetaObject::invokeMethod(player2.data(), "startTurn", Qt::QueuedConnection, Q_ARG(Game::BoardData, board->getBoardData()));
    }
}

GameEngine::GameEngine(AbstractPlayerPointer p1, AbstractPlayerPointer p2,
                       Game::BoardData board, int turnNumber, Player currentPlayer, quint64 playtime,
                       int tie,
                       QObject *parent) : QObject(parent),
    board{Game::BoardPointer(new Game::Board(board))}, tieValue{tie}, turnNumber{turnNumber},
    player1{p1}, player1_thread{new QThread()}, player2{p2}, player2_thread{new QThread()},
    currentPlayer{currentPlayer}, timerId{0}, playTime{playtime}
{
    QObject::connect(player1_thread, &QThread::started, player1.data(), &AbstractPlayer::launchBackgroundTask);
    QObject::connect(player1.data(), &AbstractPlayer::finishedTurn, this, &GameEngine::player1_hasFinished);
    QObject::connect(player1.data(), &AbstractPlayer::hasStopped, player1_thread, &QThread::quit);
    QObject::connect(player1_thread, &QThread::finished, player1_thread, &QThread::deleteLater);
    player1->moveToThread(player1_thread);
    player1_thread->start(QThread::TimeCriticalPriority);

    QObject::connect(player2_thread, &QThread::started, player2.data(), &AbstractPlayer::launchBackgroundTask);
    QObject::connect(player2.data(), &AbstractPlayer::finishedTurn, this, &GameEngine::player2_hasFinished);
    QObject::connect(player2.data(), &AbstractPlayer::hasStopped, player2_thread, &QThread::quit);
    QObject::connect(player2_thread, &QThread::finished, player2_thread, &QThread::deleteLater);
    player2->moveToThread(player2_thread);
    player2_thread->start(QThread::TimeCriticalPriority);

    timerId = startTimer(1000);

    if(currentPlayer == Player::Player1)
    {
        QMetaObject::invokeMethod(player1.data(), "startTurn", Qt::QueuedConnection, Q_ARG(Game::BoardData, this->board->getBoardData()));
    }
    else
    {
        QMetaObject::invokeMethod(player2.data(), "startTurn", Qt::QueuedConnection, Q_ARG(Game::BoardData, this->board->getBoardData()));
    }
}

GameEngine::~GameEngine()
{
    if(timerId)
    {
        killTimer(timerId);
        timerId = 0;
    }
}

void GameEngine::player1_hasFinished(Game::MovePointer move, int time)
{
    if(move.isNull())
    {
        stopGame();
        emit hasWon(Player::Player2);
        return;
    }

    PlayerMovePointer pMove = PlayerMovePointer(new PlayerMove());
    pMove->player = Player::Player1;
    pMove->move = move;
    pMove->boardMdf = Game::BoardModificationPointer(new Game::BoardModification());
    pMove->time = time;
    board = board->executeMove(move, pMove->boardMdf);
    emit madeMove(pMove, board->getBoardData());

    if(getPlayer2_removed() == 12)
    {
        stopGame();
        emit hasWon(Player::Player1);
        return;
    }
    turnNumber++;
    if(tieValue != -1 && turnNumber == tieValue)
    {
        stopGame();
        emit hasTied();
        return;
    }
    currentPlayer = Player::Player2;
    QMetaObject::invokeMethod(player2.data(), "startTurn", Qt::QueuedConnection, Q_ARG(Game::BoardData, board->getBoardData()));
}

void GameEngine::player2_hasFinished(Game::MovePointer move, int time)
{
    if(move.isNull())
    {
        stopGame();
        emit hasWon(Player::Player1);
        return;
    }

    PlayerMovePointer pMove = PlayerMovePointer(new PlayerMove());
    pMove->player = Player::Player2;
    pMove->move = move;
    pMove->boardMdf = Game::BoardModificationPointer(new Game::BoardModification());
    pMove->time = time;
    board = board->executeMove(move, pMove->boardMdf);
    emit madeMove(pMove, board->getBoardData());

    if(getPlayer1_removed() == 12)
    {
        stopGame();
        emit hasWon(Player::Player2);
        return;
    }
    turnNumber++;
    if(tieValue != -1 && turnNumber == tieValue)
    {
        stopGame();
        emit hasTied();
        return;
    }
    currentPlayer = Player::Player1;
    QMetaObject::invokeMethod(player1.data(), "startTurn", Qt::QueuedConnection, Q_ARG(Game::BoardData, board->getBoardData()));
}

int GameEngine::getPlayTime() const
{
    return playTime;
}

Player GameEngine::getWinningPlayer() const
{
    if(getPlayer1_removed() < getPlayer2_removed())
    {
        return Player::Player1;
    }
    else if(getPlayer2_removed() < getPlayer1_removed())
    {
        return Player::Player2;
    }
    else
    {
        return Player::None;
    }
}

void GameEngine::stopGame()
{
    if(timerId)
    {
        killTimer(timerId);
        timerId = 0;
    }
    player1->stop();
    player2->stop();
    currentPlayer = Player::None;
}

void GameEngine::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    playTime += 1000;
    emit(oneSecondTimerTick(playTime));
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

Player GameEngine::getCurrentPlayer() const
{
    return currentPlayer;
}

Game::BoardPointer GameEngine::getBoard() const
{
    return board;
}

Game::BoardPointer *GameEngine::getBoardPtr()
{
    return &board;
}
