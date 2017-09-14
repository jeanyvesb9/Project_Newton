#include "trainingengine.h"

using namespace NN;

TrainingEngine::TrainingEngine(NeuralNetworkManagerPointer manager, int depth, QObject *parent)
    : QObject(parent),
    status{Status::Stopped}, depth{depth}, manager{manager}
{

}

void TrainingEngine::startTraining(int iterations, int maxMoves, int searchDepth)
{
    if(status == Status::Stopped)
    {
        this->iteration = 0;
        this->totalIterations = iterations;
        this->moves = 0;
        this->tieMaxMoves = maxMoves;
        this->status = Status::Running;
        this->depth = searchDepth;
        timer.start();
        newIteration();
    }
}

void TrainingEngine::cancelTraining()
{
    status = Status::StopRequested;
}

void TrainingEngine::newIteration()
{
    iteration++;

    trainingData = TrainingDataPointer(new TrainingData);
    originalNNs = manager->getActiveNNVector();
    quint64 maxId = 0;
    for(auto &nn : *originalNNs)
    {
        if(nn->id > maxId)
        {
            maxId = nn->id;
        }
    }
    maxId++;

    for(int i = 0; i < 15; i++)
    {
        NNContainerPointer offspring(new NNContainer());
        offspring->neuralNetwork = originalNNs->at(i)->neuralNetwork->createChild();
        offspring->dateCreated = QDateTime::currentMSecsSinceEpoch();
        offspring->active = true;
        offspring->generation = manager->getNumberOfGenerations(); //Gives number of gens with gen 0, thus not needing to +1
        offspring->parent = originalNNs->at(i)->id;
        offspring->id = maxId + i; //TEMPORAL VALUE FOR SCORE ACCOUNTING - FIXED IN MANAGER

        trainingData->newNNs <<offspring;
    }

    i = 0;
    j = 0;
    pool.fill(true, 30);

    startNewGame();
}

void TrainingEngine::startNewGame()
{
    if(status == Status::StopRequested)
    {
        status = Status::Stopped;
        emit hasCanceled(iteration - 1);
        return;
    }
    moves = 0;

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

    player1 = i < 15 ? originalNNs->at(i) : trainingData->newNNs.at(i % 15);
    player2 = opponent < 15 ? originalNNs->at(opponent) : trainingData->newNNs.at(opponent % 15);

    nnp1 = new NeuralNetworkPlayer(Game::Side::PlayerSide, player1->neuralNetwork, depth);
    nnp2 = new NeuralNetworkPlayer(Game::Side::OpponentSide, player2->neuralNetwork, depth);

    engine = GameEnginePointer(new GameEngine(nnp1, nnp2, tieMaxMoves));
    QObject::connect(engine.data(), &GameEngine::madeMove, this, &TrainingEngine::madeMove);
    QObject::connect(engine.data(), &GameEngine::hasWon, this, &TrainingEngine::gameWon);
    QObject::connect(engine.data(), &GameEngine::hasTied, this, &TrainingEngine::hasTied);

    emit statusUpdate(this->status, this->engine->getBoard()->getBoardData(), this->iteration, this->totalIterations, this->moves, this->tieMaxMoves, this->getGames());
}

void TrainingEngine::madeMove(PlayerMovePointer move, Game::BoardData board)
{
    Q_UNUSED(move)
    moves++;
    emit statusUpdate(this->status, board, this->iteration, this->totalIterations, this->moves, this->tieMaxMoves, this->getGames());
}

void TrainingEngine::gameWon(Player winner)
{
    TrainingGamePointer g = TrainingGamePointer(new TrainingGame());
    g->nn1 = this->player1;
    g->nn2 = this->player2;
    g->score = winner == Player::Player1 ? 1 : -1; //VALUE REPLACED IN END FN

    this->trainingData->games.append(g);
    nextTrain();
}

void TrainingEngine::hasTied()
{
    TrainingGamePointer g = TrainingGamePointer(new TrainingGame());
    g->nn1 = this->player1;
    g->nn2 = this->player2;
    g->score = 0;

    this->trainingData->games.append(g);
    nextTrain();
}

Status TrainingEngine::getStatus() const
{
    return status;
}

void TrainingEngine::nextTrain()
{
    QObject::disconnect(engine.data(), &GameEngine::madeMove, this, &TrainingEngine::madeMove);
    QObject::disconnect(engine.data(), &GameEngine::hasWon, this, &TrainingEngine::gameWon);
    QObject::disconnect(engine.data(), &GameEngine::hasTied, this, &TrainingEngine::hasTied);

    engine.reset();

    if(j < 5)
    {
        j++;
    }
    if(j >= 5)
    {
        if(i == 29)
        {
            hasFinishedIteration();
            return;
        }
        i++;
        j = 0;
        pool.clear();
        pool.fill(1, 30);
    }
    startNewGame();
}

void TrainingEngine::hasFinishedIteration()
{
    QMap<int, int> nnScores;

    for(auto &game : trainingData->games)
    {
        nnScores[game->nn1->id] += (game->score == -1 ? -2 : game->score);
        nnScores[game->nn2->id] += (-game->score == -1 ? -2 : -game->score);
    }
    QVector<QPair<quint64, int>> ord;
    for(auto &ns : nnScores.keys())
    {
        ord.append(QPair<int, int>(ns, nnScores[ns]));
    }

    std::sort(ord.begin(), ord.end(), [](const QPair<int, int> &p1, const QPair<int, int> &p2) {
        return p1.second > p2.second;
    });

    for(int i = 15; i < 30; i++)
    {
        quint64 id = ord.at(i).first;
        bool flag = true;
        for(int j = 0; j < 15; j++)
        {
            if(originalNNs->at(j)->id == id)
            {
                flag = false;
                trainingData->originalsToDeactivate[j] = 1;
                break;
            }
        }
        if(flag)
        {
            for(auto &nn : trainingData->newNNs)
            {
                if(nn->id == id)
                {
                    nn->active = false;
                }
            }
        }
    }

    for(auto &n : *originalNNs)
    {
        if(n->id == ord.at(0).first)
        {
            trainingData->bestPerforming = n;
        }
    }
    if(trainingData->bestPerforming.isNull())
    {
        for(auto &n : trainingData->newNNs)
        {
            if(n->id == ord.at(0).first)
            {
                trainingData->bestPerforming = n;
            }
        }
    }
    trainingData->trainingTime = timer.elapsed();

    emit beginDBSave();
    QApplication::processEvents();
    manager->addTrainingIteration(trainingData);

    if(iteration < totalIterations)
    {
        newIteration();
    }
    else
    {
        status = Status::Stopped;
        emit hasFinished(iteration);
    }
}
