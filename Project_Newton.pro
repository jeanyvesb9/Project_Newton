QT += sql widgets gui core #qml quick

CONFIG += c++14

SOURCES += \
    src/main.cpp \
    src/neuralNetwork/neuralnetwork.cpp \
    src/game/board.cpp \
    src/neuralNetwork/neuron.cpp \
    src/neuralNetwork/neuralnetworkmanager.cpp \
    src/utilityfunctions.cpp \
    src/training/trainingengine.cpp \
    src/ui/mainwindow.cpp \
    src/player/abstractplayer.cpp \
    src/engine/gameengine.cpp \
    src/player/manualplayer.cpp \
    src/playing/playengine.cpp \
    src/camera/cameraanalyser.cpp \
    src/player/neuralnetworkplayer.cpp

RESOURCES += \
    resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/neuralNetwork/neuralnetwork.h \
    src/game/board.h \
    src/neuralNetwork/neuron.h \
    src/neuralNetwork/neuralnetworkmanager.h \
    src/utilityfunctions.h \
    src/training/trainingengine.h \
    src/training/trainingstructs.h \
    src/game/gamestructs.h \
    src/ui/mainwindow.h \
    src/player/abstractplayer.h \
    src/engine/gameengine.h \
    src/player/manualplayer.h \
    src/playing/playengine.h \
    src/camera/cameraanalyser.h \
    src/player/neuralnetworkplayer.h

FORMS += \
    src/ui/mainwindow.ui
