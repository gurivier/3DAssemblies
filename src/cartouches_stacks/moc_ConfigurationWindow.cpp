/****************************************************************************
** Meta object code from reading C++ file 'ConfigurationWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ConfigurationWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConfigurationWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConfigurationWindow_t {
    QByteArrayData data[24];
    char stringdata[479];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConfigurationWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConfigurationWindow_t qt_meta_stringdata_ConfigurationWindow = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 13),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 17),
QT_MOC_LITERAL(4, 53, 8),
QT_MOC_LITERAL(5, 62, 19),
QT_MOC_LITERAL(6, 82, 10),
QT_MOC_LITERAL(7, 93, 27),
QT_MOC_LITERAL(8, 121, 9),
QT_MOC_LITERAL(9, 131, 23),
QT_MOC_LITERAL(10, 155, 19),
QT_MOC_LITERAL(11, 175, 4),
QT_MOC_LITERAL(12, 180, 28),
QT_MOC_LITERAL(13, 209, 9),
QT_MOC_LITERAL(14, 219, 33),
QT_MOC_LITERAL(15, 253, 33),
QT_MOC_LITERAL(16, 287, 32),
QT_MOC_LITERAL(17, 320, 22),
QT_MOC_LITERAL(18, 343, 7),
QT_MOC_LITERAL(19, 351, 24),
QT_MOC_LITERAL(20, 376, 29),
QT_MOC_LITERAL(21, 406, 24),
QT_MOC_LITERAL(22, 431, 7),
QT_MOC_LITERAL(23, 439, 39)
    },
    "ConfigurationWindow\0timerFunction\0\0"
    "onContrastChanged\0contrast\0"
    "onLuminosityChanged\0luminosity\0"
    "onGrayscaleThresholdChanged\0threshold\0"
    "onWhiteThresholdChanged\0onTargetSizeChanged\0"
    "size\0onAreaTargetDetectionChanged\0"
    "sizeRatio\0onImageViewModeRadioButtonClicked\0"
    "onResetConfigurationButtonClicked\0"
    "onSaveConfigurationButtonClicked\0"
    "onApplyContrastClicked\0checked\0"
    "onApplyLuminosityClicked\0"
    "onGrayscaleRadioButtonToggled\0"
    "onGrayscaleInvertClicked\0clicked\0"
    "onAreaTargetDetectionRadiobuttonToggled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConfigurationWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x0a /* Public */,
       3,    1,   90,    2, 0x0a /* Public */,
       5,    1,   93,    2, 0x0a /* Public */,
       7,    1,   96,    2, 0x0a /* Public */,
       9,    1,   99,    2, 0x0a /* Public */,
      10,    1,  102,    2, 0x0a /* Public */,
      12,    1,  105,    2, 0x0a /* Public */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    1,  111,    2, 0x08 /* Private */,
      19,    1,  114,    2, 0x08 /* Private */,
      20,    1,  117,    2, 0x08 /* Private */,
      21,    1,  120,    2, 0x08 /* Private */,
      23,    1,  123,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Bool,   22,
    QMetaType::Void, QMetaType::Bool,   18,

       0        // eod
};

void ConfigurationWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConfigurationWindow *_t = static_cast<ConfigurationWindow *>(_o);
        switch (_id) {
        case 0: _t->timerFunction(); break;
        case 1: _t->onContrastChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onLuminosityChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onGrayscaleThresholdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onWhiteThresholdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onTargetSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->onAreaTargetDetectionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->onImageViewModeRadioButtonClicked(); break;
        case 8: _t->onResetConfigurationButtonClicked(); break;
        case 9: _t->onSaveConfigurationButtonClicked(); break;
        case 10: _t->onApplyContrastClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onApplyLuminosityClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->onGrayscaleRadioButtonToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->onGrayscaleInvertClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->onAreaTargetDetectionRadiobuttonToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ConfigurationWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ConfigurationWindow.data,
      qt_meta_data_ConfigurationWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *ConfigurationWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigurationWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConfigurationWindow.stringdata))
        return static_cast<void*>(const_cast< ConfigurationWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ConfigurationWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
