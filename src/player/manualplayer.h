#ifndef MANUALPLAYER_H
#define MANUALPLAYER_H

#include <QObject>
#include <QVector>

#include "src/player/abstractplayer.h"
#include "src/game/board.h"
#include "src/camera/cameraanalyser.h"

class ManualPlayer : public AbstractPlayer
{
public:
    explicit ManualPlayer(Game::Side side, QObject *parent = 0);

signals:
    void wrongMove();

public slots:
    void validateTurn(QVector<BoardPiece> rawBoardData);

protected:
    void executeTurn() override;
    void backgroundTask() override;

private:
    QVector<Game::MovePointer> possibleMoves;
};
using ManualPlayerpointer = QSharedPointer<ManualPlayer>;

#endif // MANUALPLAYER_H
