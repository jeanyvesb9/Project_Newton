#include "playwindow.h"
#include "ui_playwindow.h"

PlayWindow::PlayWindow(Game::GameFilePointer game, NN::NeuralNetworkManagerPointer nnm, ArduinoSerialPointer arduinoSerial, CameraAnalyzerPointer cameraInterface, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::PlayWindow),
    gameFile{game}, arduinoSerial{arduinoSerial}, camera{cameraInterface}, pm{nullptr}, newGame{false}
{
    ui->setupUi(this);

#ifdef MACOS
    MacWindow::setWindowStyle(this->winId());
#else
    this->setWindowTitle("Game");
    ui->mac_spacer_1->changeSize(0, 0);
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
    ui->menuInfo_lbl->setStyleSheet(QStringLiteral("QLabel { color: #FFFFFF; font-family: 'Segoe UI'; font-size: 10px; }"));



    nn = nnm->getBestNNPerformer()->neuralNetwork;

    brdWidget = new BoardWidget();
    brd_rw = new AspectRatioWidget(brdWidget, ui->boardBox->layout()->widget());
    brdWidget->setParent(brd_rw);
    ui->boardBox->layout()->addWidget(brd_rw);
    QObject::connect(brdWidget, &BoardWidget::selectedMove, this, &PlayWindow::selectedMove);

    totalTimerUpdate(gameFile->getPlayTime());

    manualPlayer = new ManualPlayer(Game::Side::PlayerSide);
    nnPlayer = new NeuralNetworkPlayer(Game::Side::OpponentSide, nn, gameFile->getDifficulty());
    if(arduinoSerial && cameraInterface && !cameraInterface->getErrorState())
    {
        setGameMode(GameMode::WithBoard);
    }
    else
    {
        setGameMode(GameMode::SoftwareOnly);
    }
    gameEngine = GameEnginePointer(new GameEngine(manualPlayer, nnPlayer, gameFile->getBoardData(), gameFile->getCurrentMoveNumber(),
                                                  gameFile->getActivePlayer(), gameFile->getPlayTime(), GameEngine::TieValue::NoTie, gameMode == GameMode::WithBoard, this));
    QObject::connect(gameEngine.data(), &GameEngine::madeMove, this, &PlayWindow::madeMove);
    QObject::connect(gameEngine.data(), &GameEngine::hasWon, this, &PlayWindow::gameFinished);
    QObject::connect(gameEngine.data(), &GameEngine::oneSecondTimerTick, this, &PlayWindow::totalTimerUpdate);

    piWidget = new PlayInfoTable(gameEngine->getPlayer1_removed(), gameEngine->getPlayer2_removed(), adobeCleanLight, segoeUILight);
    piWidget->setMinimumHeight(150);
    piWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    ui->table_layout->addWidget(piWidget);

    brdWidget->setBoard(game->getBoardData());
    if(gameMode == GameMode::SoftwareOnly)
    {
        setPlayerTurnTo(gameFile->getActivePlayer());
    }
}

PlayWindow::~PlayWindow()
{
    delete ui;
}

void PlayWindow::closeEvent(QCloseEvent *event)
{
    gameEngine->stopGame();
    if(gameMode == GameMode::WithBoard)
    {
        arduinoSerial->setBlack();
    }
    Q_UNUSED(event);
    emit closingSignal(newGame);
}

void PlayWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    if(pm)
    {
        pm->move(this->window()->rect().center() - pm->rect().center());
    }
}

void PlayWindow::setGameMode(PlayWindow::GameMode mode)
{
    gameMode = mode;
    if(gameMode == GameMode::WithBoard)
    {
        gameState = GameWithBoardState::InitialCalibration;
        ui->console_label->setVisible(true);
        ui->menuInfo_lbl->setVisible(true);
        ui->console_label->setText(tr("Before playing, you must calibrate the board! Put the correct pieces in each spot, following the pattern shown on screen. To keep track of kings, stack another piece of the same color on top of the one placed already in the designated spot. When you are finished, the camera will then validate your actions. DO NOT place any other object on top of the board, nor have your hand in front of the camera, as it would not be able to complete the board scan.\nPress the OK button on the board to continue."));
        QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn1Press, this, &PlayWindow::nextClicked);
        QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2Press, this, &PlayWindow::okClicked);
        QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2LongPress, this, &PlayWindow::okLongPressed);
        QObject::connect(this->camera.data(), &CameraAnalyzer::rawBoardVector, this, &PlayWindow::handleCameraOutput);
        QObject::connect(this->manualPlayer, &ManualPlayer::wrongMove, this, &PlayWindow::handleWrongManualPlayerMove);
        cameraErrorHandlingConnection = QObject::connect(this->camera.data(), &CameraAnalyzer::cameraError, this, [this]() {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Camera disconected."));
            msgBox.exec();
            camera.reset();
            QObject::disconnect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn1Press, this, &PlayWindow::nextClicked);
            QObject::disconnect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2Press, this, &PlayWindow::okClicked);
            QObject::disconnect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2LongPress, this, &PlayWindow::okLongPressed);
            QObject::disconnect(this->camera.data(), &CameraAnalyzer::rawBoardVector, this, &PlayWindow::handleCameraOutput);
            QObject::disconnect(this->manualPlayer, &ManualPlayer::wrongMove, this, &PlayWindow::handleWrongManualPlayerMove);
            QObject::disconnect(cameraErrorHandlingConnection);
            this->setGameMode(GameMode::SoftwareOnly);
        });
    }
    else
    {
        ui->console_label->setVisible(false);
        ui->menuInfo_lbl->setVisible(false);
        //gameEngine->resumeGame();
    }
}

