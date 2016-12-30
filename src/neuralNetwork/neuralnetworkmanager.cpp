#include "neuralnetworkmanager.h"

using namespace NN;

NeuralNetworkManager::NeuralNetworkManager(QObject *parent)
    : QObject(parent), flags{0}
{

}

NeuralNetworkManager::NeuralNetworkManager(QString file, QObject *parent)
    : QObject(parent), flags{0}
{
    db = QSqlDatabase::addDatabase("QSQLITE", "nnDb");
    db.setDatabaseName(file);
    if(!db.open())
    {
        //TODO: RAISE EXCEPTION
    }

    if(loadInMemory())
        this->flags |= NNManagerFlags::valid;
    else
    {
        //TODO: RAISE EXCEPTION
    }
}

NeuralNetworkManager::~NeuralNetworkManager()
{
    if(db.isOpen())
    {
        saveAndClose();
    }
}

bool NeuralNetworkManager::saveAndClose()
{
    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase("nnDb");
    flags = 0;
    return true;
}

bool NeuralNetworkManager::updateNNActiveStatusToDB(NNContainerPointer nn)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE networks SET Active = :active WHERE Id = :id"));
    query.bindValue(QStringLiteral(":active"), nn->active);
    query.bindValue(QStringLiteral(":id"), nn->id);
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return false;
    }
    return true;
}

quint64 NeuralNetworkManager::getNumberOfGenerations() const
{
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("SELECT MAX(Generation) FROM networks")))
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return 0;
    }
    query.next();
    return query.value(0).toULongLong() + 1;
}

quint64 NeuralNetworkManager::getTrainingInterations() const
{
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("SELECT MAX(Iteration) FROM Games")))
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return 0;
    }
    query.next();
    return query.value(0).toULongLong();
}

bool NeuralNetworkManager::addTrainingIteration(TrainingDataPointer trainingData)
{
    quint64 trainingIteration = getTrainingInterations() + 1;
    QSqlQuery query(db);
    query.prepare(QStringLiteral("UPDATE generalData SET TrainingTime = TrainingTime + :time, BestPerforming = :best"));
    query.bindValue(QStringLiteral(":time"), trainingData->trainingTime);
    query.bindValue(QStringLiteral(":best"), trainingData->bestPerforming->id);
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return false;
    }

    for(int i = 0; i < 15; i++)
    {
        if(trainingData->originalsToDeactivate.at(i))
        {
            activeNeuralNetworks.at(i)->active = false;
            updateNNActiveStatusToDB(activeNeuralNetworks.at(i));
        }
    }

    for(auto nn : trainingData->newNNs)
    {
        activeNeuralNetworks.append(nn);
        saveNNToDB(nn);
    }

    query.clear();
    for(auto game : trainingData->games)
    {
        query.prepare(QStringLiteral("INSERT INTO games (Iteration, Player1, Player2, Score) VALUES (:iteration, :p1, :p2, :score)"));
        query.bindValue(QStringLiteral(":iteration"), trainingIteration);
        query.bindValue(QStringLiteral(":p1"), game->nn1->id);
        query.bindValue(QStringLiteral(":p2"), game->nn2->id);
        query.bindValue(QStringLiteral(":score"), game->score);

        if(!query.exec())
        {
            //TODO: RAISE EXCEPTION: READ ERROR
            return false;
        }

        query.clear();
    }

    for(int i = 0; i < activeNeuralNetworks.size(); i++)
    {
        if(!activeNeuralNetworks.at(i)->active)
            activeNeuralNetworks.remove(i);
    }

    return true;
}

NNContainerPointer NeuralNetworkManager::getBestNNPerformer()
{
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("SELECT BestPerforming FROM generalData;")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.next();
    quint64 id = query.value(0).toULongLong();

    for(auto nn : activeNeuralNetworks)
    {
        if(nn->id == id)
            return nn;
    }

    //TODO: RAISE EXCEPTION
    return NNContainerPointer();
}

