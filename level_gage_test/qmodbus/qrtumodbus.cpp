/***********************************************************************************************************************
* QRtuModbus implementation.                                                                                          *
***********************************************************************************************************************/
#include "qmodbus/qrtumodbus.h"


/*** Qt includes ******************************************************************************************************/
#include <QtCore/QDataStream>
#include <QtEndian>


/*** System includes **************************************************************************************************/
#include <unistd.h>


/*** Abstract interface class destructor implementation ***************************************************************/
QAbstractModbus::~QAbstractModbus()
{}


/*** Class implementation *********************************************************************************************/
QRtuModbus::QRtuModbus() : _timeout( 500 )
{
    SerialPort=new QSerialPort(this);
}

QRtuModbus::~QRtuModbus()
{
    close();
    delete SerialPort;
}

bool QRtuModbus::open( const QString &device , const qint32 baudRate ,QSerialPort::DataBits dataBits, const QSerialPort::StopBits stopBits , const QSerialPort::Parity parity ,
                       const QSerialPort::FlowControl flowControl )
{
    SerialPort->setPortName(device);
    SerialPort->setBaudRate(baudRate);
    SerialPort->setDataBits(dataBits);
    SerialPort->setParity(parity);
    SerialPort->setStopBits(stopBits);
    SerialPort->setFlowControl(flowControl);

    if (SerialPort->open(QIODevice::ReadWrite))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool QRtuModbus::isOpen() const
{
    return SerialPort->isOpen();
}

void QRtuModbus::close()
{
    SerialPort->close();
}

unsigned int QRtuModbus::timeout( void ) const
{
    return _timeout;
}

void QRtuModbus::setTimeout( const unsigned int timeout )
{
    _timeout = timeout;

    if ( isOpen() )
    {



    }
}

QList<bool> QRtuModbus::readCoils( const quint8 deviceAddress , const quint16 startingAddress ,
                                    const quint16 quantityOfCoils , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QList<bool>();
    }

    // Create modbus read coil status pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x01 << startingAddress << quantityOfCoils;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfCoils / 8;
    if ( quantityOfCoils % 8 ) neededRxBytes++;
    pdu.clear();

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QList<bool>();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QList<bool>();
    }

    // Receive the rest.
    pdu += _read( neededRxBytes );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QList<bool>();
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 5 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x01 && byteCount == neededRxBytes )
        {
            // It is ok, read and convert the data.
            QList<bool> list;
            quint8 tmp;
            for ( int i = 0 ; i < quantityOfCoils ; i++ )
            {
                if ( i % 8 == 0 ) rxStream >> tmp;
                list.append( tmp & ( 0x01 << ( i % 8 ) ) );
            }
            if ( status ) *status = Ok;
            return list;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return QList<bool>();
}

QList<bool> QRtuModbus::readDiscreteInputs( const quint8 deviceAddress , const quint16 startingAddress ,
                                             const quint16 quantityOfInputs , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QList<bool>();
    }

    // Create modbus read coil status pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x02 << startingAddress << quantityOfInputs;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfInputs / 8;
    if ( quantityOfInputs % 8 ) neededRxBytes++;
    pdu.clear();

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QList<bool>();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QList<bool>();
    }

    // Receive the rest of the message.
    pdu += _read( neededRxBytes );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QList<bool>();
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 5 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x02 && byteCount == neededRxBytes )
        {
            // It is ok, read and convert the data.
            QList<bool> list;
            quint8 tmp;
            for ( int i = 0 ; i < quantityOfInputs ; i++ )
            {
                if ( i % 8 == 0 ) rxStream >> tmp;
                list.append( tmp & ( 0x01 << ( i % 8 ) ) );
            }
            if ( status ) *status = Ok;
            return list;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return QList<bool>();
}

QList<quint16> QRtuModbus::readHoldingRegisters( const quint8 deviceAddress , const quint16 startingAddress ,
                                                  const quint16 quantityOfRegisters , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create modbus read holding registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x03 << startingAddress << quantityOfRegisters;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfRegisters * 2;
    pdu.clear();

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QList<quint16>();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QList<quint16>();
    }

    // Receive the rest of the message.
    pdu += _read( neededRxBytes );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QList<quint16>();
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 5 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x03 && byteCount == neededRxBytes )
        {
            // It is ok, read and convert the data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < quantityOfRegisters ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return QList<quint16>();
}

