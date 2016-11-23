#include "nntraining.h"
#include "ui_nntraining.h"

NNTraining::NNTraining(NN::NeuralNetworkManagerPointer ptr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NNTraining), nnm{ptr}, te{NN::TrainingEnginePointer(new NN::TrainingEngine(nnm, 4))}
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Trainer"));
    this->setModal(true);

    brdWidget = new BoardWidget();
    brd_rw = new AspectRatioWidget(brdWidget, ui->boardBox->layout()->widget());
    brdWidget->setParent(brd_rw);
    ui->boardBox->layout()->addWidget(brd_rw);

    QObject::connect(te.data(), &NN::TrainingEngine::statusUpdate, this, &NNTraining::statusUpdate);
    QObject::connect(te.data(), &NN::TrainingEngine::beginDBSave, this, &NNTraining::beginDbSave);
    QObject::connect(te.data(), &NN::TrainingEngine::hasFinished, this, &NNTraining::hasFinished);
    QObject::connect(te.data(), &NN::TrainingEngine::hasCanceled, this, &NNTraining::hasCanceled);

    ui->iterations->setMinimum(1);
    ui->tie->setMinimum(2);
    ui->tie->setMaximum(10000);
    ui->tie->setValue(100);

    setUiStop();
}
NNTraining::~NNTraining()
{
    delete ui;

    QObject::disconnect(te.data(), &NN::TrainingEngine::statusUpdate, this, &NNTraining::statusUpdate);
    QObject::disconnect(te.data(), &NN::TrainingEngine::beginDBSave, this, &NNTraining::beginDbSave);
    QObject::disconnect(te.data(), &NN::TrainingEngine::hasFinished, this, &NNTraining::hasFinished);
    QObject::disconnect(te.data(), &NN::TrainingEngine::hasCanceled, this, &NNTraining::hasCanceled);
}

void NNTraining::on_startBtn_clicked()
{
    ui->iterations->setEnabled(false);
    ui->tie->setEnabled(false);
    ui->startBtn->setEnabled(false);
    ui->stopBtn->setEnabled(true);
    ui->closeBtn->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    te->setTieValue(ui->tie->value());
    te->startTraining(ui->iterations->value(), ui->tie->value());
}

void NNTraining::on_stopBtn_clicked()
{
    te->cancelTraining();
}

void NNTraining::on_closeBtn_clicked()
{
    accept();
}

void NNTraining::statusUpdate(NN::Status statusUpdate, Game::BoardData board, int iteration, int totalIterations, int moves, int maxMoves, int games)
{
    switch(statusUpdate)
    {
    case NN::Status::Running:
        ui->statusLbl->setText(tr("Running"));
        break;
    case NN::Status::StopRequested:
        ui->statusLbl->setText(tr("Stop Requested..."));
        break;
    case NN::Status::Stopped:
        ui->statusLbl->setText(tr("Stopped"));
        break;
    }

    brdWidget->setBoard(board);
    ui->iterationLbl->setText(QString::number(iteration) + "/" + QString::number(totalIterations));
    ui->movesLbl->setText(QString::number(moves) + "/" + QString::number(maxMoves));
    ui->gameLbl->setText(QString::number(games) + "/150");
    ui->progressBar->setValue(((double)(maxMoves * 150 * (iteration - 1) + maxMoves * (games - 1) + moves) / (double)(maxMoves * 150 * totalIterations)) * 100);
}

void NNTraining::beginDbSave()
{
    ui->statusLbl->setText(tr("Saving to file..."));
}

void NNTraining::hasFinished(int iterations)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Training finished after ") + QString::number(iterations) + tr(" iterations."));
    msgBox.exec();
    setUiStop();
}

void NNTraining::hasCanceled(int iterations)
{
    QMessageBox msgBox;
    msgBox.setText(tr("Training canceled after ") + QString::number(iterations) + tr(" iterations."));
    msgBox.exec();
    setUiStop();
}

void NNTraining::setUiStop()
{
    ui->iterations->setEnabled(true);
    ui->tie->setEnabled(true);

    brdWidget->disableBoard();
    ui->statusLbl->setText(tr("Stopped"));
    ui->iterationLbl->setText("-");
    ui->movesLbl->setText("-");
    ui->gameLbl->setText("-");
    ui->progressBar->setVisible(false);

    ui->startBtn->setEnabled(true);
    ui->stopBtn->setEnabled(false);
    ui->closeBtn->setEnabled(true);
}
