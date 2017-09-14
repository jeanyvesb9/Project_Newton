#ifndef MANUALPLAYER_H
#define MANUALPLAYER_H

#include <QObject>
#include <QVector>

#include "src/player/abstractplayer.h"
#include "src/game/board.h"
#include "src/camera/cameraanalyzer.h"

class ManualPlayer : public AbstractPlayer
{
    Q_OBJECT
public:
    explicit ManualPlayer(Game::Side side, QObject *parent = 0);

public slots:
    Q_INVOKABLE void validateTurn(QVector<BoardPiece> rawBoardData);
    Q_INVOKABLE void madeMove(Game::MovePointer move);

signals:
    void wrongMove();

protected:
    void executeTurn() override;
    void backgroundTask() override;

private:
    QVector<Game::MovePointer> possibleMoves;
    Game::MovePointer move;
    bool hasFinished;
    bool wasWrongMove;
};
using ManualPlayerPointer = QSharedPointer<ManualPlayer>;

Q_DECLARE_METATYPE(QVector<BoardPiece>)

#endif // MANUALPLAYER_H
