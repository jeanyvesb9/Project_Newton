#include "initialwindow.h"
#include "ui_initialwindow.h"

InitialWindow::InitialWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::InitialWindow), ccdw{nullptr}, nnt{nullptr}, pw{nullptr}
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Project Newton"));
    this->setMinimumSize(800, 600);

    portList = ArduinoSerial::getSerialPorts();
    for(auto &item : portList)
    {
        ui->boardList->addItem(item.portName());
    }

    brdWidget = new BoardWidget();
    brd_rw = new AspectRatioWidget(brdWidget, ui->gameBox->layout()->widget());
    brdWidget->setParent(brd_rw);
    ui->gameBox->layout()->addWidget(brd_rw);

    ui->trainNN->setEnabled(false);
    ui->calibrateCamera->setEnabled(false);
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
        arduinoSerial = ArduinoSerialPointer(new ArduinoSerial(portList[ui->boardList->currentIndex()]));
    } catch (...)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Error: Couldn't open the port"));
        msgBox.exec();
        ui->gameMode->setText(tr("Game Mode: Software Only"));
        return;
    }
    if(camera)
    {
        ui->gameMode->setText(tr("Game Mode: With Board"));
    }
    ui->boardList->setEnabled(false);
    ui->connectBoard->setEnabled(false);
    ui->boardConnectionStatus->setText(tr("Connected"));
}

void InitialWindow::on_selectCamera_clicked()
{
    if(ccdw)
    {
        ccdw->close();
        ccdw->deleteLater();
    }
    CameraAnalyzer *prevCam = camera.data();
    CameraSelector *cs = new CameraSelector(&camera, this);
    cs->exec();
    cs->deleteLater();
    if(camera)
    {
        if(camera.data() != prevCam)
        {
            if(arduinoSerial)
            {
                ui->gameMode->setText(tr("Game Mode: With Board"));
            }
            ui->cameraConnectionStatus->setText(tr("Connected to \"") + camera->getCameraDeviceInfo().name + QStringLiteral("\""));
            ui->calibrateCamera->setEnabled(true);
            ccdw = new CameraCalibrationDisplay(camera, true, true, this);
            ccdw->show();
            QObject::connect(ccdw, &CameraCalibrationDisplay::closed, this, &InitialWindow::cameraCalibrationDisplayClosed);
        }
        else
        {
            camera->stopCapture();
        }
    }
    else
    {
        ui->gameMode->setText(tr("Game Mode: Software Only"));
        ui->cameraConnectionStatus->setText(QStringLiteral("<html><head/><body><p><span style=\" color:#0000ff;\">") + tr("Not Connected") + QStringLiteral("</span></p></body></html>"));
        ui->calibrateCamera->setEnabled(false);
    }
}

void InitialWindow::on_calibrateCamera_clicked()
{
    if(!ccdw)
    {
        camera->startCapture();
        ccdw = new CameraCalibrationDisplay(camera, false, true, this);
        ccdw->show();
        QObject::connect(ccdw, &CameraCalibrationDisplay::closed, this, &InitialWindow::cameraCalibrationDisplayClosed);
    }
}

void InitialWindow::cameraCalibrationDisplayClosed()
{
    ccdw->deleteLater();
    if(!camera->getErrorState())
    {
        camera->stopCapture();
        if(!pw)
        {
            this->cameraErrorHandlingConnection = QObject::connect(camera.data(), &CameraAnalyzer::cameraError, this, &InitialWindow::cameraLocalErrorHandler);
        }
    }
    else
    {
        ui->gameMode->setText(tr("Game Mode: Software Only"));
        ui->cameraConnectionStatus->setText(QStringLiteral("<html><head/><body><p><span style=\" color:#0000ff;\">") + tr("Not Connected") + QStringLiteral("</span></p></body></html>"));
        ui->calibrateCamera->setEnabled(false);
    }
}

void InitialWindow::cameraLocalErrorHandler(QString error)
{
    QMessageBox msgBox;
    msgBox.setText(error);
    msgBox.exec();
    cameraRemoteErrorHandler();
}

void InitialWindow::cameraRemoteErrorHandler()
{
    ui->cameraConnectionStatus->setText(QStringLiteral("<html><head/><body><p><span style=\" color:#0000ff;\">") + tr("Not Connected") + QStringLiteral("</span></p></body></html>"));
    ui->calibrateCamera->setEnabled(false);
    this->camera.reset();
    ui->gameMode->setText(tr("Game Mode: Software Only"));
}

