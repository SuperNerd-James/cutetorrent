/****************************************************************************
** Meta object code from reading C++ file 'FileViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FileViewModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FileViewModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileViewModel[] =
{

// content:
    6,       // revision
    0,       // classname
    0,    0, // classinfo
    7,   14, // methods
    0,    0, // properties
    0,    0, // enums/sets
    0,    0, // constructors
    0,       // flags
    0,       // signalCount

// slots: signature, parameters, type, tag, flags
    21,   15,   14,   14, 0x0a,
    48,   14,   14,   14, 0x0a,
    66,   14,   14,   14, 0x0a,
    85,   14,   14,   14, 0x0a,
    108,   14,   14,   14, 0x0a,
    134,   14,   14,   14, 0x0a,
    158,   14,   14,   14, 0x0a,

    0        // eod
};

static const char qt_meta_stringdata_FileViewModel[] =
{
    "FileViewModel\0\0point\0FileTabContextMenu(QPoint)\0"
    "OpenDirSelected()\0OpenFileSelected()\0"
    "SetLowForCurrentFile()\0SetMediumForCurrentFile()\0"
    "SetHighForCurrentFile()\0"
    "SetNotDownloadForCurrentFile()\0"
};

void FileViewModel::qt_static_metacall(QObject* _o, QMetaObject::Call _c, int _id, void** _a)
{
    if(_c == QMetaObject::InvokeMetaMethod)
    {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileViewModel* _t = static_cast<FileViewModel*>(_o);

        switch(_id)
        {
            case 0:
                _t->FileTabContextMenu((*reinterpret_cast< const QPoint(*) > (_a[1])));
                break;

            case 1:
                _t->OpenDirSelected();
                break;

            case 2:
                _t->OpenFileSelected();
                break;

            case 3:
                _t->SetLowForCurrentFile();
                break;

            case 4:
                _t->SetMediumForCurrentFile();
                break;

            case 5:
                _t->SetHighForCurrentFile();
                break;

            case 6:
                _t->SetNotDownloadForCurrentFile();
                break;

            default:
                ;
        }
    }
}

const QMetaObjectExtraData FileViewModel::staticMetaObjectExtraData =
{
    0,  qt_static_metacall
};

const QMetaObject FileViewModel::staticMetaObject =
{
    {
        &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_FileViewModel,
        qt_meta_data_FileViewModel, &staticMetaObjectExtraData
    }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject& FileViewModel::getStaticMetaObject()
{
    return staticMetaObject;
}
#endif //Q_NO_DATA_RELOCATION

const QMetaObject* FileViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void* FileViewModel::qt_metacast(const char* _clname)
{
    if(!_clname) { return 0; }

    if(!strcmp(_clname, qt_meta_stringdata_FileViewModel))
    { return static_cast<void*>(const_cast< FileViewModel*>(this)); }

    return QAbstractItemModel::qt_metacast(_clname);
}

int FileViewModel::qt_metacall(QMetaObject::Call _c, int _id, void** _a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);

    if(_id < 0)
    { return _id; }

    if(_c == QMetaObject::InvokeMetaMethod)
    {
        if(_id < 7)
        { qt_static_metacall(this, _c, _id, _a); }

        _id -= 7;
    }

    return _id;
}
QT_END_MOC_NAMESPACE
