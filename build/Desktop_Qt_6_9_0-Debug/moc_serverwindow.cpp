/****************************************************************************
** Meta object code from reading C++ file 'serverwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../serverwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'serverwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN12ServerWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto ServerWindow::qt_create_metaobjectdata<qt_meta_tag_ZN12ServerWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ServerWindow",
        "ServerApplied",
        "",
        "ServerFolder",
        "ServerActivated",
        "ServerDeactivated",
        "LoadServerConfig",
        "QDir",
        "directory",
        "GetPublicIP",
        "on_listProps_currentRowChanged",
        "currentRow",
        "on_btnApply_clicked",
        "on_btnInstallServer_clicked",
        "on_btnStartServer_clicked",
        "on_btnStopServer_clicked",
        "ServerStateChanged",
        "QProcess::ProcessState",
        "state",
        "SetServerVisualState",
        "VisualState",
        "ReadOutput",
        "on_btnShowConsole_clicked",
        "on_btnConnectToServer_clicked",
        "on_btnCopyIp_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ServerApplied'
        QtMocHelpers::SignalData<void(QString)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'ServerActivated'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'ServerDeactivated'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'LoadServerConfig'
        QtMocHelpers::SlotData<void(QDir)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'GetPublicIP'
        QtMocHelpers::SlotData<QString()>(9, 2, QMC::AccessPrivate, QMetaType::QString),
        // Slot 'on_listProps_currentRowChanged'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Slot 'on_btnApply_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnInstallServer_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnStartServer_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnStopServer_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'ServerStateChanged'
        QtMocHelpers::SlotData<void(QProcess::ProcessState)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Slot 'SetServerVisualState'
        QtMocHelpers::SlotData<void(VisualState)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 20, 18 },
        }}),
        // Slot 'ReadOutput'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnShowConsole_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnConnectToServer_clicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnCopyIp_clicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ServerWindow, qt_meta_tag_ZN12ServerWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ServerWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ServerWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ServerWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12ServerWindowE_t>.metaTypes,
    nullptr
} };

void ServerWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ServerWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ServerApplied((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->ServerActivated(); break;
        case 2: _t->ServerDeactivated(); break;
        case 3: _t->LoadServerConfig((*reinterpret_cast< std::add_pointer_t<QDir>>(_a[1]))); break;
        case 4: { QString _r = _t->GetPublicIP();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->on_listProps_currentRowChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->on_btnApply_clicked(); break;
        case 7: _t->on_btnInstallServer_clicked(); break;
        case 8: _t->on_btnStartServer_clicked(); break;
        case 9: _t->on_btnStopServer_clicked(); break;
        case 10: _t->ServerStateChanged((*reinterpret_cast< std::add_pointer_t<QProcess::ProcessState>>(_a[1]))); break;
        case 11: _t->SetServerVisualState((*reinterpret_cast< std::add_pointer_t<VisualState>>(_a[1]))); break;
        case 12: _t->ReadOutput(); break;
        case 13: _t->on_btnShowConsole_clicked(); break;
        case 14: _t->on_btnConnectToServer_clicked(); break;
        case 15: _t->on_btnCopyIp_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ServerWindow::*)(QString )>(_a, &ServerWindow::ServerApplied, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ServerWindow::*)()>(_a, &ServerWindow::ServerActivated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ServerWindow::*)()>(_a, &ServerWindow::ServerDeactivated, 2))
            return;
    }
}

const QMetaObject *ServerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ServerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12ServerWindowE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ServerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void ServerWindow::ServerApplied(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ServerWindow::ServerActivated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ServerWindow::ServerDeactivated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
