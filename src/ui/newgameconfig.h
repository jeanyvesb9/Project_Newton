#ifndef NEWGAMECONFIG_H
#define NEWGAMECONFIG_H

#include <QDialog>

namespace Ui {
class NewGameConfig;
}

class NewGameConfig : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameConfig(QWidget *parent = 0);
    ~NewGameConfig();

    QString getName() const;
    unsigned int getDifficulty() const;

private slots:
    void on_nameEdit_textChanged(const QString &arg1);

private:
    Ui::NewGameConfig *ui;
};

#endif // NEWGAMECONFIG_H