NeuralNetworkManagerPointer NeuralNetworkManager::createNewNNFamily(QString file, InternalTopology topology)
{
    NeuralNetworkManagerPointer ptr = NeuralNetworkManagerPointer(new NeuralNetworkManager());
    QSqlDatabase &db = ptr->db;
    db = QSqlDatabase::addDatabase("QSQLITE", "nnDb");
    db.setDatabaseName(file);
    if(!db.open())
    {
        //TODO: RAISE EXCEPTION
    }

    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("CREATE TABLE generalData (DateCreated INTEGER NOT NULL, TrainingTime INTEGER NOT NULL DEFAULT (0), BestPerforming INTEGER REFERENCES networks (Id));")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE internalLayers (Id INTEGER PRIMARY KEY AUTOINCREMENT DEFAULT (0), Neurons INTEGER NOT NULL DEFAULT (1));")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE networks (Id INTEGER PRIMARY KEY AUTOINCREMENT, DateCreated INTEGER NOT NULL, Parent INTEGER REFERENCES networks (Id), Generation INTEGER NOT NULL DEFAULT (0), Active BOOLEAN NOT NULL DEFAULT (1), KingValue TEXT NOT NULL);")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE weights (Network INTEGER REFERENCES networks (Id), Layer INTEGER, Neuron INTEGER, NeuronWeight INTEGER, Value TEXT NOT NULL, Sigma TEXT NOT NULL, PRIMARY KEY (Network, Layer, Neuron, NeuronWeight));")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    if(!query.exec(QStringLiteral("CREATE TABLE games (Id INTEGER PRIMARY KEY AUTOINCREMENT, Iteration INTEGER NOT NULL DEFAULT (0), Player1 INTEGER REFERENCES networks (Id) NOT NULL, Player2 INTEGER REFERENCES networks (Id) NOT NULL, Score INTEGER NOT NULL);")))
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();

    ptr->topology = topology;
    for(auto layer : topology)
    {
        query.prepare(QStringLiteral("INSERT INTO internalLayers (Neurons) VALUES (:layer)"));
        query.bindValue(QStringLiteral(":layer"), layer);
        if(!query.exec())
        {
            //TODO: RAISE EXCEPTION
        }
        query.clear();
    }

    for(int i = 0; i < 15; i++)
    {
        NNContainerPointer nn(new NNContainer);
        nn->neuralNetwork = NeuralNetwork::createGenerationZeroNN(&(ptr->topology));
        nn->parent = 0;
        nn->generation = 0;
        nn->active = true;
        nn->dateCreated = QDateTime::currentMSecsSinceEpoch();

        ptr->activeNeuralNetworks.append(nn);
        ptr->saveNNToDB(nn);
    }

    query.prepare(QStringLiteral("INSERT INTO generalData VALUES (:date, 0, 1)"));
    query.bindValue(QStringLiteral(":date"), QDateTime::currentMSecsSinceEpoch());
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
    }
    query.clear();
    ptr->getBestNNPerformer();

    ptr->flags |= NNManagerFlags::valid;
    return ptr;
}

NNContainerPointer NeuralNetworkManager::loadNNfromQuery(QSqlQuery *query) const
{
    NNContainerPointer ptr = NNContainerPointer(new NNContainer());
    ptr->id = query->value(QStringLiteral("Id")).toULongLong();
    ptr->dateCreated = query->value(QStringLiteral("Datecreated")).toULongLong();
    if(query->value(QStringLiteral("Parent")).isNull())
        ptr->parent = 0;
    else
        ptr->parent = query->value(QStringLiteral("Parent")).toULongLong();
    ptr->generation = query->value(QStringLiteral("Generation")).toULongLong();
    ptr->active = query->value(QStringLiteral("Active")).toBool();

    QVector<long double> rawData;

    QSqlQuery dataQuery(db);
    dataQuery.prepare(QStringLiteral("SELECT * FROM weights WHERE weights.Network == :id ORDER BY weights.Layer ASC, weights.Neuron ASC"));
    dataQuery.bindValue(QStringLiteral(":id"), ptr->id);
    if(!dataQuery.exec())
    {
        //TODO: ADD CHECKING
    }
    while(dataQuery.next())
    {
        rawData.append(QStringToLongDouble(dataQuery.value(QStringLiteral("Value")).toString()));
        rawData.append(QStringToLongDouble(dataQuery.value(QStringLiteral("Sigma")).toString()));
    }

    QString kingText = query->value(QStringLiteral("KingValue")).toString();
    long double king = QStringToLongDouble(&kingText);
    NNTopologyDataPointer nnTD = NNTopologyDataPointer(new NNTopologyData(&rawData, &topology, king));
    ptr->neuralNetwork = NeuralNetworkPointer(new NeuralNetwork(nnTD));

    return ptr;
}

