#include "neuralnetworkmanager.h"

using namespace NN;

NeuralNetworkManager::NeuralNetworkManager(QObject *parent)
    : QObject(parent), flags{0}
{

}

NeuralNetworkManager::NeuralNetworkManager(QString file, QObject *parent)
    : QObject(parent), flags{0}
{
    db = QSqlDatabase::addDatabase("QSQLITE");
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

bool NeuralNetworkManager::saveAndClose()
{
    db.close();
    flags = 0;
    return true;
}

NeuralNetworkManagerPointer NeuralNetworkManager::createNewNNFamily(QString file, InternalTopology *topology)
{
    NeuralNetworkManagerPointer ptr = NeuralNetworkManagerPointer(new NeuralNetworkManager());
    QSqlDatabase &db = ptr->db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(file);
    if(!db.open())
    {
        //TODO: RAISE EXCEPTION
    }

    QSqlQuery query(db);
    if(!query.exec(QStringLiteral("CREATE TABLE generalData (DateCreated DATETIME NOT NULL, TrainingTime INTEGER NOT NULL DEFAULT (0), TrainingIterations INTEGER NOT NULL DEFAULT (0));")))
    {
        //TODO: RAISE EXCEPTION
    }
    if(!query.exec(QStringLiteral("CREATE TABLE internalLayers (Id INTEGER PRIMARY KEY AUTOINCREMENT DEFAULT (0), Neurons INTEGER NOT NULL DEFAULT (1));")))
    {
        //TODO: RAISE EXCEPTION
    }
    if(!query.exec(QStringLiteral("CREATE TABLE networks (Id INTEGER PRIMARY KEY AUTOINCREMENT, DateCreated DATETIME NOT NULL, Parent INTEGER REFERENCES networks (Id), Generation INTEGER NOT NULL DEFAULT (0), Active BOOLEAN NOT NULL DEFAULT (1), KingValue TEXT NOT NULL);")))
    {
        //TODO: RAISE EXCEPTION
    }
    if(!query.exec(QStringLiteral("CREATE TABLE weights (Network INTEGER REFERENCES networks (Id), Layer INTEGER REFERENCES internalLayers (Id), Neuron INTEGER, Value TEXT NOT NULL, Sigma TEXT NOT NULL, PRIMARY KEY (Network, Layer, Neuron));")))
    {
        //TODO: RAISE EXCEPTION
    }
    if(!query.exec(QStringLiteral("CREATE TABLE games (Id INTEGER PRIMARY KEY AUTOINCREMENT, Generation INTEGER NOT NULL DEFAULT (0), Player1 INTEGER REFERENCES networks (Id) NOT NULL, Player2 INTEGER REFERENCES networks (Id) NOT NULL, Score INTEGER NOT NULL);")))
    {
        //TODO: RAISE EXCEPTION
    }



    ptr->flags |= NNManagerFlags::valid;
    return ptr;
}

NNContainerPointer NeuralNetworkManager::loadNNfromQuery(QSqlQuery *query) const
{
    NNContainerPointer ptr = NNContainerPointer(new NNContainer());
    ptr->id = query->value(QStringLiteral("Id")).toULongLong();
    ptr->dateCreated = query->value(1).toULongLong(); //FIX get date
    if(query->value(2).isNull())
        ptr->parent = -1;
    else
        ptr->parent = query->value(2).toLongLong();
    ptr->generation = query->value(3).toULongLong();
    ptr->active = query->value(4).toBool();

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
        rawData.append(QStringToLongDouble(dataQuery.value(3).toString()));
        rawData.append(QStringToLongDouble(dataQuery.value(4).toString()));
    }

    QString kingText = query->value(5).toString();
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
        topology.append(query.value(1).toUInt());
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
        neuralNetworks.append(loadNNfromQuery(&query));
    }

    return true;
}

bool NeuralNetworkManager::saveNNToDB(NNContainerPointer ptr)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("INSERT INTO networks (DateCreated, Parent, Generation, Active, KingValue) VALUES (datetime('now'), :parent, :gen, 1, :king)"));
    query.bindValue(QStringLiteral(":parent"), ptr->parent);
    query.bindValue(QStringLiteral(":gen"), ptr->generation);
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
    return true;
}
