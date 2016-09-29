#ifndef CHECKERSGAME_H
#define CHECKERSGAME_H

#include <QObject>
#include <QString>

class CheckersGame : public QObject
{
    Q_OBJECT
public:
    CheckersGame(QObject *parent = 0);

signals:

public slots:
};

#endif // CHECKERSGAME_H
