#include "playwindow.h"
#include "ui_playwindow.h"

PlayWindow::PlayWindow(Game::GameFilePointer game, NN::NeuralNetworkManagerPointer nnm, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::PlayWindow),
    gameFile{game}, pm{nullptr}
{
    ui->setupUi(this);

#ifdef MACX
    MacWindow::setWindowStyle(this->winId());
#endif

    int f1 = QFontDatabase::addApplicationFont(QStringLiteral(":/assets/fonts/AdobeClean-Light.otf"));
    int f2 = QFontDatabase::addApplicationFont(QStringLiteral(":/assets/fonts/segoeuil.ttf"));
    adobeCleanLight = QFont(QFontDatabase::applicationFontFamilies(f1).at(0));
    segoeUILight = QFont(QFontDatabase::applicationFontFamilies(f2).at(0));

    ui->title_lbl->setFont(segoeUILight);
    ui->subtitle_lbl->setFont(segoeUILight);
    ui->sign_lbl->setFont(adobeCleanLight);
    ui->timer_lbl->setStyleSheet(QStringLiteral("QLabel { color: #FFFFFF; font-family: 'Adobe Clean'; font-size: 20px; }"));
    ui->turn_lbl->setStyleSheet(QStringLiteral("QLabel { color: #FFFFFF; font-family: 'Segoe UI'; font-size: 20px; }"));

#ifndef MACX
    this->setWindowTitle("Game");
    ui->mac_spacer_1->changeSize(0, 0);
#endif

    nn = nnm->getBestNNPerformer()->neuralNetwork;

    brdWidget = new BoardWidget();
    brd_rw = new AspectRatioWidget(brdWidget, ui->boardBox->layout()->widget());
    brdWidget->setParent(brd_rw);
    ui->boardBox->layout()->addWidget(brd_rw);
    QObject::connect(brdWidget, &BoardWidget::selectedMove, this, &PlayWindow::selectedMove);

    totalTimerUpdate(gameFile->getPlayTime());

    manualPlayer = ManualPlayerPointer(new ManualPlayer(Game::Side::PlayerSide));
    nnPlayer = NeuralNetworkPlayerPointer(new NeuralNetworkPlayer(Game::Side::OpponentSide, nn, gameFile->getDifficulty()));
    gameEngine = GameEnginePointer(new GameEngine(manualPlayer, nnPlayer, gameFile->getBoardData(), gameFile->getCurrentMoveNumber(),
                                                  gameFile->getActivePlayer(), gameFile->getPlayTime(), GameEngine::TieValue::NoTie, this));
    QObject::connect(gameEngine.data(), &GameEngine::madeMove, this, &PlayWindow::madeMove);
    QObject::connect(gameEngine.data(), &GameEngine::hasWon, this, &PlayWindow::gameFinished);
    QObject::connect(gameEngine.data(), &GameEngine::oneSecondTimerTick, this, &PlayWindow::totalTimerUpdate);

    piWidget = new PlayInfoTable(gameEngine->getPlayer1_removed(), gameEngine->getPlayer2_removed(), adobeCleanLight, segoeUILight);
    piWidget->setMinimumHeight(150);
    piWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    ui->table_layout->addWidget(piWidget);

    brdWidget->setBoard(game->getBoardData());
    setPlayerTurnTo(gameFile->getActivePlayer());
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

void PlayWindow::closeEvent(QCloseEvent *event)
{
    gameEngine->stopGame();
    Q_UNUSED(event);
    emit closingSignal();
}

void PlayWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(pm)
    {
        pm->move(this->window()->rect().center() - pm->rect().center());
    }
}

void PlayWindow::on_menu_btn_clicked()
{
    pm = new PlayMenu(false, true, adobeCleanLight, segoeUILight, this);
    QObject::connect(pm, &PlayMenu::resumeGame, this, &PlayWindow::resumeGame);
    pm->move(this->window()->rect().center() - pm->rect().center());
    ui->menu_btn->setEnabled(false);
    ui->exit_btn->setEnabled(false);
    brdWidget->setEnabled(false);
    pm->show();
    pm->setFocus();
}

void PlayWindow::on_exit_btn_clicked()
{
    gameFile->setPlayTime(totalTime);
    close();
}

void PlayWindow::resumeGame()
{
    ui->menu_btn->setEnabled(true);
    ui->exit_btn->setEnabled(true);
    brdWidget->setEnabled(true);
}

void PlayWindow::madeMove(PlayerMovePointer move, Game::BoardData board)
{
    piWidget->updateCount(gameEngine->getPlayer2_removed(), gameEngine->getPlayer1_removed());
    gameFile->addMove(move, board);
    gameFile->setPlayTime(totalTime);
    brdWidget->setBoard(board);
    if(gameEngine->getCurrentPlayer() != Player::None)
    {
        setPlayerTurnTo(move->player == Player::Player1 ? Player::Player2 : Player::Player1);
    }
}

void PlayWindow::selectedMove(Game::MovePointer move)
{
    Game::BoardData bd = gameEngine->getBoard()->executeMove(move)->getBoardData();
    brdWidget->setBoard(bd);

    QVector<BoardPiece> valData;
    for(auto &p : bd)
    {
        switch(p)
        {
        case Game::Piece::Empty:
            valData <<BoardPiece::Empty;
            break;
        case Game::Piece::Player:
        case Game::Piece::King:
            valData << BoardPiece::PlayerPiece;
            break;
        case Game::Piece::OpPlayer:
        case Game::Piece::OpKing:
            valData << BoardPiece::OpPlayerPiece;
            break;
        }
    }

    QMetaObject::invokeMethod(manualPlayer.data(), "validateTurn", Qt::QueuedConnection, Q_ARG(QVector<BoardPiece>, valData));
}

void PlayWindow::gameFinished(Player winner)
{
    gameFile->finishGame(winner);
    ui->turn_lbl->setText(tr("The game has finished"));
    ui->timer_lbl->setText(QString::number(gameFile->getCurrentMoveNumber() - 1) + tr(" moves"));
}

void PlayWindow::totalTimerUpdate(int time)
{
    totalTime = time;
    quint8 totalSeconds = time / 1000;
    if(!(totalSeconds % 10))
    {
        gameFile->setPlayTime(totalTime);
    }

    QString hours;
    if((totalSeconds / 3600) != 0)
    {
        hours = QString::number(totalSeconds / 3600);
        if(hours.length() == 1)
            hours.prepend('0');
        hours.append(':');
    }
    QString minutes = QString::number((totalSeconds / 60) % 60);
    if(minutes.length() == 1)
    {
        minutes.prepend('0');
    }
    minutes.append(':');
    QString seconds = QString::number(totalSeconds % 60);
    if(seconds.length() == 1)
    {
        seconds.prepend('0');
    }
    ui->timer_lbl->setText(hours + minutes + seconds + QStringLiteral(" - ") + QString::number(gameFile->getCurrentMoveNumber() - 1) + tr(" moves"));
}

void PlayWindow::setPlayerTurnTo(Player player)
{
    if(player == Player::Player1)
    {
        brdWidget->displayAvailableMoves(gameEngine->getBoard(), gameEngine->getBoard()->getAllMoves(Game::Side::PlayerSide));
        ui->turn_lbl->setText(tr("It's your turn"));
    }
    else
    {
        ui->turn_lbl->setText(tr("It's the computer's turn"));
    }
}
