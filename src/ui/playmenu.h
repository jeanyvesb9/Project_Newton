#ifndef PLAYMENU_H
#define PLAYMENU_H

#include <QDialog>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>

class PlayMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PlayMenu(bool cameraConnected, bool boardConnected, QFont adobeCleanLight, QFont segoeUILight, QWidget *parent = 0);
    ~PlayMenu();

    enum State { NormalMenuState, NewGameState, CameraCalibrationState, HelpState, QuitState };
signals:
    void newGame(QString file, QString name, int difficulty);
    void resumeGame();
    void exitGame();

public slots:
    void nextPressed();
    void okPressed();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    State state;
    int index;
    int maxIndex;
    int normalMenuIndex;

    bool cameraConnected;
    bool boardConnected;

    QFont adobeCleanLight;
    QFont segoeUILight;

    int drawTitle(QString title, QPainter &painter);
    int drawOptions(QStringList options, int index, int startingHeight, QPainter &painter);
    int drawText(QString text, int startingHeight, QPainter &painter, int size = 13);

    void backToNormalMenu();
};

#endif // PLAYMENU_H
