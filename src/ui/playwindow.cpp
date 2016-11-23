#include "playwindow.h"
#include "ui_playwindow.h"

PlayWindow::PlayWindow(Game::GameFilePointer game, NN::NeuralNetworkManagerPointer nnm, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayWindow)
{
    ui->setupUi(this);
    auto nnbp = nnm->getBestNNPerformer();
    nn = nnm->getBestNNPerformer()->neuralNetwork;

    brdWidget = new BoardWidget();
    brd_rw = new AspectRatioWidget(brdWidget, ui->boardBox->layout()->widget());
    brdWidget->setParent(brd_rw);
    ui->boardBox->layout()->addWidget(brd_rw);
    QObject::connect(brdWidget, &BoardWidget::selectedMove, this, &PlayWindow::selectedMove);

    manualPlayer = ManualPlayerPointer(new ManualPlayer(Game::Side::PlayerSide));
    nnPlayer = NeuralNetworkPlayerPointer(new NeuralNetworkPlayer(Game::Side::OpponentSide, nn, 4));
    gameEngine = GameEnginePointer(new GameEngine(manualPlayer, nnPlayer, -1, this));
    QObject::connect(gameEngine.data(), &GameEngine::madeMove, this, &PlayWindow::madeMove);
}

PlayWindow::~PlayWindow()
{
    delete ui;

    manualPlayer.reset();
    nnPlayer.reset();

    gameEngine->deleteLater();
    gameEngine.reset();
}

void PlayWindow::on_menu_btn_clicked()
{

}

void PlayWindow::on_exit_btn_clicked()
{

}

void PlayWindow::madeMove(PlayerMovePointer move)
{
    if(move->player == 2)
    {
        brdWidget->setBoard(gameEngine->getBoard()->getBoardData());
        brdWidget->displayAvailableMoves(gameEngine->getBoard(), gameEngine->getBoard()->getAllMoves(Game::Side::PlayerSide));
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

    manualPlayer->validateTurn(valData);
}
