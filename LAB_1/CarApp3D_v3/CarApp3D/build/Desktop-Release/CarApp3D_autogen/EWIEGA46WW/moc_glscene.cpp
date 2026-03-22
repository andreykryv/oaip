/****************************************************************************
** Meta object code from reading C++ file 'glscene.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../glscene.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glscene.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN7GlSceneE_t {};
} // unnamed namespace

template <> constexpr inline auto GlScene::qt_create_metaobjectdata<qt_meta_tag_ZN7GlSceneE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GlScene",
        "doorAnimStep",
        "",
        "angle",
        "doorAnimDone"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'doorAnimStep'
        QtMocHelpers::SignalData<void(float)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Float, 3 },
        }}),
        // Signal 'doorAnimDone'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GlScene, qt_meta_tag_ZN7GlSceneE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GlScene::staticMetaObject = { {
    QMetaObject::SuperData::link<MovingRectangle::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7GlSceneE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7GlSceneE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7GlSceneE_t>.metaTypes,
    nullptr
} };

void GlScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GlScene *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->doorAnimStep((*reinterpret_cast<std::add_pointer_t<float>>(_a[1]))); break;
        case 1: _t->doorAnimDone(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GlScene::*)(float )>(_a, &GlScene::doorAnimStep, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GlScene::*)()>(_a, &GlScene::doorAnimDone, 1))
            return;
    }
}

const QMetaObject *GlScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7GlSceneE_t>.strings))
        return static_cast<void*>(this);
    return MovingRectangle::qt_metacast(_clname);
}

int GlScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MovingRectangle::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void GlScene::doorAnimStep(float _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void GlScene::doorAnimDone()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
