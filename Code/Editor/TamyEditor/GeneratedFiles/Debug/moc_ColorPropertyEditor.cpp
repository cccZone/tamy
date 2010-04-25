/****************************************************************************
** Meta object code from reading C++ file 'ColorPropertyEditor.h'
**
** Created: Sat 24. Apr 18:21:09 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ColorPropertyEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ColorPropertyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ColorPropertyEditor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      25,   21,   20,   20, 0x0a,
      45,   21,   20,   20, 0x0a,
      65,   21,   20,   20, 0x0a,
      85,   21,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ColorPropertyEditor[] = {
    "ColorPropertyEditor\0\0val\0rValChanged(double)\0"
    "gValChanged(double)\0bValChanged(double)\0"
    "aValChanged(double)\0"
};

const QMetaObject ColorPropertyEditor::staticMetaObject = {
    { &QPropertyEditor::staticMetaObject, qt_meta_stringdata_ColorPropertyEditor,
      qt_meta_data_ColorPropertyEditor, 0 }
};

const QMetaObject *ColorPropertyEditor::metaObject() const
{
    return &staticMetaObject;
}

void *ColorPropertyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColorPropertyEditor))
        return static_cast<void*>(const_cast< ColorPropertyEditor*>(this));
    return QPropertyEditor::qt_metacast(_clname);
}

int ColorPropertyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPropertyEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: gValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: bValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: aValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
