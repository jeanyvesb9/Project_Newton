#ifndef CHECKERSGAME_H
#define CHECKERSGAME_H

#include <QObject>

class CheckersGame : public QObject
{
    Q_OBJECT
public:
    explicit CheckersGame(QObject *parent = 0);

signals:

public slots:
};

#endif // CHECKERSGAME_H