void PlayWindow::handleCameraOutput(QVector<BoardPiece> rawVector)
{
    if(gameState == GameWithBoardState::InitialCalibration)
    {
        if(convertBoardToBoardPieceVector(gameEngine->getBoard()) == rawVector)
        {
            previousBoard = gameEngine->getBoard();
            setPlayerTurnTo(gameFile->getActivePlayer());
            gameEngine->resumeGame();
        }
        else
        {
            ui->console_label->setText(tr("OOPS! The camera couldn't validate your actions, check the board and try again! If you are not successful after another attempt, you might need to adjust the camera parameters. Close this window and proceed to recalibrate.\nPut the correct pieces in each spot, following the pattern shown on screen. DO NOT place any other object on top of the board, nor have your hand in front of the camera, as it would not be able to complete the board scan.\nPress the OK button on the board to continue."));
        }
    }
    else if(gameState == GameWithBoardState::ComputerPlaying)
    {

    }
    else if(gameState == GameWithBoardState::ComputerShowingMoves)
    {
        if(convertBoardToBoardPieceVector(gameEngine->getBoard()) == rawVector)
        {
            setPlayerTurnTo(Player::Player1);
        }
        else
        {
            arduinoSerial->sendMove(previousBoard, auxMove);
            ui->console_label->setText(tr("OOPS! The camera couldn't validate your actions, check the board and try again! If you are not successful after another attempt, you might need to adjust the camera parameters. Close this window and proceed to recalibrate.\nExecute the computer's move on the board, following the light patterns. If you are confused or made a mistake, use the screen as a reference. When you are done, the computer will proceed to validate your actions. Keep the board clear for the camera.\n\n Press OK when you have finished."));
        }
    }
    else if(gameState == GameWithBoardState::UserPlaying)
    {
        QMetaObject::invokeMethod(manualPlayer, "validateTurn", Qt::QueuedConnection, Q_ARG(QVector<BoardPiece>, rawVector));
    }
    else if(gameState == GameWithBoardState::CheckingUserMoves)
    {

    }
    else if(gameState == GameWithBoardState::ShowingUserBrdWidgetMoves)
    {
        if(convertBoardToBoardPieceVector(gameEngine->getBoard()->executeMove(auxMove)) == rawVector)
        {
            QMetaObject::invokeMethod(manualPlayer, "madeMove", Qt::QueuedConnection, Q_ARG(Game::MovePointer, auxMove));
        }
        else
        {
            arduinoSerial->sendMove(gameEngine->getBoard(), auxMove);
            ui->console_label->setText(tr("OOPS! The camera couldn't validate your actions, check the board and try again! If you are not successful after another attempt, you might need to adjust the camera parameters. Close this window and proceed to recalibrate.\nExecute your move on the board, following the light patterns. If you are confused or made a mistake, use the screen as a reference. When you are done, the computer will proceed to validate your actions. Keep the board clear for the camera.\n\n Press OK when you have finished."));
        }
    }
}

void PlayWindow::handleWrongManualPlayerMove()
{
    ui->console_label->setText(tr("OOPS! The camera couldn't validate your actions, check the board and try again! If you are not successful after another attempt, you might need to adjust the camera parameters. Close this window and proceed to recalibrate.\nMake a move! Remember to take the jump if it's available. When you are done, the computer will proceed to validate your actions. Keep the board clear for the camera.\n\n Press OK when you have finished."));
}

void PlayWindow::on_menu_btn_clicked()
{
    gameFile->setPlayTime(totalTime);
    gameEngine->pauseGame();
    ui->menu_btn->setEnabled(false);
    ui->exit_btn->setEnabled(false);
    brdWidget->setEnabled(false);
    pm = new PlayMenu(gameMode == GameMode::WithBoard, adobeCleanLight, segoeUILight, this);
    QObject::connect(pm, &PlayMenu::resumeGame, this, &PlayWindow::resumeGame);
    QObject::connect(pm, &PlayMenu::newGame, [this](){
        newGame = true;
        on_exit_btn_clicked();
    });
    QObject::connect(pm, &PlayMenu::quitGame, this, &PlayWindow::on_exit_btn_clicked);
    if(gameMode == GameMode::WithBoard)
    {
        QObject::disconnect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn1Press, this, &PlayWindow::nextClicked);
        QObject::disconnect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2Press, this, &PlayWindow::okClicked);
        QObject::disconnect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2LongPress, this, &PlayWindow::okLongPressed);
        QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn1Press, pm, &PlayMenu::nextPressed);
        QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2Press, pm, &PlayMenu::okPressed);
    }
    pm->move(this->window()->rect().center() - pm->rect().center());
    pm->show();
    pm->setFocus();
}

