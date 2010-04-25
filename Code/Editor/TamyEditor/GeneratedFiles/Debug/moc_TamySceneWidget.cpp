/****************************************************************************
** Meta object code from reading C++ file 'TamySceneWidget.h'
**
** Created: Sat 24. Apr 18:21:08 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TamySceneWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TamySceneWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TamySceneWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

       0        // eod
};

static const char qt_meta_stringdata_TamySceneWidget[] = {
    "TamySceneWidget\0"
};

const QMetaObject TamySceneWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TamySceneWidget,
      qt_meta_data_TamySceneWidget, 0 }
};

const QMetaObject *TamySceneWidget::metaObject() const
{
    return &staticMetaObject;
}

void *TamySceneWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TamySceneWidget))
        return static_cast<void*>(const_cast< TamySceneWidget*>(this));
    if (!strcmp(_clname, "UserInputController"))
        return static_cast< UserInputController*>(const_cast< TamySceneWidget*>(this));
    if (!strcmp(_clname, "Component<TamyEditor>"))
        return static_cast< Component<TamyEditor>*>(const_cast< TamySceneWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int TamySceneWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
