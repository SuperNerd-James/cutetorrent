/****************************************************************************
** Meta object code from reading C++ file 'RutorSearchProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RutorSearchProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RutorSearchProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RutorSearchProvider[] = {

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
      28,   21,   20,   20, 0x05,
      68,   62,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      83,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RutorSearchProvider[] = {
    "RutorSearchProvider\0\0result\0"
    "SearchReady(QList<SearchResult*>)\0"
    "error\0Error(QString)\0replyReady(QNetworkReply*)\0"
};

void RutorSearchProvider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RutorSearchProvider *_t = static_cast<RutorSearchProvider *>(_o);
        switch (_id) {
        case 0: _t->SearchReady((*reinterpret_cast< QList<SearchResult*>(*)>(_a[1]))); break;
        case 1: _t->Error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->replyReady((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RutorSearchProvider::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RutorSearchProvider::staticMetaObject = {
    { &ISerachProvider::staticMetaObject, qt_meta_stringdata_RutorSearchProvider,
      qt_meta_data_RutorSearchProvider, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RutorSearchProvider::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RutorSearchProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RutorSearchProvider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RutorSearchProvider))
        return static_cast<void*>(const_cast< RutorSearchProvider*>(this));
    return ISerachProvider::qt_metacast(_clname);
}

int RutorSearchProvider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ISerachProvider::qt_metacall(_c, _id, _a);
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
void RutorSearchProvider::SearchReady(QList<SearchResult*> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RutorSearchProvider::Error(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
