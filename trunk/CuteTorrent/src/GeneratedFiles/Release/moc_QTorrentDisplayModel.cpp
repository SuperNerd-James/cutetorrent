/****************************************************************************
** Meta object code from reading C++ file 'QTorrentDisplayModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QTorrentDisplayModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QTorrentDisplayModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QTorrentDisplayModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      38,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   21,   21,   21, 0x0a,
      84,   21,   21,   21, 0x0a,
     120,   21,   21,   21, 0x0a,
     143,   21,   21,   21, 0x0a,
     161,   21,   21,   21, 0x0a,
     179,   21,   21,   21, 0x0a,
     188,   21,   21,   21, 0x0a,
     205,   21,   21,   21, 0x0a,
     215,   21,   21,   21, 0x0a,
     225,   21,   21,   21, 0x0a,
     240,   21,   21,   21, 0x0a,
     258,   21,   21,   21, 0x0a,
     273,   21,   21,   21, 0x0a,
     297,   21,   21,   21, 0x0a,
     312,   21,   21,   21, 0x0a,
     334,   21,   21,   21, 0x0a,
     355,   21,   21,   21, 0x0a,
     369,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QTorrentDisplayModel[] = {
    "QTorrentDisplayModel\0\0initCompleted()\0"
    "updateTabSender(int)\0onTorrentRemove(QString)\0"
    "UpdateSelectedIndex(QItemSelection)\0"
    "contextualMenu(QPoint)\0OpenDirSelected()\0"
    "DellTorrentOnly()\0Rehash()\0UpdateTrackers()\0"
    "DellAll()\0MountDT()\0playInPlayer()\0"
    "setSequentualDL()\0moveStorrage()\0"
    "updateVisibleTorrents()\0SetSuperSeed()\0"
    "initSessionFinished()\0generateMagnetLink()\0"
    "changeGroup()\0setupContextMenu()\0"
};

void QTorrentDisplayModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QTorrentDisplayModel *_t = static_cast<QTorrentDisplayModel *>(_o);
        switch (_id) {
        case 0: _t->initCompleted(); break;
        case 1: _t->updateTabSender((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onTorrentRemove((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->UpdateSelectedIndex((*reinterpret_cast< const QItemSelection(*)>(_a[1]))); break;
        case 4: _t->contextualMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->OpenDirSelected(); break;
        case 6: _t->DellTorrentOnly(); break;
        case 7: _t->Rehash(); break;
        case 8: _t->UpdateTrackers(); break;
        case 9: _t->DellAll(); break;
        case 10: _t->MountDT(); break;
        case 11: _t->playInPlayer(); break;
        case 12: _t->setSequentualDL(); break;
        case 13: _t->moveStorrage(); break;
        case 14: _t->updateVisibleTorrents(); break;
        case 15: _t->SetSuperSeed(); break;
        case 16: _t->initSessionFinished(); break;
        case 17: _t->generateMagnetLink(); break;
        case 18: _t->changeGroup(); break;
        case 19: _t->setupContextMenu(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QTorrentDisplayModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTorrentDisplayModel::staticMetaObject = {
    { &QAbstractListModel::staticMetaObject, qt_meta_stringdata_QTorrentDisplayModel,
      qt_meta_data_QTorrentDisplayModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTorrentDisplayModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTorrentDisplayModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTorrentDisplayModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTorrentDisplayModel))
        return static_cast<void*>(const_cast< QTorrentDisplayModel*>(this));
    return QAbstractListModel::qt_metacast(_clname);
}

int QTorrentDisplayModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void QTorrentDisplayModel::initCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QTorrentDisplayModel::updateTabSender(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
