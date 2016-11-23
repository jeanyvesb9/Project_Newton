#ifndef NNTRAINING_H
#define NNTRAINING_H

#include <QDialog>

#include "src/arduino/arduinoserial.h"
#include "src/engine/gameengine.h"
#include "src/game/board.h"
#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/training/trainingengine.h"
#include "src/ui/boardwidget.h"
#include "src/ui/aspectratiowidget.h"

namespace Ui {
class NNTraining;
}

class NNTraining : public QDialog
{
    Q_OBJECT

public:
    explicit NNTraining(NN::NeuralNetworkManagerPointer ptr, QWidget *parent = 0);
    ~NNTraining();

private slots:
    void on_startBtn_clicked();
    void on_stopBtn_clicked();
    void on_closeBtn_clicked();

    void statusUpdate(NN::Status statusUpdate, Game::BoardData board, int iteration, int totalIterations, int moves, int maxMoves, int games);
    void beginDbSave();
    void hasFinished(int iterations);
    void hasCanceled(int iterations);

private:
    Ui::NNTraining *ui;
    BoardWidget *brdWidget;

    NN::NeuralNetworkManagerPointer nnm;

    NN::TrainingEnginePointer te;
    AspectRatioWidget *brd_rw;

    void setUiStop();
};

#endif // NNTRAINING_H
