/****************************************************************************
** Meta object code from reading C++ file 'MultipleDTDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MultipleDTDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MultipleDTDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MultipleDTDialog[] =
{

// content:
    6,       // revision
    0,       // classname
    0,    0, // classinfo
    1,   14, // methods
    0,    0, // properties
    0,    0, // enums/sets
    0,    0, // constructors
    0,       // flags
    0,       // signalCount

// slots: signature, parameters, type, tag, flags
    18,   17,   17,   17, 0x0a,

    0        // eod
};

static const char qt_meta_stringdata_MultipleDTDialog[] =
{
    "MultipleDTDialog\0\0MountSelectedFILE()\0"
};

void MultipleDTDialog::qt_static_metacall(QObject* _o, QMetaObject::Call _c, int _id, void** _a)
{
    if(_c == QMetaObject::InvokeMetaMethod)
    {
        Q_ASSERT(staticMetaObject.cast(_o));
        MultipleDTDialog* _t = static_cast<MultipleDTDialog*>(_o);

        switch(_id)
        {
            case 0:
                _t->MountSelectedFILE();
                break;

            default:
                ;
        }
    }

    Q_UNUSED(_a);
}

const QMetaObjectExtraData MultipleDTDialog::staticMetaObjectExtraData =
{
    0,  qt_static_metacall
};

const QMetaObject MultipleDTDialog::staticMetaObject =
{
    {
        &BaseWindow<QDialog>::staticMetaObject, qt_meta_stringdata_MultipleDTDialog,
        qt_meta_data_MultipleDTDialog, &staticMetaObjectExtraData
    }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject& MultipleDTDialog::getStaticMetaObject()
{
    return staticMetaObject;
}
#endif //Q_NO_DATA_RELOCATION

const QMetaObject* MultipleDTDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void* MultipleDTDialog::qt_metacast(const char* _clname)
{
    if(!_clname)
    {
        return 0;
    }

    if(!strcmp(_clname, qt_meta_stringdata_MultipleDTDialog))
    {
        return static_cast<void*>(const_cast< MultipleDTDialog*>(this));
    }

    if(!strcmp(_clname, "Ui::DTMountDialog"))
    {
        return static_cast< Ui::DTMountDialog*>(const_cast< MultipleDTDialog*>(this));
    }

    return BaseWindow<QDialog>::qt_metacast(_clname);
}

int MultipleDTDialog::qt_metacall(QMetaObject::Call _c, int _id, void** _a)
{
    _id = BaseWindow<QDialog>::qt_metacall(_c, _id, _a);

    if(_id < 0)
    {
        return _id;
    }

    if(_c == QMetaObject::InvokeMetaMethod)
    {
        if(_id < 1)
        {
            qt_static_metacall(this, _c, _id, _a);
        }

        _id -= 1;
    }

    return _id;
}
QT_END_MOC_NAMESPACE
