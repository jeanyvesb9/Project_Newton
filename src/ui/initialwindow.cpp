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
    if(!nnm.isNull() && nnm->isValid())
    {
        nnm->saveAndClose();
    }
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), "Neural Network Family (*.nnf)");
    if(!fileName.isEmpty())
    {
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
    if(!nnm.isNull() && nnm->isValid())
    {
        nnm->saveAndClose();
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::homePath(), "Neural Network Family (*.nnf)");
    if(!fileName.isEmpty())
    {
        NN::InternalTopology topology = {40, 10}; //TODO FIX WITH DIALOG
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

void InitialWindow::on_playGame_clicked()
{
    gameFile = Game::GameFile::createNewGame("");
    pw = new PlayWindow(gameFile, nnm, this);
}