QList<quint16> QRtuModbus::readInputRegisters( const quint8 deviceAddress , const quint16 startingAddress ,
                                                const quint16 quantityOfInputRegisters , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create modbus read input status pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x04 << startingAddress << quantityOfInputRegisters;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityOfInputRegisters * 2;
    pdu.clear();

    SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QList<quint16>();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QList<quint16>();
    }

    // Receive the rest of the message.
    pdu += _read( neededRxBytes );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QList<quint16>();
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 5 )
    {
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x04 && byteCount == neededRxBytes )
        {
            // It is ok, read and convert the data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < quantityOfInputRegisters ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return QList<quint16>();
}

bool QRtuModbus::writeSingleCoil( const quint8 deviceAddress , const quint16 outputAddress ,
                                   const bool outputValue , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create modbus write single coil pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x05 << outputAddress << ( outputValue ? (quint16)0xFF00 : (quint16)0x0000 );

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    pdu.clear();
    SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return false;
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return false;
    }

    // Read the rest of the message.
    pdu += _read( 3 );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return false;
    }

    // Check data and return them on success.
    if ( pdu.size() == 8 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        quint16 rxOutputAddress , rxOutputValue;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> rxOutputAddress >> rxOutputValue;
        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x05 && rxOutputAddress == outputAddress &&
             ( ( outputValue && rxOutputValue == 0xFF00 ) || ( !outputValue && rxOutputValue == 0x0000 ) ) )
        {
            if ( status ) *status = Ok;
            return true;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return false;
}

bool QRtuModbus::writeSingleRegister( const quint8 deviceAddress , const quint16 outputAddress ,
                                       const quint16 registerValue , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create modbus write single coil pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x06 << outputAddress << registerValue;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    pdu.clear();

     SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return false;
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return false;
    }

    // Read the rest of the message.
    pdu += _read( 3 );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return false;
    }

    // Check data and return them on success.
    if ( pdu.size() == 8 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        quint16 rxOutputAddress , rxRegisterValue;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> rxOutputAddress >> rxRegisterValue;
        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x06 && rxOutputAddress == outputAddress &&
             rxRegisterValue == registerValue )
        {
            if ( status ) *status = Ok;
            return true;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return false;
}

bool QRtuModbus::writeMultipleCoils( const quint8 deviceAddress , const quint16 startingAddress ,
                                      const QList<bool> & outputValues , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create modbus write multiple coil pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    quint8 txBytes = outputValues.count() / 8;
    if ( outputValues.count() % 8 != 0 ) txBytes++;
    pduStream << deviceAddress << (quint8)0x0F << startingAddress << (quint16)outputValues.count() << txBytes;

    // Encode the binary values.
    quint8 tmp = 0;
    for ( int i = 0 ; i < outputValues.count() ; i++ )
    {
        if ( i % 8 == 0 )
        {
            if ( i != 0 ) pduStream << tmp;
            tmp = 0;
        }
        if ( outputValues[i] ) tmp |= 0x01 << ( i % 8 );
    }
    pduStream << tmp;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    pdu.clear();
    SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return false;
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return false;
    }

    // Read the rest of the message.
    pdu += _read( 3 );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return false;
    }

    // Check data and return them on success.
    if ( pdu.size() == 8 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        quint16 rxStartingAddress , rxQuantityOfOutputs;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> rxStartingAddress >> rxQuantityOfOutputs;
        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x0F && rxStartingAddress == startingAddress &&
             rxQuantityOfOutputs == outputValues.count() )
        {
            if ( status ) *status = Ok;
            return true;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return false;
}

bool QRtuModbus::writeMultipleRegisters( const quint8 deviceAddress , const quint16 startingAddress ,
                                          const QList<quint16> & registersValues , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create modbus write multiple registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    quint8 txBytes = registersValues.count() * 2;
    pduStream << deviceAddress << (quint8)0x10 << startingAddress << (quint16)registersValues.count() << txBytes;

    // Encode the register values.
    foreach ( quint16 reg , registersValues )
    {
        pduStream << reg;
    }

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    pdu.clear();
    SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return false;
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return false;
    }

    // Read the rest of the message.
    pdu += _read( 3 );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return false;
    }

    // Check data and return them on success.
    if ( pdu.size() == 8 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        quint16 rxStartingAddress , rxQuantityOfRegisters;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> rxStartingAddress >> rxQuantityOfRegisters;
        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x10 && rxStartingAddress == startingAddress &&
             rxQuantityOfRegisters == registersValues.count() )
        {
            if ( status ) *status = Ok;
            return true;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return false;
}

