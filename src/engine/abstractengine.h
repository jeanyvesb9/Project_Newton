#ifndef ABSTRACTENGINE_H
#define ABSTRACTENGINE_H

#include <QObject>

class AbstractEngine : public QObject
{
    Q_OBJECT
public:
    explicit AbstractEngine(QObject *parent = 0);

signals:

public slots:
};

#endif // ABSTRACTENGINE_H