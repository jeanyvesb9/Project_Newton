#ifndef PLAYENGINE_H
#define PLAYENGINE_H

#include <QObject>

#include "src/game/board.h"

struct LedCellData
{
    QVector<Game::Cell> path;
    QVector<Game::Cell> removed;
    Game::CellPointer crowned;
};
using LedCellDataPointer = QSharedPointer<LedCellData>;

class PlayEngine : public QObject
{
    Q_OBJECT
public:
    explicit PlayEngine(QObject *parent = 0);

signals:

public slots:
};

#endif // PLAYENGINE_H
