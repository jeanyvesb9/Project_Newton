#include <QApplication>

#include <QDebug>
#include <QDateTime>
#include <iostream>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/training/trainingengine.h"
#include "src/game/board.h"
#include "src/engine/gameengine.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"
#include "src/ui/initialwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    qRegisterMetaType<Game::MovePointer>();
    qRegisterMetaType<Game::BoardPointer>();
    qRegisterMetaType<NN::TrainingDataPointer>();
    qRegisterMetaType<NN::Status>();

    InitialWindow w;
    w.show();

    return app.exec();
}
