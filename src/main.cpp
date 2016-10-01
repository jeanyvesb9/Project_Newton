#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QDateTime>
#include <iostream>

#include "src/neuralNetwork/neuralnetworkmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    NN::InternalTopology topology = {40, 10};

    NN::NeuralNetworkManagerPointer manager(new NN::NeuralNetworkManager("/Users/jeanyves/Desktop/QT/Project_Newton/database_models/neuralNetwork_test.db"));
    qDebug() <<"Done";
    return app.exec();
}
