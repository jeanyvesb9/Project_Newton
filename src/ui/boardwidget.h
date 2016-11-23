#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include <QMouseEvent>
#include <QPainter>
#include <QIcon>
#include <QGridLayout>

#include "src/game/board.h"

class BoardPieceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardPieceWidget(int id, QWidget *parent = 0);

    enum State { Hidden, Player, King, OpPlayer, OpKing, Path };

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(int id);

public slots:
    void setState(State state);

private:
    int id;
    State state;
};

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void selectedMove(Game::MovePointer move);

public slots:
    void setBoard(Game::BoardData board);
    void disableBoard();
    void displayAvailableMoves(Game::BoardPointer board, QVector<Game::MovePointer> moves);

private slots:
    void cellClicked(int id);

private:
    Game::BoardData board;
    bool boardValid;
    Game::BoardPointer boardPtr;

    QMap<Game::Cell, QVector<Game::MovePointer>> moves;
    Game::CellPointer selectedCell;
    QMap<Game::MovePointer, QVector<Game::Cell>> selectedCellMoves;

    QVector<BoardPieceWidget*> pieces;
};

#endif // BOARDWIDGET_H
