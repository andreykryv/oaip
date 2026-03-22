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
        "sortChanged",
        "error",
        "message",
        "openFileDialog",
        "saveFileDialog",
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
        "getStudent",
        "QVariantMap",
        "filtered",
        "filterType",
        "nameSearch",
        "sortByName",
        "sortByGroup",
        "sortByAvgDesc",
        "allStudents",
        "debtCount",
        "expelCount",
        "okCount",
        "sortColumn",
        "sortAsc"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'studentsChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sortChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Slot 'openFileDialog'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'saveFileDialog'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'openFile'
        QtMocHelpers::SlotData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'saveFile'
        QtMocHelpers::SlotData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'addStudent'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QVariantList &, const QVariantList &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { 0x80000000 | 14, 15 }, { 0x80000000 | 14, 16 },
        }}),
        // Slot 'deleteStudent'
        QtMocHelpers::SlotData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'editStudent'
        QtMocHelpers::SlotData<void(int, const QString &, const QString &, const QVariantList &, const QVariantList &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { 0x80000000 | 14, 15 },
            { 0x80000000 | 14, 16 },
        }}),
        // Slot 'getStudent'
        QtMocHelpers::SlotData<QVariantMap(int) const>(20, 2, QMC::AccessPublic, 0x80000000 | 21, {{
            { QMetaType::Int, 18 },
        }}),
        // Slot 'filtered'
        QtMocHelpers::SlotData<QVariantList(int, const QString &) const>(22, 2, QMC::AccessPublic, 0x80000000 | 14, {{
            { QMetaType::Int, 23 }, { QMetaType::QString, 24 },
        }}),
        // Slot 'sortByName'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sortByGroup'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'sortByAvgDesc'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'allStudents'
        QtMocHelpers::PropertyData<QVariantList>(28, 0x80000000 | 14, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 0),
        // property 'debtCount'
        QtMocHelpers::PropertyData<int>(29, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'expelCount'
        QtMocHelpers::PropertyData<int>(30, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'okCount'
        QtMocHelpers::PropertyData<int>(31, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'sortColumn'
        QtMocHelpers::PropertyData<QString>(32, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'sortAsc'
        QtMocHelpers::PropertyData<bool>(33, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
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
        case 1: _t->sortChanged(); break;
        case 2: _t->error((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->openFileDialog(); break;
        case 4: _t->saveFileDialog(); break;
        case 5: _t->openFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->saveFile((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->addStudent((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[4]))); break;
        case 8: _t->deleteStudent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->editStudent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<QVariantList>>(_a[5]))); break;
        case 10: { QVariantMap _r = _t->getStudent((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QVariantMap*>(_a[0]) = std::move(_r); }  break;
        case 11: { QVariantList _r = _t->filtered((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->sortByName(); break;
        case 13: _t->sortByGroup(); break;
        case 14: _t->sortByAvgDesc(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (StudentController::*)()>(_a, &StudentController::studentsChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (StudentController::*)()>(_a, &StudentController::sortChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (StudentController::*)(const QString & )>(_a, &StudentController::error, 2))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->allStudents(); break;
        case 1: *reinterpret_cast<int*>(_v) = _t->debtCount(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->expelCount(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->okCount(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->sortColumn(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->sortAsc(); break;
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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void StudentController::studentsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void StudentController::sortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void StudentController::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
