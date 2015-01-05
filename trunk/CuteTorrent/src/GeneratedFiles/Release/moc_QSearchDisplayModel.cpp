/****************************************************************************
** Meta object code from reading C++ file 'QSearchDisplayModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QSearchDisplayModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QSearchDisplayModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QSearchDisplayModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,
      42,   20,   20,   20, 0x0a,
      65,   20,   20,   20, 0x0a,
      84,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QSearchDisplayModel[] = {
    "QSearchDisplayModel\0\0OnNewSearchResults()\0"
    "contextualMenu(QPoint)\0setupContextMenu()\0"
    "retranslate()\0"
};

void QSearchDisplayModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QSearchDisplayModel *_t = static_cast<QSearchDisplayModel *>(_o);
        switch (_id) {
        case 0: _t->OnNewSearchResults(); break;
        case 1: _t->contextualMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->setupContextMenu(); break;
        case 3: _t->retranslate(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QSearchDisplayModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QSearchDisplayModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_QSearchDisplayModel,
      qt_meta_data_QSearchDisplayModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QSearchDisplayModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QSearchDisplayModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QSearchDisplayModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QSearchDisplayModel))
        return static_cast<void*>(const_cast< QSearchDisplayModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int QSearchDisplayModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
