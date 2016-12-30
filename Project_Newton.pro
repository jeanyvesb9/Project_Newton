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
    src/game/gamefile.cpp \
    src/ui/topologyselector.cpp \
    src/ui/spinboxdelegate.cpp \
    src/ui/newgameconfig.cpp \
    src/ui/playwindow.cpp \
    src/ui/playinfotable.cpp \
    src/ui/playmenu.cpp

RESOURCES += \
    resources.qrc

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
    src/game/gamefile.h \
    src/ui/topologyselector.h \
    src/ui/spinboxdelegate.h \
    src/ui/newgameconfig.h \
    src/ui/macwindow.h \
    src/ui/playwindow.h \
    src/ui/playinfotable.h \
    src/ui/playmenu.h

FORMS += \
    src/ui/nntraining.ui \
    src/ui/initialwindow.ui \
    src/ui/topologyselector.ui \
    src/ui/newgameconfig.ui \
    src/ui/playwindow.ui


macx: {
    DEFINES += MACX

    LIBS += -framework Foundation -framework Cocoa
    INCLUDEPATH += /System/Library/Frameworks/Foundation.framework/Versions/C/Headers \
                   /System/Library/Frameworks/AppKit.framework/Headers \
                   /System/Library/Frameworks/Cocoa.framework/Headers

    OBJECTIVE_SOURCES += \
        src/ui/macwindow.mm

}

DISTFILES += \
    assets/text/help.txt
