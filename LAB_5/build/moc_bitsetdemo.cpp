/****************************************************************************
** Meta object code from reading C++ file 'bitsetdemo.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../bitsetdemo.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bitsetdemo.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BitSetDemo_t {
    QByteArrayData data[19];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BitSetDemo_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BitSetDemo_t qt_meta_stringdata_BitSetDemo = {
    {
QT_MOC_LITERAL(0, 0, 10), // "BitSetDemo"
QT_MOC_LITERAL(1, 11, 13), // "createBitSets"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "toggleBit"
QT_MOC_LITERAL(4, 36, 6), // "setNum"
QT_MOC_LITERAL(5, 43, 5), // "index"
QT_MOC_LITERAL(6, 49, 10), // "performAll"
QT_MOC_LITERAL(7, 60, 10), // "performAny"
QT_MOC_LITERAL(8, 71, 11), // "performNone"
QT_MOC_LITERAL(9, 83, 12), // "performCount"
QT_MOC_LITERAL(10, 96, 14), // "performFlipAll"
QT_MOC_LITERAL(11, 111, 14), // "performFlipBit"
QT_MOC_LITERAL(12, 126, 10), // "performSet"
QT_MOC_LITERAL(13, 137, 12), // "performReset"
QT_MOC_LITERAL(14, 150, 10), // "performAND"
QT_MOC_LITERAL(15, 161, 9), // "performOR"
QT_MOC_LITERAL(16, 171, 10), // "performNOT"
QT_MOC_LITERAL(17, 182, 10), // "showString"
QT_MOC_LITERAL(18, 193, 10) // "showULLong"

    },
    "BitSetDemo\0createBitSets\0\0toggleBit\0"
    "setNum\0index\0performAll\0performAny\0"
    "performNone\0performCount\0performFlipAll\0"
    "performFlipBit\0performSet\0performReset\0"
    "performAND\0performOR\0performNOT\0"
    "showString\0showULLong"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BitSetDemo[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    2,   90,    2, 0x08 /* Private */,
       6,    0,   95,    2, 0x08 /* Private */,
       7,    0,   96,    2, 0x08 /* Private */,
       8,    0,   97,    2, 0x08 /* Private */,
       9,    0,   98,    2, 0x08 /* Private */,
      10,    0,   99,    2, 0x08 /* Private */,
      11,    0,  100,    2, 0x08 /* Private */,
      12,    0,  101,    2, 0x08 /* Private */,
      13,    0,  102,    2, 0x08 /* Private */,
      14,    0,  103,    2, 0x08 /* Private */,
      15,    0,  104,    2, 0x08 /* Private */,
      16,    0,  105,    2, 0x08 /* Private */,
      17,    0,  106,    2, 0x08 /* Private */,
      18,    0,  107,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BitSetDemo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BitSetDemo *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->createBitSets(); break;
        case 1: _t->toggleBit((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->performAll(); break;
        case 3: _t->performAny(); break;
        case 4: _t->performNone(); break;
        case 5: _t->performCount(); break;
        case 6: _t->performFlipAll(); break;
        case 7: _t->performFlipBit(); break;
        case 8: _t->performSet(); break;
        case 9: _t->performReset(); break;
        case 10: _t->performAND(); break;
        case 11: _t->performOR(); break;
        case 12: _t->performNOT(); break;
        case 13: _t->showString(); break;
        case 14: _t->showULLong(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BitSetDemo::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_BitSetDemo.data,
    qt_meta_data_BitSetDemo,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BitSetDemo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BitSetDemo::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BitSetDemo.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BitSetDemo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
