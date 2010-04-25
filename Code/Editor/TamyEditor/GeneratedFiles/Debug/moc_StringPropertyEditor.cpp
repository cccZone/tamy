/****************************************************************************
** Meta object code from reading C++ file 'StringPropertyEditor.h'
**
** Created: Sat 24. Apr 18:21:08 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../StringPropertyEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StringPropertyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StringPropertyEditor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      26,   22,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StringPropertyEditor[] = {
    "StringPropertyEditor\0\0val\0valChanged(QString)\0"
};

const QMetaObject StringPropertyEditor::staticMetaObject = {
    { &QPropertyEditor::staticMetaObject, qt_meta_stringdata_StringPropertyEditor,
      qt_meta_data_StringPropertyEditor, 0 }
};

const QMetaObject *StringPropertyEditor::metaObject() const
{
    return &staticMetaObject;
}

void *StringPropertyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StringPropertyEditor))
        return static_cast<void*>(const_cast< StringPropertyEditor*>(this));
    return QPropertyEditor::qt_metacast(_clname);
}

int StringPropertyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPropertyEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
