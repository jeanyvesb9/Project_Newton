#ifndef CAMERAANALYSER_H
#define CAMERAANALYSER_H

#include <QObject>
#include <QVector>

#include "src/game/board.h"

enum class BoardPiece { PlayerPiece, OpPlayerPiece, Empty };
QVector<BoardPiece> convertBoardToBoardPieceVector(Game::BoardPointer board);

class CameraAnalyser : public QObject
{
    Q_OBJECT
public:
    explicit CameraAnalyser(QObject *parent = 0);

signals:

public slots:
};

#endif // CAMERAANALYSER_H
