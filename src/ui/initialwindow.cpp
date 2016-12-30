#include "initialwindow.h"
#include "ui_initialwindow.h"

InitialWindow::InitialWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InitialWindow), nnt{nullptr}, boardConnected{0}, cameraConnected{0}
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Project Newton"));
    this->setMinimumSize(800, 600);

    portList = ArduinoSerial::getSerialPorts();
    for(auto &item : portList)
    {
        ui->cameraList->addItem(item.portName());
        ui->boardList->addItem(item.portName());
    }

    brdWidget = new BoardWidget();
    brd_rw = new AspectRatioWidget(brdWidget, ui->gameBox->layout()->widget());
    brdWidget->setParent(brd_rw);
    ui->gameBox->layout()->addWidget(brd_rw);

    ui->trainNN->setEnabled(0);
    ui->gameBox->setEnabled(false);
    ui->playGame->setEnabled(false);
}

InitialWindow::~InitialWindow()
{
    delete ui;

    if(!nnm.isNull() && nnm->isValid())
    {
        nnm->saveAndClose();
    }
}

void InitialWindow::on_connectBoard_clicked()
{
    try
    {
        serial = QSharedPointer<ArduinoSerial>(new ArduinoSerial(portList[ui->boardList->currentIndex()]));
    } catch (...)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Error: Couldn't open the port"));
        msgBox.exec();
        return;
    }
    QMessageBox msgBox;
    msgBox.setText(tr("Conectado"));
    msgBox.exec();
    boardConnected = 1;
}

void InitialWindow::on_connectCamera_clicked()
{
    //TODO
}

void InitialWindow::on_openNN_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "Neural Network Family (*.nnf)");
    if(!fileName.isEmpty())
    {
        if(!nnm.isNull() && nnm->isValid())
        {
            nnm->saveAndClose();
        }
        nnm = NN::NeuralNetworkManagerPointer(new NN::NeuralNetworkManager(fileName));
        if(!nnm->isValid())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Could not open file."));
            msgBox.exec();
            ui->nnConectionStatusLbl->setText(QString("<html><head/><body><p><span style=\" color:#ff0000;\">") + tr("Not Loaded") + QString("</span></p></body></html>"));
            ui->gameBox->setEnabled(false);
            ui->trainNN->setEnabled(false);
            return;
        }
        ui->nnConectionStatusLbl->setText(fileName);
        ui->gameBox->setEnabled(true);
        ui->trainNN->setEnabled(1);
    }
}

void InitialWindow::on_createNN_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), "Neural Network Family (*.nnf)");
    if(!fileName.isEmpty())
    {
        if(!nnm.isNull() && nnm->isValid())
        {
            nnm->saveAndClose();
        }
        if(QFile::exists(fileName) && !QFile::resize(fileName, 0))
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Could not create file."));
            msgBox.exec();
            ui->nnConectionStatusLbl->setText(QString("<html><head/><body><p><span style=\" color:#ff0000;\">") + tr("Not Loaded") + QString("</span></p></body></html>"));
            ui->gameBox->setEnabled(false);
            ui->trainNN->setEnabled(false);
            return;
        }
        TopologySelector *ts = new TopologySelector(this);
        ts->exec();
        NN::InternalTopology topology = ts->getTopology();
        nnm = NN::NeuralNetworkManager::createNewNNFamily(fileName, topology);
        if(!nnm->isValid())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Could not create file."));
            msgBox.exec();
            ui->nnConectionStatusLbl->setText(QString("<html><head/><body><p><span style=\" color:#ff0000;\">") + tr("Not Loaded") + QString("</span></p></body></html>"));
            ui->gameBox->setEnabled(false);
            ui->trainNN->setEnabled(false);
            return;
        }
        ui->nnConectionStatusLbl->setText(fileName);
        ui->gameBox->setEnabled(true);
        ui->trainNN->setEnabled(true);
    }
}

void InitialWindow::on_trainNN_clicked()
{
     nnt = new NNTraining(nnm, this);
     nnt->show();
}

void InitialWindow::on_openGame_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "Checkers Game File (*.cg)");
    if(!fileName.isEmpty())
    {
        if(!gameFile.isNull() && gameFile->isValid())
        {
            gameFile->saveAndClose();
        }
        gameFile = Game::GameFilePointer(new Game::GameFile(fileName));
        if(!gameFile->isValid())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Could not open file."));
            msgBox.exec();
            ui->gameAddressLbl->setText(tr("Not Loaded"));
            ui->playGame->setEnabled(false);
            brdWidget->disableBoard();
            return;
        }
        ui->gameAddressLbl->setText(fileName);
        if(gameFile->hasFinished())
            ui->playGame->setEnabled(false);
        else
            ui->playGame->setEnabled(true);
        brdWidget->setBoard(gameFile->getBoardData());
    }
}

void InitialWindow::on_newGame_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), "Checkers Game File (*.cg)");
    if(!fileName.isEmpty())
    {
        if(!gameFile.isNull() && gameFile->isValid())
        {
            gameFile->saveAndClose();
        }
        if(QFile::exists(fileName) && !QFile::resize(fileName, 0))
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Could not create file."));
            msgBox.exec();
            ui->gameAddressLbl->setText(tr("Not Loaded"));
            ui->playGame->setEnabled(false);
            brdWidget->disableBoard();
            return;
        }
        NewGameConfig *ngc = new NewGameConfig(this);
        ngc->exec();
        gameFile = Game::GameFile::createNewGame(fileName, ngc->getName(), ngc->getDifficulty());
        if(!gameFile->isValid())
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Error: Could not create file."));
            msgBox.exec();
            ui->gameAddressLbl->setText(tr("Not Loaded"));
            ui->playGame->setEnabled(false);
            brdWidget->disableBoard();
            return;
        }
        ui->gameAddressLbl->setText(fileName);
        ui->playGame->setEnabled(true);
        brdWidget->setBoard(gameFile->getBoardData());
    }
}

void InitialWindow::on_playGame_clicked()
{
    PlayWindow *pw = new PlayWindow(gameFile, nnm, this);
    pw->show();
    this->hide();
    connect(pw, &PlayWindow::closingSignal, this, &InitialWindow::updateBoardDisplayAndButton);
}

void InitialWindow::updateBoardDisplayAndButton()
{
    brdWidget->setBoard(gameFile->getBoardData());
    if(gameFile->hasFinished())
            ui->playGame->setEnabled(false);
    show();
}