bool QRtuModbus::maskWriteRegister( const quint8 deviceAddress , const quint16 referenceAddress ,
                                     const quint16 andMask , const quint16 orMask , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return false;
    }

    // Create modbus mask write register pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x16 << referenceAddress << andMask << orMask;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    pdu.clear();
    SerialPort->waitForReadyRead(100);
    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return false;
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return false;
    }

    // Read the rest of the message.
    pdu += _read( 5 );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return false;
    }

    // Check data and return them on success.
    if ( pdu.size() == 10 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        quint16 rxReferenceAddress , rxAndMask , rxOrMask;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> rxReferenceAddress >> rxAndMask >> rxOrMask;
        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x16 &&
             rxReferenceAddress == referenceAddress && rxAndMask == andMask && rxOrMask == orMask )
        {
            if ( status ) *status = Ok;
            return true;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return false;
}


QList<quint16> QRtuModbus::writeReadMultipleRegisters( const quint8 deviceAddress ,
                                                        const quint16 writeStartingAddress ,
                                                        const QList<quint16> & writeValues ,
                                                        const quint16 readStartingAddress ,
                                                        const quint16 quantityToRead , quint8 *const status) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create modbus read holding registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x17 << readStartingAddress << quantityToRead
              << writeStartingAddress << (quint16)writeValues.count() << (quint16)( writeValues.count() * 2 );

    // Add data.
    foreach ( quint16 reg , writeValues )
    {
        pduStream << reg;
    }

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    quint16 neededRxBytes = quantityToRead * 2;
    pdu.clear();
    SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QList<quint16>();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QList<quint16>();
    }

    // Read the rest of the message.
    pdu += _read( neededRxBytes );

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QList<quint16>();
    }

    // Check data and return them on success.
    if ( pdu.size() == neededRxBytes + 5 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode , byteCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount;

        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x17 && byteCount == neededRxBytes )
        {
            // Generate response data.
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < quantityToRead ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
    }

    // If we arrive here, something went wrong.
    if ( status ) *status = UnknownError;
    return QList<quint16>();
}

QList<quint16> QRtuModbus::readFifoQueue( const quint8 deviceAddress , const quint16 fifoPointerAddress ,
                                           quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QList<quint16>();
    }

    // Create modbus read holding registers pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << (quint8)0x18 << fifoPointerAddress;

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    pdu.clear();
    SerialPort->waitForReadyRead(100);

    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QList<quint16>();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QList<quint16>();
    }

    // Read the rest of the message.
    pdu += _readAll();

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QList<quint16>();
    }

    // Check data and return them on success.
    if ( pdu.size() >= 10 )
    {
        // Read TCP fields and, device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        quint16 byteCount , fifoCount;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode >> byteCount >> fifoCount;

        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == 0x18 && byteCount == fifoCount * 2 )
        {
            QList<quint16> list;
            quint16 tmp;
            for ( int i = 0 ; i < fifoCount ; i++ )
            {
                rxStream >> tmp;
                list.append( tmp );
            }
            if ( status ) *status = Ok;
            return list;
        }
    }

    // Something went wrong...
    if ( status ) *status = UnknownError;
    return QList<quint16>();
}

