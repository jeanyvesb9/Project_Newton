#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QFontDatabase>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/game/board.h"
#include "src/engine/gameengine.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/arduino/arduinoserial.h"
#include "src/game/gamefile.h"
#include "src/ui/macwindow.h"
#include "src/ui/aspectratiowidget.h"
#include "src/ui/boardwidget.h"
#include "src/ui/playinfotable.h"
#include "src/ui/playmenu.h"

namespace Ui {
class PlayWindow;
}

class PlayWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PlayWindow(Game::GameFilePointer game, NN::NeuralNetworkManagerPointer nnm, QWidget *parent = 0);
    ~PlayWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_menu_btn_clicked();
    void on_exit_btn_clicked();

    void resumeGame();

    void madeMove(PlayerMovePointer move, Game::BoardData board);
    void selectedMove(Game::MovePointer move);
    void gameFinished(Player winner);
    void totalTimerUpdate(int time);

signals:
    void closingSignal();

private:
    Ui::PlayWindow *ui;
    QFont adobeCleanLight;
    QFont segoeUILight;

    BoardWidget *brdWidget;
    AspectRatioWidget *brd_rw;
    PlayInfoTable *piWidget;

    Game::GameFilePointer gameFile;

    GameEnginePointer gameEngine;
    NeuralNetworkPlayerPointer nnPlayer;
    ManualPlayerPointer manualPlayer;

    NN::NeuralNetworkPointer nn;

    void setPlayerTurnTo(Player player);

    quint64 totalTime;

    PlayMenu *pm;
};

#endif // PLAYWINDOW_H
