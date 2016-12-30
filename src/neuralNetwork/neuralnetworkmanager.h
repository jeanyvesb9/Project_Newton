#ifndef NEURALNETWORKMANAGER_H
#define NEURALNETWORKMANAGER_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <exception> //TODO: HANDLE BAD FILES!! (PERMISSIONS/OPENING OR FORMAT[DB ERROR])

#include "src/neuralNetwork/neuralnetwork.h"
#include "src/training/trainingstructs.h"

#include "src/utilityfunctions.h"

namespace NN {

enum NNManagerFlags { valid = 1, corruptFile = 2 };

class NeuralNetworkManager;
using NeuralNetworkManagerPointer = QSharedPointer<NeuralNetworkManager>;

class NeuralNetworkManager : public QObject
{
    Q_OBJECT

    explicit NeuralNetworkManager(QObject *parent = 0);
public:
    explicit NeuralNetworkManager(QString file, QObject *parent = 0);
    ~NeuralNetworkManager();

    bool saveAndClose();
    inline bool isValid() const { return flags & NNManagerFlags::valid; }
    inline bool isFlagActive(NNManagerFlags flag) const { return flags & flag; }
    inline unsigned int getFlags() const { return flags; }

    bool updateNNActiveStatusToDB(NNContainerPointer nn);
    inline QVector<NNContainerPointer>* getActiveNNVector() { return &activeNeuralNetworks; }

    quint64 getNumberOfGenerations() const; //Returns number of gens INCLUDING GEN 0!
    quint64 getTrainingInterations() const;
    bool addTrainingIteration(TrainingDataPointer trainingData);

    NNContainerPointer getBestNNPerformer();

    static NeuralNetworkManagerPointer createNewNNFamily(QString file, InternalTopology topology);

private:
    QSqlDatabase db;
    unsigned int flags;

    InternalTopology topology;
    QVector<NNContainerPointer> activeNeuralNetworks;

    NNContainerPointer loadNNfromQuery(QSqlQuery *query) const;
    bool loadInMemory();
    bool saveNNToDB(NNContainerPointer ptr); //ADDS Id
};

}

#endif // NEURALNETWORKMANAGER_H