QByteArray QRtuModbus::executeCustomFunction( const quint8 deviceAddress , const quint8 modbusFunction ,
                                               QByteArray &data , quint8 *const status ) const
{
    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QByteArray();
    }

    // Create modbus pdu (Modbus uses Big Endian).
    QByteArray pdu;
    QDataStream pduStream( &pdu , QIODevice::WriteOnly );
    pduStream.setByteOrder( QDataStream::BigEndian );
    pduStream << deviceAddress << modbusFunction;
    pduStream.writeRawData( data.data() , data.size() );

    // Calculate CRC and add it to the PDU.
    quint16 crc = _calculateCrc( pdu );
    pduStream.writeRawData( (const char *)&crc , sizeof( crc ) );

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the pdu.
    _write( pdu );

    // Await response.
    // Even on error we have at least 5 bytes to read.
    pdu = _read( 5 );

    // Handle timeout.
    if ( pdu.size() < 5 )
    {
        if ( status ) *status = Timeout;
        return QByteArray();
    }

    // Was it a Modbus error?
    if ( pdu[1] & 0x80 )
    {
        if ( !_checkCrc( pdu ) )
        {
            if ( status ) *status = CrcError;
        }
        else
        {
            if ( status ) *status = pdu[2];
        }
        return QByteArray();
    }

    // Read the rest of the message.
    pdu += _readAll();

    // Check CRC.
    if ( !_checkCrc( pdu ) )
    {
        if ( status ) *status = CrcError;
        return QByteArray();
    }

    // Check data and return them on success.
    if ( pdu.size() >= 4 )
    {
        // Read device address and command ID and control them.
        quint8 rxDeviceAddress , rxFunctionCode;
        QDataStream rxStream( pdu );
        rxStream.setByteOrder( QDataStream::BigEndian );
        rxStream >> rxDeviceAddress >> rxFunctionCode;

        // Control values of the fields.
        if ( rxDeviceAddress == deviceAddress && rxFunctionCode == modbusFunction )
        {
            // Convert data.
            return pdu.mid( 2 ,  pdu.size() - 4 );
        }
    }

    // Something went wrong...
    if ( status ) *status = UnknownError;
    return QByteArray();
}

QByteArray QRtuModbus::executeRaw( QByteArray &data , quint8 *const status ) const
{
    QByteArray response;

    // Are we connected ?
    if ( !isOpen() )
    {
        if ( status ) *status = NoConnection;
        return QByteArray();
    }

    // Clear the RX buffer before making the request.
    _readAll();

    // Send the data.
    _write( data );
    SerialPort->waitForReadyRead(100);

    // Await response.
    // Even on error we have at least 5 bytes to read.
    response = _readAll();

    // Handle timeout.
    if ( response.size() == 0 )
    {
        if ( status ) *status = Timeout;
        return QByteArray();
    }

    return response;
}

QByteArray QRtuModbus::calculateCheckSum( QByteArray &data ) const
{
    quint16 crc = _calculateCrc( data );
    return QByteArray( (char *)&crc , 2 );
}

QByteArray QRtuModbus::_read( const int numberBytes ) const
{
//    QByteArray data( numberBytes , 0 );

//    if ( numberBytes == 0 ) return data;

// //   while(SerialPort->bytesAvailable()<numberBytes);

    //data.append(SerialPort->read(numberBytes));
    return SerialPort->read(numberBytes);
}

QByteArray QRtuModbus::_readAll( void ) const
{
    return SerialPort->readAll();
}

QByteArray QRtuModbus::_readLine( int maxBytes ) const
{
    return SerialPort->readLine(maxBytes);
}

bool QRtuModbus::_write( QByteArray &data ) const
{
    if(SerialPort->isOpen())
    {
       SerialPort->write(data);
       return true;
    }
    else
    {
        return false;
    }
}


quint16 QRtuModbus::_calculateCrc( const QByteArray &pdu ) const
{
    quint16 crc , carryFlag , tmp;

    crc = 0xFFFF;
    for ( int i = 0 ; i < pdu.size() ; i++ )
    {
        crc = crc ^ (unsigned char)pdu.at( i );
        for (int j = 0 ; j < 8 ; j++ )
        {
            tmp = crc;
            carryFlag = tmp & 0x0001;
            crc = crc >> 1;
            if ( carryFlag == 1 )
            {
                crc = crc ^ 0xA001;
            }
        }
    }

    return crc;
}

bool QRtuModbus::_checkCrc( const QByteArray &pdu ) const
{
    QByteArray msg = pdu.left( pdu.size() - 2 );

    quint16 crc = _calculateCrc( msg );

    return pdu.endsWith( QByteArray( (const char *)&crc , 2 ) );
}

void QRtuModbus::processRtuModbus()
{
    while(1)
    {
        if(isOpen())
        {
           readInputRegisters(0xA,0x1,1,NULL);
        }
        QThread::sleep(500);
    }
}