bool NeuralNetworkManager::loadInMemory()
{
    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("SELECT * FROM internalLayers")))
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return false;
    }
    if(!querySize(&query))
    {
        //TODO: RAISE EXCEPTION: CORRUPT OR UNINITIALIZED FILE!!
        return false;
    }
    while(query.next())
    {
        topology.append(query.value(QStringLiteral("Neurons")).toUInt());
    }
    query.clear();

    if(!query.exec(QStringLiteral("SELECT * FROM networks WHERE networks.Active == 1")))
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return false;
    }
    if(querySize(&query) < 15)
    {
        //TODO: RAISE EXCEPTION: CORRUPT OR UNINITIALIZED FILE!!
        return false;
    }
    while(query.next())
    {
        activeNeuralNetworks.append(loadNNfromQuery(&query));
    }

    return true;
}

bool NeuralNetworkManager::saveNNToDB(NNContainerPointer ptr)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO networks (DateCreated, Parent, Generation, Active, KingValue) VALUES (:date, :parent, :gen, :active, :king)"));
    query.bindValue(QStringLiteral(":date"), ptr->dateCreated);
    if(!ptr->generation)
        query.bindValue(QStringLiteral(":parent"), QVariant());
    else
        query.bindValue(QStringLiteral(":parent"), ptr->parent);
    query.bindValue(QStringLiteral(":gen"), ptr->generation);
    query.bindValue(QStringLiteral(":active"), ptr->active);
    query.bindValue(QStringLiteral(":king"), longDoubleToQString(ptr->neuralNetwork->getDataStruct()->king));
    if(!query.exec())
    {
        //TODO: RAISE EXCEPTION
        return false;
    }

    query.clear();
    if(!query.exec(QStringLiteral("SELECT last_insert_rowid()")))
    {
        //TODO: RAISE EXCEPTION: READ ERROR
        return false;
    }
    query.next();
    ptr->id = query.value(0).toULongLong();

    query.clear();
    NNTopologyDataPointer nnData = ptr->neuralNetwork->getDataStruct();
    for(int i = 0; i < nnData->weights.size(); i++) //layer
    {
        for(int j = 0; j < nnData->weights.at(i).size(); j++) //Neuron
        {
            for(int k = 0; k < nnData->weights.at(i).at(j).size(); k++) //NeuronData
            {
                query.prepare(QStringLiteral("INSERT INTO weights VALUES (:network, :layer, :neuron, :neuronWeight, :value, :sigma)"));
                query.bindValue(QStringLiteral(":network"), ptr->id);
                query.bindValue(QStringLiteral(":layer"), i + 1);
                query.bindValue(QStringLiteral(":neuron"), j + 1);
                query.bindValue(QStringLiteral(":neuronWeight"), k + 1);
                query.bindValue(QStringLiteral(":value"), longDoubleToQString(nnData->weights.at(i).at(j).at(k).weight));
                query.bindValue(QStringLiteral(":sigma"), longDoubleToQString(nnData->weights.at(i).at(j).at(k).sigma));

                query.exec();
                query.clear();
            }
        }
    }

    return true;
}
