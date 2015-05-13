#include "serial.h"
#include <qdebug.h>

Serial::Serial(QObject *parent) :
    QObject(parent)
{
    SerialPort = new QSerialPort(this);
}

Serial::~Serial()
{
    delete(SerialPort);
    emit finishedSerial();
}

void Serial::processSerial()
{
    connect(SerialPort,SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(SerialPort, SIGNAL(readyRead()),this,SLOT(ReadSerial()));
}

void Serial::writeSettingsSerial(QString name, int baudrate,int DataBits,
                         int Parity,int StopBits, int FlowControl)
{
    SettingsSerial.name = name;
    SettingsSerial.baudRate = (QSerialPort::BaudRate) baudrate;
    SettingsSerial.dataBits = (QSerialPort::DataBits) DataBits;
    SettingsSerial.parity = (QSerialPort::Parity) Parity;
    SettingsSerial.stopBits = (QSerialPort::StopBits) StopBits;
    SettingsSerial.flowControl = (QSerialPort::FlowControl) FlowControl;
}

void Serial::connectSerial(void)
{

}

void Serial::handleError(QSerialPort::SerialPortError error)
{
    if ( (SerialPort->isOpen()) && (error == QSerialPort::ResourceError))
    {
        error_(SerialPort->errorString().toLocal8Bit());
        disconnectSerial();
    }
}


void  Serial::disconnectSerial()
{
    if(SerialPort->isOpen())
    {
        SerialPort->close();
        error_(SettingsSerial.name.toLocal8Bit() + " >> Закрыт!\r");
        qDebug("CLOSE");
    }
}


void Serial::writeToSerial(QByteArray data)
{
    if(SerialPort->isOpen())
    {
        SerialPort->write(data);
    }
}

void Serial::readSerial()
{
    QByteArray data;
    data.append(SerialPort->readAll());
    outSerial(data);
}

