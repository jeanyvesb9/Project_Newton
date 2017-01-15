#ifndef PLAYMENU_H
#define PLAYMENU_H

#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>

class PlayMenu : public QWidget
{
    Q_OBJECT
public:
    explicit PlayMenu(bool cameraConnected, QFont adobeCleanLight, QFont segoeUILight, QWidget *parent = 0);
    ~PlayMenu();

    enum State { NormalMenuState, NewGameState, CameraCalibrationState, HelpState, QuitState, NewGameLoadState };
signals:
    void newGame();
    void resumeGame();
    void quitGame();

public slots:
    void nextPressed();
    void okPressed();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    State state;
    int index;
    int maxIndex;
    int normalMenuIndex;
    QVector<QRectF> individualOptionsBoundRect;
    QRectF optionsBoundRect;

    bool cameraConnected;
    bool boardConnected;

    QFont adobeCleanLight;
    QFont segoeUILight;

    int drawTitle(QString title, QPainter &painter);
    int drawOptions(QStringList options, int index, int startingHeight, QPainter &painter);
    int drawText(QString text, int startingHeight, QPainter &painter, int size = 13, int flags = 0, int fixedHeight = 10);

    void backToNormalMenu();
    void updateIndexFromCursor(QPointF p);
};

#endif // PLAYMENU_H
