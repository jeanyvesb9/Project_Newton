QT += qml quick sql

CONFIG += c++14

SOURCES += \
    src/main.cpp \
    src/game/checkersgame.cpp \
    src/neuralNetwork/neuralnetwork.cpp \
    src/engine/abstractengine.cpp \
    src/engine/neuralnetworkengine.cpp \
    src/engine/manualplayerengine.cpp \
    src/game/board.cpp \
    src/neuralNetwork/neuron.cpp \
    src/neuralNetwork/neuralnetworkmanager.cpp \
    src/utilityfunctions.cpp \
    src/training/trainingengine.cpp

RESOURCES += \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/game/checkersgame.h \
    src/neuralNetwork/neuralnetwork.h \
    src/engine/abstractengine.h \
    src/engine/neuralnetworkengine.h \
    src/engine/manualplayerengine.h \
    src/game/board.h \
    src/neuralNetwork/neuron.h \
    src/neuralNetwork/neuralnetworkmanager.h \
    src/utilityfunctions.h \
    src/training/trainingengine.h \
    src/training/trainingstructs.h \
    src/game/gamestructs.h
