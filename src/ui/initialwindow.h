#ifndef INITIALWINDOW_H
#define INITIALWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QMessageBox>
#include <QTextEdit>

#include "src/arduino/arduinoserial.h"
#include "src/engine/gameengine.h"
#include "src/game/board.h"
#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/training/trainingengine.h"
#include "src/ui/nntraining.h"
#include "src/ui/boardwidget.h"
#include "src/ui/aspectratiowidget.h"
#include "src/ui/playwindow.h"

namespace Ui {
class InitialWindow;
}

class InitialWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InitialWindow(QWidget *parent = 0);
    ~InitialWindow();

private slots:
    void on_connectBoard_clicked();

    void on_connectCamera_clicked();

    void on_openNN_clicked();

    void on_createNN_clicked();

    void on_trainNN_clicked();

    void on_playGame_clicked();

private:
    Ui::InitialWindow *ui;
    QList<QSerialPortInfo> portList;
    QSharedPointer<ArduinoSerial> serial;

    NN::NeuralNetworkManagerPointer nnm;
    NNTraining *nnt;

    bool boardConnected;
    bool cameraConnected;

    BoardWidget *brdWidget;
    Game::BoardPointer brd;
    AspectRatioWidget *brd_rw;

    Game::GameFilePointer gameFile;
    PlayWindow *pw;
};

#endif // INITIALWINDOW_H
