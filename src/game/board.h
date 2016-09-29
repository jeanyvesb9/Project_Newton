#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QList>

#include "src/checkers/piecestype.h"

namespace Game {

class Board : public QObject
{
    Q_OBJECT
public:
    Board(QObject *parent = 0);

    //static defaultBoard

private:
    QList<Checkers::Places> boardDiagram;

signals:

public slots:
};

}

#endif // BOARD_H
