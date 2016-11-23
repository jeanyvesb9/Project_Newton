#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QDialog>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/game/board.h"
#include "src/engine/gameengine.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/arduino/arduinoserial.h"
#include "src/game/gamefile.h"
#include "src/ui/aspectratiowidget.h"
#include "src/ui/boardwidget.h"

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlayWindow(Game::GameFilePointer game, NN::NeuralNetworkManagerPointer nnm, QWidget *parent = 0);
    ~PlayWindow();

private slots:
    void on_menu_btn_clicked();
    void on_exit_btn_clicked();

    void madeMove(PlayerMovePointer move);
    void selectedMove(Game::MovePointer move);
private:
    Ui::PlayWindow *ui;

    BoardWidget *brdWidget;
    AspectRatioWidget *brd_rw;

    GameEnginePointer gameEngine;
    NeuralNetworkPlayerPointer nnPlayer;
    ManualPlayerPointer manualPlayer;

    NN::NeuralNetworkPointer nn;

};

#endif // PLAYWINDOW_H
