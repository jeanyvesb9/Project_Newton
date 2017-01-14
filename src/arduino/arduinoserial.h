#ifndef ARDUINOSERIAL_H
#define ARDUINOSERIAL_H

#include <QObject>
#include <QSharedPointer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QList>
#include <QtGlobal>
#include <QtEndian>
#include <QDebug>

#include "src/game/gamestructs.h"
#include "src/game/board.h"

class ArduinoSerialOpenException {};

class ArduinoSerial : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoSerial(QSerialPortInfo &port, QObject *parent = 0);
    ~ArduinoSerial();

    static QList<QSerialPortInfo> getSerialPorts();

public slots:
    void sendMove(Game::BoardPointer board, Game::MovePointer move);
    void sendCellGroup(QVector<Game::Cell> cells);
    void setBlack();

private slots:
    void buttonPressRequest();

signals:
    void gotBtn1Press();
    void gotBtn1LongPress();
    void gotBtn2Press();
    void gotBtn2LongPress();

private:
    QSerialPort *serial;
};

using ArduinoSerialPointer = QSharedPointer<ArduinoSerial>;

 #endif // ARDUINOSERIAL_H
