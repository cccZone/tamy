/****************************************************************************
** Meta object code from reading C++ file 'MtxPropertyEditor.h'
**
** Created: Sat 24. Apr 18:21:09 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MtxPropertyEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MtxPropertyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MtxPropertyEditor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      23,   19,   18,   18, 0x0a,
      45,   19,   18,   18, 0x0a,
      69,   19,   18,   18, 0x0a,
      92,   19,   18,   18, 0x0a,
     112,   19,   18,   18, 0x0a,
     132,   19,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MtxPropertyEditor[] = {
    "MtxPropertyEditor\0\0val\0yawValChanged(double)\0"
    "pitchValChanged(double)\0rollValChanged(double)\0"
    "xValChanged(double)\0yValChanged(double)\0"
    "zValChanged(double)\0"
};

const QMetaObject MtxPropertyEditor::staticMetaObject = {
    { &QPropertyEditor::staticMetaObject, qt_meta_stringdata_MtxPropertyEditor,
      qt_meta_data_MtxPropertyEditor, 0 }
};

const QMetaObject *MtxPropertyEditor::metaObject() const
{
    return &staticMetaObject;
}

void *MtxPropertyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MtxPropertyEditor))
        return static_cast<void*>(const_cast< MtxPropertyEditor*>(this));
    return QPropertyEditor::qt_metacast(_clname);
}

int MtxPropertyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPropertyEditor::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: yawValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: pitchValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: rollValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: xValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: yValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: zValChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
