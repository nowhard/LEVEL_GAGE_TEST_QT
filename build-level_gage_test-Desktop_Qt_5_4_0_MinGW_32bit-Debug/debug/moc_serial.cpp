/****************************************************************************
** Meta object code from reading C++ file 'serial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../level_gage_test/serial/serial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Serial_t {
    QByteArrayData data[22];
    char stringdata[237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Serial_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Serial_t qt_meta_stringdata_Serial = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Serial"
QT_MOC_LITERAL(1, 7, 14), // "finishedSerial"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 6), // "error_"
QT_MOC_LITERAL(4, 30, 3), // "err"
QT_MOC_LITERAL(5, 34, 9), // "outSerial"
QT_MOC_LITERAL(6, 44, 4), // "data"
QT_MOC_LITERAL(7, 49, 16), // "disconnectSerial"
QT_MOC_LITERAL(8, 66, 13), // "connectSerial"
QT_MOC_LITERAL(9, 80, 19), // "writeSettingsSerial"
QT_MOC_LITERAL(10, 100, 4), // "name"
QT_MOC_LITERAL(11, 105, 8), // "baudrate"
QT_MOC_LITERAL(12, 114, 8), // "DataBits"
QT_MOC_LITERAL(13, 123, 6), // "Parity"
QT_MOC_LITERAL(14, 130, 8), // "StopBits"
QT_MOC_LITERAL(15, 139, 11), // "FlowControl"
QT_MOC_LITERAL(16, 151, 13), // "processSerial"
QT_MOC_LITERAL(17, 165, 13), // "writeToSerial"
QT_MOC_LITERAL(18, 179, 10), // "readSerial"
QT_MOC_LITERAL(19, 190, 11), // "handleError"
QT_MOC_LITERAL(20, 202, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(21, 231, 5) // "error"

    },
    "Serial\0finishedSerial\0\0error_\0err\0"
    "outSerial\0data\0disconnectSerial\0"
    "connectSerial\0writeSettingsSerial\0"
    "name\0baudrate\0DataBits\0Parity\0StopBits\0"
    "FlowControl\0processSerial\0writeToSerial\0"
    "readSerial\0handleError\0"
    "QSerialPort::SerialPortError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Serial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   71,    2, 0x0a /* Public */,
       8,    0,   72,    2, 0x0a /* Public */,
       9,    6,   73,    2, 0x0a /* Public */,
      16,    0,   86,    2, 0x0a /* Public */,
      17,    1,   87,    2, 0x0a /* Public */,
      18,    0,   90,    2, 0x0a /* Public */,
      19,    1,   91,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,   10,   11,   12,   13,   14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void Serial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Serial *_t = static_cast<Serial *>(_o);
        switch (_id) {
        case 0: _t->finishedSerial(); break;
        case 1: _t->error_((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->outSerial((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->disconnectSerial(); break;
        case 4: _t->connectSerial(); break;
        case 5: _t->writeSettingsSerial((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 6: _t->processSerial(); break;
        case 7: _t->writeToSerial((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->readSerial(); break;
        case 9: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Serial::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Serial::finishedSerial)) {
                *result = 0;
            }
        }
        {
            typedef void (Serial::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Serial::error_)) {
                *result = 1;
            }
        }
        {
            typedef void (Serial::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Serial::outSerial)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Serial::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Serial.data,
      qt_meta_data_Serial,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Serial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Serial::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Serial.stringdata))
        return static_cast<void*>(const_cast< Serial*>(this));
    return QObject::qt_metacast(_clname);
}

int Serial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Serial::finishedSerial()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Serial::error_(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Serial::outSerial(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
