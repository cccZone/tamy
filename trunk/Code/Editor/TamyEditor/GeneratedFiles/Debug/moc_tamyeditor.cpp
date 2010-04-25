/****************************************************************************
** Meta object code from reading C++ file 'tamyeditor.h'
**
** Created: Sat 24. Apr 20:33:01 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../tamyeditor.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tamyeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TamyEditor[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      25,   11,   11,   11, 0x0a,
      37,   11,   11,   11, 0x0a,
      49,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TamyEditor[] = {
    "TamyEditor\0\0updateMain()\0loadScene()\0"
    "saveScene()\0importFromIWF()\0"
};

const QMetaObject TamyEditor::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TamyEditor,
      qt_meta_data_TamyEditor, 0 }
};

const QMetaObject *TamyEditor::metaObject() const
{
    return &staticMetaObject;
}

void *TamyEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TamyEditor))
        return static_cast<void*>(const_cast< TamyEditor*>(this));
    if (!strcmp(_clname, "ComponentsManager<TamyEditor>"))
        return static_cast< ComponentsManager<TamyEditor>*>(const_cast< TamyEditor*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int TamyEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateMain(); break;
        case 1: loadScene(); break;
        case 2: saveScene(); break;
        case 3: importFromIWF(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
