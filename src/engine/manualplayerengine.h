#ifndef MANUALPLAYERENGINE_H
#define MANUALPLAYERENGINE_H

#include <QObject>
#include "src/engine/abstractengine.h"

class ManualPlayerEngine : public AbstractEngine
{
    Q_OBJECT
public:
    ManualPlayerEngine();
};

#endif // MANUALPLAYERENGINE_H
