/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Thu Aug 12 13:36:36 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      38,   12,   11,   11, 0x08,
      58,   12,   11,   11, 0x08,
      78,   12,   11,   11, 0x08,
      98,   12,   11,   11, 0x08,
     118,   12,   11,   11, 0x08,
     138,   11,   11,   11, 0x0a,
     155,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0angle\0slider1Changed(int)\0"
    "slider2Changed(int)\0slider3Changed(int)\0"
    "slider4Changed(int)\0slider5Changed(int)\0"
    "slider6Changed(int)\0setParamStates()\0"
    "setInitGLFrame()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slider1Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slider2Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: slider3Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: slider4Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: slider5Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: slider6Changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setParamStates(); break;
        case 7: setInitGLFrame(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
