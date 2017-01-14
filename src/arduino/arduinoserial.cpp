#include "arduinoserial.h"

ArduinoSerial::ArduinoSerial(QSerialPortInfo &port, QObject *parent) : QObject(parent),
    serial{new QSerialPort(port)}
{
    if(!serial->open(QIODevice::ReadWrite))
    {
        throw ArduinoSerialOpenException();
    }
    serial->setBaudRate(QSerialPort::Baud57600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    QObject::connect(serial, &QSerialPort::readyRead, this, &ArduinoSerial::buttonPressRequest);
}

ArduinoSerial::~ArduinoSerial()
{
    serial->flush();
    serial->close();
    serial->deleteLater();
}

QList<QSerialPortInfo> ArduinoSerial::getSerialPorts()
{
    return QSerialPortInfo::availablePorts();
}

void ArduinoSerial::sendMove(Game::BoardPointer board, Game::MovePointer move)
{
    QByteArray data;
    QVector<Game::Cell> cells = board->getMovePathCells(move);

    data.append(0xE2);
    for(auto &cell : cells)
    {
        data.append(cell.toCellNum());
    }
    data.append(0xFF);

    serial->write(data);
}

void ArduinoSerial::sendCellGroup(QVector<Game::Cell> cells)
{
    QByteArray data;

    data.append(0xE1);
    for(auto &cell : cells)
    {
        data.append(cell.toCellNum());
    }
    data.append(0xFF);

    serial->write(data);
}

void ArduinoSerial::setBlack()
{
    QByteArray data;

    data.append(0xE0);
    data.append(0xFF);

    serial->write(data);
}

void ArduinoSerial::buttonPressRequest()
{
    char data = serial->read(1).at(0);
    if(data == 'A')
    {
        emit gotBtn1Press();
    }
    else if(data == 'B')
    {
        emit gotBtn2Press();
    }
    else if(data == 'C')
    {
        emit gotBtn1LongPress();
    }
    else if(data == 'D')
    {
        emit gotBtn2LongPress();
    }
}
