/****************************************************************************
** Meta object code from reading C++ file 'qrtumodbus.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../level_gage_test/qmodbus/qrtumodbus.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qrtumodbus.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QRtuModbus_t {
    QByteArrayData data[3];
    char stringdata[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QRtuModbus_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QRtuModbus_t qt_meta_stringdata_QRtuModbus = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QRtuModbus"
QT_MOC_LITERAL(1, 11, 16), // "processRtuModbus"
QT_MOC_LITERAL(2, 28, 0) // ""

    },
    "QRtuModbus\0processRtuModbus\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QRtuModbus[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QRtuModbus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QRtuModbus *_t = static_cast<QRtuModbus *>(_o);
        switch (_id) {
        case 0: _t->processRtuModbus(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QRtuModbus::staticMetaObject = {
    { &QAbstractModbus::staticMetaObject, qt_meta_stringdata_QRtuModbus.data,
      qt_meta_data_QRtuModbus,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QRtuModbus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QRtuModbus::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QRtuModbus.stringdata))
        return static_cast<void*>(const_cast< QRtuModbus*>(this));
    return QAbstractModbus::qt_metacast(_clname);
}

int QRtuModbus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractModbus::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
