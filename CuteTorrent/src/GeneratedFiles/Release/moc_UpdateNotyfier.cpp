/****************************************************************************
** Meta object code from reading C++ file 'UpdateNotyfier.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../UpdateNotyfier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UpdateNotyfier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UpdateNotifier[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      42,   15,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      70,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UpdateNotifier[] = {
    "UpdateNotifier\0\0showUpdateNitify(QString)\0"
    "showNoUpdateNitify(QString)\0"
    "replyFinished(QNetworkReply*)\0"
};

void UpdateNotifier::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        UpdateNotifier *_t = static_cast<UpdateNotifier *>(_o);
        switch (_id) {
        case 0: _t->showUpdateNitify((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->showNoUpdateNitify((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->replyFinished((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData UpdateNotifier::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UpdateNotifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UpdateNotifier,
      qt_meta_data_UpdateNotifier, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UpdateNotifier::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UpdateNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UpdateNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UpdateNotifier))
        return static_cast<void*>(const_cast< UpdateNotifier*>(this));
    return QObject::qt_metacast(_clname);
}

int UpdateNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UpdateNotifier::showUpdateNitify(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpdateNotifier::showNoUpdateNitify(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
