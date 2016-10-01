#include "trainingengine.h"

using namespace NN;

TrainingEngine::TrainingEngine(NeuralNetworkManagerPointer manager, QObject *parent)
    : QObject(parent),
    state{true}
{
    originalNNs = manager->getActiveNNVector();

    for(int i = 0; i < 15; i++)
    {
        NNContainerPointer offspring(new NNContainer());
        offspring->neuralNetwork = originalNNs->at(i)->neuralNetwork->createChild();
        offspring->dateCreated = QDateTime::currentMSecsSinceEpoch();
        offspring->active = true;
        offspring->generation = manager->getNumberOfGenerations(); //Gives number of gens with gen 0, thus not needing to +1
        offspring->parent = originalNNs->at(i)->id;

        trainingData->newNNs.append(offspring);
    }
}

void TrainingEngine::startTraining()
{
    work();
    emit finished(trainingData);
}

void TrainingEngine::cancelTraining()
{
    mutex.lock();
    state = 0;
    mutex.unlock();
}

void TrainingEngine::work()
{
    timer.start();

    for(int i = 0; i < 30; i++) //For each NN...
    {
        QVector<bool> pool;
        pool.fill(true, 30);
        for(int j = 0; j < 5; j++) //For each of 5 games
        {
            QGuiApplication::processEvents();
            mutex.lock();
            if(!state)
            {
                emit hasCanceled();
                return;
            }

            int opponent = -1;
            while(opponent == -1)
            {
                int op = rand() % 30;
                if(pool.at(op))
                {
                    pool[op] = false;
                    opponent = op;
                }
            }

            NNContainerPointer player1 = i < 15 ? originalNNs->at(i) : trainingData->newNNs.at(i % 15);
            NNContainerPointer player2 = opponent < 15 ? originalNNs->at(opponent) : trainingData->newNNs.at(opponent % 15);

            mutex.unlock();
        }
    }

    trainingData->trainingTime = timer.elapsed();
}
