#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

struct Settings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

class Serial : public QObject
{
    Q_OBJECT

public:

    explicit Serial(QObject *parent = 0);

    ~Serial();
    QSerialPort *SerialPort;

    Settings SettingsSerial;

signals:

    void finishedSerial(); //

    void error_(QString err);

    void outSerial(QString data);

public slots:

    void disconnectSerial(void);

    void connectSerial(void);

    void writeSettingsSerial(QString name, int baudrate, int DataBits, int Parity, int StopBits, int FlowControl);

    void processSerial();

    void writeToSerial(QByteArray data);

    void readSerial(void);

private slots:

    void handleError(QSerialPort::SerialPortError error);//

public:

};

#endif // PORT_H
