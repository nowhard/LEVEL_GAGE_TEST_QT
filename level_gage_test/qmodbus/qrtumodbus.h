/***********************************************************************************************************************
* QiRtuModbus : Local (serial port) support for RTU Modbus devices.                                                    *
************************************************************************************************************************
* Author : Michael Clausen (michael.clausen@hevs.ch)                                                                   *
* Date   : 2009/07/14                                                                                                  *
* Changes: -                                                                                                           *
***********************************************************************************************************************/
#pragma once


/*** Base class *******************************************************************************************************/
#include "qmodbus/qabstractmodbus.h"


/*** Qt includes & prototypes *****************************************************************************************/
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include <QtSerialPort>


/*** System includes **************************************************************************************************/

#ifdef Q_OS_UNIX /*****************************************************************************************************/

#   include <termios.h>

#endif /* Q_OS_UNIX ***************************************************************************************************/

#ifdef Q_OS_WIN /******************************************************************************************************/

#   include <windows.h>
#   include <io.h>

#endif /* Q_OS_WIN ****************************************************************************************************/


/*** QiRtuModbus class declaration and help ***************************************************************************/
/*!
* The QiRtuModbus class talks to local attached (serial port) modbus slave devices using the modbus protocol in RTU
* mode.
* \headerfile qirtumodbus.h QiRtuModbus
*/
class QRtuModbus : public QAbstractModbus
{
public:

    unsigned int _timeout;                  // Timeout to use in serial communication.

public:
    /*!
    * Constructor.
    */
    QRtuModbus();

    /*!
    * Destructor.
    */
    virtual ~QRtuModbus();

    bool open( const QString &device , const qint32 baudRate ,QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits , const QSerialPort::Parity parity ,
               const QSerialPort::FlowControl flowControl );

    /*!
    * Returns the state of the serial port handle.
    * \return True if the port is open and ready to use, false otherwise.
    */
    bool isOpen() const;

    /*!
    * Closes the connection to the slave modbus device.
    */
    void close();

    // Interface implementation (QiAbstractModbus).
    unsigned int timeout( void ) const;

    // Interface implementation (QiAbstractModbus).
    void setTimeout( const unsigned int timeout );

    // Interface implementation (QiAbstractModbus).
    QList<bool> readCoils( const quint8 deviceAddress ,
                           const quint16 startingAddress ,
                           const quint16 quantityOfCoils ,
                           quint8 *const status = NULL
                         ) const;

    // Interface implementation (QiAbstractModbus).
    QList<bool> readDiscreteInputs( const quint8 deviceAddress ,
                                    const quint16 startingAddress ,
                                    const quint16 quantityOfInputs ,
                                    quint8 *const status = NULL
                                  ) const;

    // Interface implementation (QiAbstractModbus).
    QList<quint16> readHoldingRegisters( const quint8 deviceAddress ,
                                         const quint16 startingAddress ,
                                         const quint16 quantityOfRegisters ,
                                         quint8 *const status = NULL
                                       ) const;

    // Interface implementation (QiAbstractModbus).
    QList<quint16> readInputRegisters( const quint8 deviceAddress ,
                                       const quint16 startingAddress ,
                                       const quint16 quantityOfInputRegisters ,
                                       quint8 *const status = NULL
                                     ) const;

    // Interface implementation (QiAbstractModbus).
    bool writeSingleCoil( const quint8 deviceAddress ,
                          const quint16 outputAddress ,
                          const bool outputValue ,
                          quint8 *const status = NULL
                        ) const;

    // Interface implementation (QiAbstractModbus).
    bool writeSingleRegister( const quint8 deviceAddress ,
                              const quint16 registerAddress ,
                              const quint16 registerValue ,
                              quint8 *const status = NULL
                            ) const;

    // Interface implementation (QiAbstractModbus).
    bool writeMultipleCoils( const quint8 deviceAddress ,
                             const quint16 startingAddress ,
                             const QList<bool> & outputValues ,
                             quint8 *const status = NULL
                           ) const;

    // Interface implementation (QiAbstractModbus).
    bool writeMultipleRegisters( const quint8 deviceAddress ,
                                 const quint16 startingAddress ,
                                 const QList<quint16> & registersValues ,
                                 quint8 *const status = NULL
                               ) const;

    // Interface implementation (QiAbstractModbus).
    bool maskWriteRegister( const quint8 deviceAddress ,
                            const quint16 referenceAddress ,
                            const quint16 andMask ,
                            const quint16 orMask ,
                            quint8 *const status = NULL
                          ) const;

    // Interface implementation (QiAbstractModbus).
    QList<quint16> writeReadMultipleRegisters( const quint8 deviceAddress ,
                                               const quint16 writeStartingAddress ,
                                               const QList<quint16> & writeValues ,
                                               const quint16 readStartingAddress ,
                                               const quint16 quantityToRead ,
                                               quint8 *const status = NULL
                                             ) const;


    // Interface implementation (QiAbstractModbus).
    QList<quint16> readFifoQueue( const quint8 deviceAddress ,
                                  const quint16 fifoPointerAddress ,
                                  quint8 *const status = NULL
                                ) const;

    // Interface implementation (QiAbstractModbus).
    QByteArray executeCustomFunction( const quint8 deviceAddress , const quint8 modbusFunction ,
                                      QByteArray &data , quint8 *const status = NULL ) const;

    // Interface implementation (QiAbstractModbus).
    QByteArray executeRaw( QByteArray &data , quint8 *const status = NULL ) const;

    // Interface implementation (QiAbstractModbus).
    QByteArray calculateCheckSum( QByteArray &data ) const;

public slots:

    void processRtuModbus();

private:

    QByteArray _read( const int numberBytes ) const;
    QByteArray _readAll( void ) const;
    QByteArray _readLine( int maxBytes ) const;
    bool _write( QByteArray &data ) const;

    // Used to perform CRC on outgoing modbus messages.
    quint16 _calculateCrc( const QByteArray &pdu ) const;

    // Used to check CRC on incomming modbus messages.
    bool _checkCrc( const QByteArray &pdu ) const;

    QSerialPort *SerialPort;
};
