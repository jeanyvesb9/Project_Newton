#include "newgameconfig.h"
#include "ui_newgameconfig.h"

NewGameConfig::NewGameConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameConfig)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setWindowTitle(tr("New Game Settings"));
    //this->setWindowFlags(Qt::FramelessWindowHint);

    ui->difficultyEdit->setValue(3);
    ui->difficultyEdit->setRange(1, 20);

    ui->buttonBox->setEnabled(false);
}

NewGameConfig::~NewGameConfig()
{
    delete ui;
}

QString NewGameConfig::getName() const
{
    return ui->nameEdit->text();
}

unsigned int NewGameConfig::getDifficulty() const
{
    return ui->difficultyEdit->value();
}

void NewGameConfig::on_nameEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->buttonBox->setEnabled(false);
    else
        ui->buttonBox->setEnabled(true);
}
