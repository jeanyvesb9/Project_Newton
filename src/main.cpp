#include <QGuiApplication>
//#include <QQmlApplicationEngine>
#include <QApplication>

#include <QDebug>
#include <QDateTime>
#include <iostream>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/game/board.h"
#include "src/engine/gameengine.h"
#include "src/player/manualplayer.h"
#include "src/player/neuralnetworkplayer.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QGuiApplication app(argc, argv);
    QApplication a(argc, argv);

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    NN::InternalTopology topology = {40, 10};

    //NN::NeuralNetworkManagerPointer manager = NN::NeuralNetworkManager::createNewNNFamily("/Users/jeanyves/Desktop/QT/Project_Newton/database_models/neuralNetwork_test.db", topology);
    //NN::NeuralNetworkManagerPointer manager(new NN::NeuralNetworkManager("/Users/jeanyves/Desktop/QT/Project_Newton/database_models/neuralNetwork_test.db"));
    //qDebug() <<"Done";

    NN::NeuralNetworkPointer nn1 = NN::NeuralNetwork::createGenerationZeroNN(&topology);
    NeuralNetworkPlayerPointer p1 = NeuralNetworkPlayerPointer(new NeuralNetworkPlayer(Game::Side::PlayerSide, nn1, 3));

    NN::NeuralNetworkPointer nn2 = NN::NeuralNetwork::createGenerationZeroNN(&topology);
    NeuralNetworkPlayerPointer p2 = NeuralNetworkPlayerPointer(new NeuralNetworkPlayer(Game::Side::OpponentSide, nn2, 3));

    GameEnginePointer engine = GameEnginePointer(new GameEngine(p1, p2));

    return a.exec();
    //return app.exec();
}
