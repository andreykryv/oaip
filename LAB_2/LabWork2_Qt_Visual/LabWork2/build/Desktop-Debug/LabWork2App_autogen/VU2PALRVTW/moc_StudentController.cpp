/****************************************************************************
** Meta object code from reading C++ file 'StudentController.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/backend/StudentController.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StudentController.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17StudentControllerE_t {};
} // unnamed namespace

template <> constexpr inline auto StudentController::qt_create_metaobjectdata<qt_meta_tag_ZN17StudentControllerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "StudentController",
        "studentsChanged",
        "",
        "error",
        "message",
        "openFile",
        "path",
        "saveFile",
        "addStudent",
        "group",
        "name",
        "QVariantList",
        "winter",
        "summer",
        "deleteStudent",
        "index",
        "editStudent",
        "filtered",
        "filterType",
        "nameSearch",
        "sortByName",
        "sortByGroup",
        "sortByAvgDesc",
        "allStudents",
        "debtCount",
        "expelCount",
        "okCount"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'studentsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'openFile'
        QtMocHelpers::SlotData<void(const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Slot 'saveFile'
        QtMocHelpers::SlotData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 },
        }}),
        // Slot 'addStudent'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QVariantList &, const QVariantList &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { 0x80000000 | 11, 12 }, { 0x80000000 | 11, 13 },
        }}),
        // Slot 'deleteStudent'
        QtMocHelpers::SlotData<void(int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'editStudent'
        QtMocHelpers::SlotData<void(int, const QString &, const QString &, const QVariantList &, const QVariantList &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 15 }, { QMetaType::QString, 9 }, { QMetaType::QString, 10 }, { 0x80000000 | 11, 12 },
            { 0x80000000 | 11, 13 },
        }}),
        // Slot 'filtered'
        QtMocHelpers::SlotData<QVariantList(int, const QString &) const>(17, 2, QMC::AccessPublic, 0x80000000 | 11, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 19 },
        }}),
        // Slot 'sortByName'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sortByGroup'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sortByAvgDesc'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'allStudents'
        QtMocHelpers::PropertyData<QVariantList>(23, 0x80000000 | 11, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'debtCount'
        QtMocHelpers::PropertyData<int>(24, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'expelCount'
        QtMocHelpers::PropertyData<int>(25, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'okCount'
        QtMocHelpers::PropertyData<int>(26, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<StudentController, qt_meta_tag_ZN17StudentControllerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject StudentController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17StudentControllerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17StudentControllerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17StudentControllerE_t>.metaTypes,
    nullptr
} };

void StudentController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<StudentController *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->studentsChanged(); break;
        case 1: _t->error((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->openFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->saveFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->addStudent((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[4]))); break;
        case 5: _t->deleteStudent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->editStudent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[5]))); break;
        case 7: { QVariantList _r = _t->filtered((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->sortByName(); break;
        case 9: _t->sortByGroup(); break;
        case 10: _t->sortByAvgDesc(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (StudentController::*)()>(_a, &StudentController::studentsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (StudentController::*)(const QString & )>(_a, &StudentController::error, 1))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->allStudents(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->debtCount(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->expelCount(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->okCount(); break;
        default: break;
        }
    }
}

const QMetaObject *StudentController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StudentController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17StudentControllerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int StudentController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void StudentController::studentsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void StudentController::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}
QT_WARNING_POP
