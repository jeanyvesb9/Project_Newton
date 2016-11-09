#ifndef MANUALPLAYER_H
#define MANUALPLAYER_H

#include <QObject>

#include "src/player/abstractplayer.h"

class ManualPlayer : public AbstractPlayer
{
    Q_OBJECT
public:
    explicit ManualPlayer(Game::Side side, QObject *parent = 0);

signals:

public slots:
    void makeMove()

protected:
    void executeTurn() override;
    void backgroundTask() override;
};
using ManualPlayerpointer = QSharedPointer<ManualPlayer>;

#endif // MANUALPLAYER_H
