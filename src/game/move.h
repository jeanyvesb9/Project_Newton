#ifndef MOVE_H
#define MOVE_H

#include <QObject>

class Move : public QObject
{
    Q_OBJECT
public:
    explicit Move(QObject *parent = 0);

signals:

public slots:
};

#endif // MOVE_H