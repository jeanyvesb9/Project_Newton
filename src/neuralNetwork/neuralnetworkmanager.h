#ifndef NEURALNETWORKMANAGER_H
#define NEURALNETWORKMANAGER_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <exception> //TODO: HANDLE BAD FILES!! (PERMISSIONS/OPENING OR FORMAT[DB ERROR])

#include "src/neuralNetwork/neuralnetwork.h"

#include "src/utilityfunctions.h"

namespace NN {

struct NNContainer {
    NeuralNetworkPointer neuralNetwork;
    quint64 id;
    quint64 parent;
    qint64 generation;
    bool active;
    quint64 dateCreated;
};
using NNContainerPointer = QSharedPointer<NNContainer>;

class NeuralNetworkManager;
using NeuralNetworkManagerPointer = QSharedPointer<NeuralNetworkManager>;

enum NNManagerFlags { valid = 1, corruptFile = 2 };

class NeuralNetworkManager : public QObject
{
    Q_OBJECT
public:
    NeuralNetworkManager(QObject *parent = 0);
    NeuralNetworkManager(QString file, QObject *parent = 0);

    bool saveAndClose();
    inline bool isValid() const { return flags & NNManagerFlags::valid; }
    inline bool isFlagActive(NNManagerFlags flag) const { return flags & flag; }
    inline unsigned int getFlags() const { return flags; }

    static NeuralNetworkManagerPointer createNewNNFamily(QString file, InternalTopology *topology);

private:
    QSqlDatabase db;
    unsigned int flags;

    InternalTopology topology;
    QVector<NNContainerPointer> neuralNetworks;

    NNContainerPointer loadNNfromQuery(QSqlQuery *query) const;
    bool loadInMemory();
    bool saveNNToDB(NNContainerPointer ptr); //ADDS Id

};

}

#endif // NEURALNETWORKMANAGER_H