void PlayWindow::on_exit_btn_clicked()
{
    gameFile->setPlayTime(totalTime);
    close();
}

void PlayWindow::nextClicked()
{
    if(gameState == GameWithBoardState::InitialCalibration)
    {

    }
    else if(gameState == GameWithBoardState::ComputerPlaying)
    {

    }
    else if(gameState == GameWithBoardState::ComputerShowingMoves)
    {

    }
    else if(gameState == GameWithBoardState::UserPlaying)
    {

    }
    else if(gameState == GameWithBoardState::CheckingUserMoves)
    {

    }
    else if(gameState == GameWithBoardState::ShowingUserBrdWidgetMoves)
    {

    }
}

void PlayWindow::okClicked()
{
    if(gameState == GameWithBoardState::InitialCalibration)
    {
        camera->captureSingleRawBoard();
    }
    else if(gameState == GameWithBoardState::ComputerPlaying)
    {

    }
    else if(gameState == GameWithBoardState::ComputerShowingMoves)
    {
        arduinoSerial->setBlack();
        QTimer::singleShot(200, [this]() {
            this->camera->captureSingleRawBoard();
        });
    }
    else if(gameState == GameWithBoardState::UserPlaying)
    {
        camera->captureSingleRawBoard();
    }
    else if(gameState == GameWithBoardState::CheckingUserMoves)
    {

    }
    else if(gameState == GameWithBoardState::ShowingUserBrdWidgetMoves)
    {
        arduinoSerial->setBlack();
        QTimer::singleShot(200, [this]() {
            this->camera->captureSingleRawBoard();
        });
    }
}

void PlayWindow::okLongPressed()
{
    on_menu_btn_clicked();
}

void PlayWindow::resumeGame()
{
    QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn1Press, this, &PlayWindow::nextClicked);
    QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2Press, this, &PlayWindow::okClicked);
    QObject::connect(this->arduinoSerial.data(), &ArduinoSerial::gotBtn2LongPress, this, &PlayWindow::okLongPressed);
    if(gameState == GameWithBoardState::UserPlaying || gameState == GameWithBoardState::ComputerPlaying)
    {
        gameEngine->resumeGame();
    }
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
        if(gameMode == GameMode::SoftwareOnly || gameState == GameWithBoardState::UserPlaying)
        {
            setPlayerTurnTo(move->player == Player::Player1 ? Player::Player2 : Player::Player1);
            previousBoard = gameEngine->getBoard();
        }
        else
        {
            gameState = GameWithBoardState::ComputerShowingMoves;
            ui->console_label->setText(tr("Execute the computer's move on the board, following the light patterns. If you are confused or made a mistake, use the screen as a reference. When you are done, the computer will proceed to validate your actions. Keep the board clear for the camera.\n\n Press OK when you have finished."));
            auxMove = move->move;
            arduinoSerial->sendMove(previousBoard, move->move);
        }
    }
}

void PlayWindow::selectedMove(Game::MovePointer move)
{
    if(gameMode == GameMode::WithBoard)
    {
        auxMove = move;
        gameState = GameWithBoardState::ShowingUserBrdWidgetMoves;
        ui->console_label->setText(tr("Execute your move on the board, following the light patterns. If you are confused or made a mistake, use the screen as a reference. When you are done, the computer will proceed to validate your actions. Keep the board clear for the camera.\n\n Press OK when you have finished."));
        arduinoSerial->sendMove(gameEngine->getBoard(), move);
    }
    else
    {
        QMetaObject::invokeMethod(manualPlayer, "madeMove", Qt::QueuedConnection, Q_ARG(Game::MovePointer, move));
    }
}

void PlayWindow::gameFinished(Player winner)
{
    gameFile->finishGame(winner);
    ui->turn_lbl->setText(tr("The game has finished"));
    ui->console_label->setText(tr("The game has finished! Start another from the home screen, or from the option in the menu."));
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
        if(gameMode == GameMode::WithBoard)
        {
            ui->console_label->setText("Make a move! Remember to take the jump if it's available. When you are done, the computer will proceed to validate your actions. Keep the board clear for the camera.\n\n Press OK when you have finished.");
            gameState = GameWithBoardState::UserPlaying;
        }
    }
    else if(player == Player::Player2)
    {
        ui->turn_lbl->setText(tr("It's the computer's turn"));
        if(gameMode == GameMode::WithBoard)
        {
            ui->console_label->setText("The computer is making it's move. Please wait...");
            gameState = GameWithBoardState::ComputerPlaying;
        }
    }
}
