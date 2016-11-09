#include <QGuiApplication>
//#include <QQmlApplicationEngine>
#include <QApplication>

#include <QDebug>
#include <QDateTime>
#include <iostream>

#include "src/neuralNetwork/neuralnetworkmanager.h"
#include "src/game/board.h"

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QGuiApplication app(argc, argv);
    QApplication a(argc, argv);

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    //NN::InternalTopology topology = {40, 10};

    //NN::NeuralNetworkManagerPointer manager = NN::NeuralNetworkManager::createNewNNFamily("/Users/jeanyves/Desktop/QT/Project_Newton/database_models/neuralNetwork_test.db", topology);
    //NN::NeuralNetworkManagerPointer manager(new NN::NeuralNetworkManager("/Users/jeanyves/Desktop/QT/Project_Newton/database_models/neuralNetwork_test.db"));
    //qDebug() <<"Done";

    Game::BoardData d = fileToBoard("/Users/jeanyves/Desktop/QT/board.brd");
    Game::BoardPointer brd = Game::BoardPointer(new Game::Board(d));

    QVector<Game::BoardPointer> brds;
    Game::BoardPointer nbrd;
    do
    {
        nbrd = brd->treeBranchGenerator();
        if(!nbrd.isNull())
        {
            qDebug().noquote() <<nbrd->getBoardString();
            brds.append(nbrd);
        }
    } while(!nbrd.isNull());

    return a.exec();
    //return app.exec();
}
