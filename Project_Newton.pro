QT += sql widgets gui core serialport

CONFIG += c++14

SOURCES += \
    src/main.cpp \
    src/neuralNetwork/neuralnetwork.cpp \
    src/game/board.cpp \
    src/neuralNetwork/neuron.cpp \
    src/neuralNetwork/neuralnetworkmanager.cpp \
    src/utilityfunctions.cpp \
    src/training/trainingengine.cpp \
    src/player/abstractplayer.cpp \
    src/engine/gameengine.cpp \
    src/player/manualplayer.cpp \
    src/camera/cameraanalyser.cpp \
    src/player/neuralnetworkplayer.cpp \
    src/arduino/arduinoserial.cpp \
    src/ui/nntraining.cpp \
    src/ui/initialwindow.cpp \
    src/ui/boardwidget.cpp \
    src/ui/aspectratiowidget.cpp \
    src/ui/playwindow.cpp \
    src/game/gamefile.cpp

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
    src/player/abstractplayer.h \
    src/engine/gameengine.h \
    src/player/manualplayer.h \
    src/camera/cameraanalyser.h \
    src/player/neuralnetworkplayer.h \
    src/arduino/arduinoserial.h \
    src/ui/nntraining.h \
    src/ui/initialwindow.h \
    src/ui/boardwidget.h \
    src/ui/aspectratiowidget.h \
    src/ui/playwindow.h \
    src/game/gamefile.h

FORMS += \
    src/ui/nntraining.ui \
    src/ui/initialwindow.ui \
    src/ui/playwindow.ui
