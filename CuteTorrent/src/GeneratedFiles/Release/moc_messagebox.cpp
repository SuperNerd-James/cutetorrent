/****************************************************************************
** Meta object code from reading C++ file 'messagebox.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../messagebox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messagebox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MyMessageBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      46,   13,   13,   13, 0x08,
      55,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MyMessageBox[] = {
    "MyMessageBox\0\0buttonClicked(QAbstractButton*)\0"
    "accept()\0reject()\0"
};

void MyMessageBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MyMessageBox *_t = static_cast<MyMessageBox *>(_o);
        switch (_id) {
        case 0: _t->buttonClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 1: _t->accept(); break;
        case 2: _t->reject(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MyMessageBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MyMessageBox::staticMetaObject = {
    { &BaseWindow<QDialog>::staticMetaObject, qt_meta_stringdata_MyMessageBox,
      qt_meta_data_MyMessageBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyMessageBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyMessageBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyMessageBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyMessageBox))
        return static_cast<void*>(const_cast< MyMessageBox*>(this));
    return BaseWindow<QDialog>::qt_metacast(_clname);
}

int MyMessageBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseWindow<QDialog>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
