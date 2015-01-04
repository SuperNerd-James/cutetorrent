/****************************************************************************
** Meta object code from reading C++ file 'ISerachProvider.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ISerachProvider.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ISerachProvider.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ISerachProvider[] =
{

// content:
    6,       // revision
    0,       // classname
    0,    0, // classinfo
    2,   14, // methods
    0,    0, // properties
    0,    0, // enums/sets
    0,    0, // constructors
    0,       // flags
    2,       // signalCount

// signals: signature, parameters, type, tag, flags
    24,   17,   16,   16, 0x05,
    64,   58,   16,   16, 0x05,

    0        // eod
};

static const char qt_meta_stringdata_ISerachProvider[] =
{
    "ISerachProvider\0\0result\0"
    "SearchReady(QList<SearchResult*>)\0"
    "error\0Error(QString)\0"
};

void ISerachProvider::qt_static_metacall(QObject* _o, QMetaObject::Call _c, int _id, void** _a)
{
    if(_c == QMetaObject::InvokeMetaMethod)
    {
        Q_ASSERT(staticMetaObject.cast(_o));
        ISerachProvider* _t = static_cast<ISerachProvider*>(_o);

        switch(_id)
        {
            case 0:
                _t->SearchReady((*reinterpret_cast< QList<SearchResult*>(*) > (_a[1])));
                break;

            case 1:
                _t->Error((*reinterpret_cast< QString(*) > (_a[1])));
                break;

            default:
                ;
        }
    }
}

const QMetaObjectExtraData ISerachProvider::staticMetaObjectExtraData =
{
    0,  qt_static_metacall
};

const QMetaObject ISerachProvider::staticMetaObject =
{
    {
        &QObject::staticMetaObject, qt_meta_stringdata_ISerachProvider,
        qt_meta_data_ISerachProvider, &staticMetaObjectExtraData
    }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject& ISerachProvider::getStaticMetaObject()
{
    return staticMetaObject;
}
#endif //Q_NO_DATA_RELOCATION

const QMetaObject* ISerachProvider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void* ISerachProvider::qt_metacast(const char* _clname)
{
    if(!_clname) { return 0; }

    if(!strcmp(_clname, qt_meta_stringdata_ISerachProvider))
    { return static_cast<void*>(const_cast< ISerachProvider*>(this)); }

    return QObject::qt_metacast(_clname);
}

int ISerachProvider::qt_metacall(QMetaObject::Call _c, int _id, void** _a)
{
    _id = QObject::qt_metacall(_c, _id, _a);

    if(_id < 0)
    { return _id; }

    if(_c == QMetaObject::InvokeMetaMethod)
    {
        if(_id < 2)
        { qt_static_metacall(this, _c, _id, _a); }

        _id -= 2;
    }

    return _id;
}
QT_END_MOC_NAMESPACE
