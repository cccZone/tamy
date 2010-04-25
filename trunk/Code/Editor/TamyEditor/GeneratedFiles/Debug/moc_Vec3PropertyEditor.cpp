/****************************************************************************
** Meta object code from reading C++ file 'Vec3PropertyEditor.h'
**
** Created: Sat 24. Apr 18:21:08 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Vec3PropertyEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Vec3PropertyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Vec3PropertyEditor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      24,   20,   19,   19, 0x0a,
      44,   20,   19,   19, 0x0a,
      64,   20,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Vec3PropertyEditor[] = {
    "Vec3PropertyEditor\0\0val\0xValChanged(double)\0"
    "yValChanged(double)\0zValChanged(double)\0"
};

const QMetaObject Vec3PropertyEditor::staticMetaObject = {
    { &QPropertyEditor::staticMetaObject, qt_meta_stringdata_Vec3PropertyEditor,
      qt_meta_data_Vec3PropertyEditor, 0 }
};

const QMetaObject *Vec3PropertyEditor::metaObject() const
{
    return &staticMetaObject;
}

void *Vec3PropertyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Vec3PropertyEditor))
        return static_cast<void*>(const_cast< Vec3PropertyEditor*>(this));
    return QPropertyEditor::qt_metacast(_clname);
}

int Vec3PropertyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPropertyEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: xValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: yValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: zValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
