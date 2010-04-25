/****************************************************************************
** Meta object code from reading C++ file 'QPropertyEditor.h'
**
** Created: Sat 24. Apr 18:21:08 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QPropertyEditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QPropertyEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QPropertyEditor[] = {

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

static const char qt_meta_stringdata_QPropertyEditor[] = {
    "QPropertyEditor\0"
};

const QMetaObject QPropertyEditor::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_QPropertyEditor,
      qt_meta_data_QPropertyEditor, 0 }
};

const QMetaObject *QPropertyEditor::metaObject() const
{
    return &staticMetaObject;
}

void *QPropertyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QPropertyEditor))
        return static_cast<void*>(const_cast< QPropertyEditor*>(this));
    if (!strcmp(_clname, "TPropertyEditor<QPropertiesView>"))
        return static_cast< TPropertyEditor<QPropertiesView>*>(const_cast< QPropertyEditor*>(this));
    return QFrame::qt_metacast(_clname);
}

int QPropertyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
