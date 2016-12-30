#include <QApplication>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/training/trainingengine.h"
#include "src/game/board.h"
#include "src/engine/gameengine.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/ui/initialwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_EnableHighDpiScaling);

    qRegisterMetaType<Game::MovePointer>();
    qRegisterMetaType<Game::BoardPointer>();
    qRegisterMetaType<Game::BoardData>();
    qRegisterMetaType<NN::TrainingDataPointer>();
    qRegisterMetaType<NN::Status>();
    qRegisterMetaType<QVector<BoardPiece>>();

    InitialWindow w;
    w.show();

    return app.exec();
}
