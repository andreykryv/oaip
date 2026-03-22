/****************************************************************************
** Meta object code from reading C++ file 'star_shape.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../star_shape.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'star_shape.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN9StarShapeE_t {};
} // unnamed namespace

template <> constexpr inline auto StarShape::qt_create_metaobjectdata<qt_meta_tag_ZN9StarShapeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "StarShape"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<StarShape, qt_meta_tag_ZN9StarShapeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject StarShape::staticMetaObject = { {
    QMetaObject::SuperData::link<PolygonShape::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9StarShapeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9StarShapeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9StarShapeE_t>.metaTypes,
    nullptr
} };

void StarShape::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StarShape *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *StarShape::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StarShape::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9StarShapeE_t>.strings))
        return static_cast<void*>(this);
    return PolygonShape::qt_metacast(_clname);
}

int StarShape::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PolygonShape::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