void InitialWindow::on_openNN_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "Neural Network Family (*.nnf)");
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), "Neural Network Family (*.nnf)");
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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), "Checkers Game File (*.cg)");
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
    pw = new PlayWindow(gameFile, nnm, arduinoSerial, camera, this);
    pw->show();
    this->hide();
    if(ccdw)
    {
        ccdw->setErrorHandlingResponsibility(false);
    }
    QObject::disconnect(cameraErrorHandlingConnection);
    if(!camera->getErrorState())
    {
        this->cameraErrorHandlingConnection = QObject::connect(camera.data(), &CameraAnalyzer::cameraError, this, &InitialWindow::cameraRemoteErrorHandler);
    }
    QObject::connect(pw, &PlayWindow::closingSignal, this, &InitialWindow::handlePlayWindowClosure);
    QObject::connect(pw, &PlayWindow::newGame, this, &InitialWindow::newGameFromPlayWindow);
}

void InitialWindow::handlePlayWindowClosure()
{
    brdWidget->setBoard(gameFile->getBoardData());
    if(gameFile->hasFinished())
            ui->playGame->setEnabled(false);
    QObject::disconnect(cameraErrorHandlingConnection);
    if(!camera->getErrorState())
    {
        if(ccdw)
        {
            ccdw->setErrorHandlingResponsibility(true);
        }
        else
        {
            this->cameraErrorHandlingConnection = QObject::connect(camera.data(), &CameraAnalyzer::cameraError, this, &InitialWindow::cameraLocalErrorHandler);
            camera->stopCapture();
        }
    }
    show();
    pw->deleteLater();
    pw = nullptr;
}

void InitialWindow::newGameFromPlayWindow()
{
    QObject::disconnect(cameraErrorHandlingConnection);
    if(!camera->getErrorState())
    {
        if(ccdw)
        {
            ccdw->setErrorHandlingResponsibility(true);
        }
        else
        {
            this->cameraErrorHandlingConnection = QObject::connect(camera.data(), &CameraAnalyzer::cameraError, this, &InitialWindow::cameraLocalErrorHandler);
        }
    }

    QFileInfo origFile(gameFile->getFileName());
    QDir folder = origFile.dir();
    QString origFileName = origFile.fileName().remove(QRegExp(QStringLiteral("_\\d+\\.cg$|\\.cg$")));
    QStringList fileList = folder.entryList({"*.cg"});
    int maxIndex = 0;
    for(auto &fpn : fileList)
    {
        QString f = QFileInfo(fpn).fileName();
        QString fTrunc = f;
        if(f.contains(QRegExp(QStringLiteral("_\\d+\\.cg$"))) && fTrunc.remove(QRegExp(QStringLiteral("_\\d+\\.cg$"))) == origFileName)
        {
            int endIter = f.size() - 1;
            while(f.at(endIter) != '_')
            {
                endIter--;
            }
            endIter++;
            int indexNumber = f.mid(endIter, f.size() - endIter - 3).toInt();
            if(indexNumber > maxIndex)
            {
                maxIndex = indexNumber;
            }
        }
    }
    QString suffix = QStringLiteral("_") + QString::number(maxIndex + 1) + QStringLiteral(".cg");
    if(suffix.size() == 5)
    {
        suffix.insert(1, '0');
    }
    QString newFileName = folder.absolutePath() + QStringLiteral("/") + origFileName + suffix;
    QString playerName = gameFile->getPlayerName();
    quint8 difficulty = gameFile->getDifficulty();

    QObject::disconnect(pw, &PlayWindow::closingSignal, this, &InitialWindow::handlePlayWindowClosure);
    gameFile->saveAndClose();
    gameFile.reset();
    gameFile = Game::GameFile::createNewGame(newFileName, playerName, difficulty);
    if(!gameFile->isValid())
    {
        show();
        pw->close();
        QMessageBox msgBox;
        msgBox.setText(tr("Error: Could not create file."));
        msgBox.exec();
        ui->gameAddressLbl->setText(tr("Not Loaded"));
        ui->playGame->setEnabled(false);
        brdWidget->disableBoard();
    }
    else
    {
        ui->gameAddressLbl->setText(newFileName);
        ui->playGame->setEnabled(true);
        brdWidget->setBoard(gameFile->getBoardData());

        PlayWindow *npw = new PlayWindow(gameFile, nnm, arduinoSerial, camera, this);
        npw->show();
        if(pw->isMaximized())
        {
            npw->showMaximized();
        }
        else if(pw->isFullScreen())
        {
            npw->showFullScreen();
        }
        else
        {
            npw->resize(pw->width(), pw->height());
            npw->move(pw->x(), pw->y());
        }

        QObject::connect(npw, &PlayWindow::closingSignal, this, &InitialWindow::handlePlayWindowClosure);
        QObject::connect(npw, &PlayWindow::newGame, this, &InitialWindow::newGameFromPlayWindow);
        QObject::disconnect(cameraErrorHandlingConnection);
        if(!camera->getErrorState())
        {
            this->cameraErrorHandlingConnection = QObject::connect(camera.data(), &CameraAnalyzer::cameraError, this, &InitialWindow::cameraRemoteErrorHandler);
        }
        pw->quitGame();
        pw = npw;
    }
}
