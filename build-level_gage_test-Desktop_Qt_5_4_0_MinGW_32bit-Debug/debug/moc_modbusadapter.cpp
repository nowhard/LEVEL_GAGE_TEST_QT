/****************************************************************************
** Meta object code from reading C++ file 'modbusadapter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../level_gage_test/src/modbusadapter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modbusadapter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ModbusAdapter_t {
    QByteArrayData data[6];
    char stringdata[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModbusAdapter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModbusAdapter_t qt_meta_stringdata_ModbusAdapter = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ModbusAdapter"
QT_MOC_LITERAL(1, 14, 11), // "refreshView"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 17), // "modbusTransaction"
QT_MOC_LITERAL(4, 45, 13), // "resetCounters"
QT_MOC_LITERAL(5, 59, 7) // "timeOut"

    },
    "ModbusAdapter\0refreshView\0\0modbusTransaction\0"
    "resetCounters\0timeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModbusAdapter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ModbusAdapter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ModbusAdapter *_t = static_cast<ModbusAdapter *>(_o);
        switch (_id) {
        case 0: _t->refreshView(); break;
        case 1: _t->modbusTransaction(); break;
        case 2: _t->resetCounters(); break;
        case 3: _t->timeOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ModbusAdapter::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ModbusAdapter::refreshView)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject ModbusAdapter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ModbusAdapter.data,
      qt_meta_data_ModbusAdapter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ModbusAdapter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModbusAdapter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ModbusAdapter.stringdata))
        return static_cast<void*>(const_cast< ModbusAdapter*>(this));
    return QObject::qt_metacast(_clname);
}

int ModbusAdapter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ModbusAdapter::refreshView()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
