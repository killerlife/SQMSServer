/****************************************************************************
** Meta object code from reading C++ file 'ModSelect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ModSelect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ModSelect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SQMSTcpSocket_t {
    QByteArrayData data[10];
    char stringdata[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SQMSTcpSocket_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SQMSTcpSocket_t qt_meta_stringdata_SQMSTcpSocket = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 12),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 28),
QT_MOC_LITERAL(4, 57, 11),
QT_MOC_LITERAL(5, 69, 13),
QT_MOC_LITERAL(6, 83, 28),
QT_MOC_LITERAL(7, 112, 11),
QT_MOC_LITERAL(8, 124, 12),
QT_MOC_LITERAL(9, 137, 9)
    },
    "SQMSTcpSocket\0stateChanged\0\0"
    "QAbstractSocket::SocketState\0socketState\0"
    "errorReceived\0QAbstractSocket::SocketError\0"
    "socketError\0disconnected\0readyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SQMSTcpSocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       5,    1,   37,    2, 0x08 /* Private */,
       8,    0,   40,    2, 0x08 /* Private */,
       9,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SQMSTcpSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SQMSTcpSocket *_t = static_cast<SQMSTcpSocket *>(_o);
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 1: _t->errorReceived((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 2: _t->disconnected(); break;
        case 3: _t->readyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    }
}

const QMetaObject SQMSTcpSocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_SQMSTcpSocket.data,
      qt_meta_data_SQMSTcpSocket,  qt_static_metacall, 0, 0}
};


const QMetaObject *SQMSTcpSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SQMSTcpSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SQMSTcpSocket.stringdata))
        return static_cast<void*>(const_cast< SQMSTcpSocket*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int SQMSTcpSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_SQMSModSelect_t {
    QByteArrayData data[3];
    char stringdata[26];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SQMSModSelect_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SQMSModSelect_t qt_meta_stringdata_SQMSModSelect = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 10),
QT_MOC_LITERAL(2, 25, 0)
    },
    "SQMSModSelect\0newConnect\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SQMSModSelect[] = {

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
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void SQMSModSelect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SQMSModSelect *_t = static_cast<SQMSModSelect *>(_o);
        switch (_id) {
        case 0: _t->newConnect(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject SQMSModSelect::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SQMSModSelect.data,
      qt_meta_data_SQMSModSelect,  qt_static_metacall, 0, 0}
};


const QMetaObject *SQMSModSelect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SQMSModSelect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SQMSModSelect.stringdata))
        return static_cast<void*>(const_cast< SQMSModSelect*>(this));
    if (!strcmp(_clname, "IModSelect"))
        return static_cast< IModSelect*>(const_cast< SQMSModSelect*>(this));
    return QObject::qt_metacast(_clname);
}

int SQMSModSelect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
