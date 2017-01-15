#ifndef PLAYWINDOW_H
#define PLAYWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QFontDatabase>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/game/board.h"
#include "src/engine/gameengine.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/arduino/arduinoserial.h"
#include "src/game/gamefile.h"
#include "src/camera/cameraanalyzer.h"
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
    explicit PlayWindow(Game::GameFilePointer game, NN::NeuralNetworkManagerPointer nnm, ArduinoSerialPointer arduinoSerial, CameraAnalyzerPointer cameraInterface, QWidget *parent = 0);
    ~PlayWindow();

    enum GameMode { SoftwareOnly, WithBoard };

public slots:
    void quitGame();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void setGameMode(GameMode mode);
    void handleCameraOutput(QVector<BoardPiece> rawVector);
    void handleWrongManualPlayerMove();

    void on_menu_btn_clicked();
    void on_exit_btn_clicked();

    void nextClicked();
    void okClicked();
    void okLongPressed();

    void resumeGame();

    void madeMove(PlayerMovePointer move, Game::BoardData board);
    void selectedMove(Game::MovePointer move);
    void gameFinished(Player winner);
    void totalTimerUpdate(int time);

signals:
    void newGame();
    void closingSignal();

private:
    enum GameWithBoardState { InitialCalibration, ComputerPlaying, UserPlaying, ComputerShowingMoves, CheckingUserMoves, ShowingUserBrdWidgetMoves };

    Ui::PlayWindow *ui;
    GameMode gameMode;
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

    ArduinoSerialPointer arduinoSerial;
    CameraAnalyzerPointer camera;
    QMetaObject::Connection cameraErrorHandlingConnection;
    GameWithBoardState gameState;
    Game::MovePointer auxMove;
    Game::BoardPointer previousBoard;

    void setPlayerTurnTo(Player player);

    quint64 totalTime;

    PlayMenu *pm;
};

#endif // PLAYWINDOW_H
