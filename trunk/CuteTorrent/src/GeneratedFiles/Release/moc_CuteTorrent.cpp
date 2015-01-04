/****************************************************************************
** Meta object code from reading C++ file 'CuteTorrent.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CuteTorrent.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CuteTorrent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CuteTorrent[] =
{

// content:
    6,       // revision
    0,       // classname
    0,    0, // classinfo
    37,   14, // methods
    0,    0, // properties
    0,    0, // enums/sets
    0,    0, // constructors
    0,       // flags
    0,       // signalCount

// slots: signature, parameters, type, tag, flags
    13,   12,   12,   12, 0x0a,
    39,   12,   12,   12, 0x0a,
    53,   12,   12,   12, 0x0a,
    67,   12,   12,   12, 0x0a,
    79,   12,   12,   12, 0x0a,
    97,   12,   12,   12, 0x0a,
    123,   12,   12,   12, 0x0a,
    151,   12,   12,   12, 0x0a,
    167,  165,   12,   12, 0x0a,
    201,   12,   12,   12, 0x0a,
    220,  165,   12,   12, 0x0a,
    264,  165,   12,   12, 0x0a,
    303,   12,   12,   12, 0x0a,
    329,   12,   12,   12, 0x0a,
    353,   12,   12,   12, 0x0a,
    369,   12,   12,   12, 0x0a,
    384,   12,   12,   12, 0x0a,
    401,   12,   12,   12, 0x0a,
    418,   12,   12,   12, 0x0a,
    434,   12,   12,   12, 0x0a,
    450,   12,   12,   12, 0x0a,
    466,   12,   12,   12, 0x0a,
    485,   12,   12,   12, 0x0a,
    513,  506,   12,   12, 0x0a,
    562,   12,   12,   12, 0x0a,
    583,   12,   12,   12, 0x0a,
    599,   12,   12,   12, 0x0a,
    614,   12,   12,   12, 0x0a,
    632,   12,   12,   12, 0x0a,
    652,   12,   12,   12, 0x0a,
    670,   12,   12,   12, 0x0a,
    683,   12,   12,   12, 0x0a,
    693,   12,   12,   12, 0x0a,
    715,   12,   12,   12, 0x0a,
    735,   12,   12,   12, 0x0a,
    756,   12,   12,   12, 0x0a,
    777,   12,   12,   12, 0x0a,

    0        // eod
};

static const char qt_meta_stringdata_CuteTorrent[] =
{
    "CuteTorrent\0\0HandleNewTorrent(QString)\0"
    "UpdateUL(int)\0UpdateDL(int)\0ShowAbout()\0"
    "CheckForUpdates()\0ShowUpdateNitify(QString)\0"
    "ShowNoUpdateNitify(QString)\0Retranslate()\0"
    ",\0ShowTorrentError(QString,QString)\0"
    "EnableNitifyShow()\0"
    "ShowTorrentCompletedNotyfy(QString,QString)\0"
    "ShowTorrentInfoNotyfy(QString,QString)\0"
    "ShowCreateTorrentDialog()\0"
    "ShowOpenTorrentDialog()\0PauseSelected()\0"
    "StopSelected()\0ResumeSelected()\0"
    "DeleteSelected()\0UpdateInfoTab()\0"
    "UpdatePeerTab()\0UpdateFileTab()\0"
    "UpadteTrackerTab()\0OpenSettingsDialog()\0"
    "reason\0IconActivated(QSystemTrayIcon::ActivationReason)\0"
    "UpdateTabWidget(int)\0ProcessMagnet()\0"
    "PeformSearch()\0CopyDiscribtion()\0"
    "ClearPieceDisplay()\0initWindowIcons()\0"
    "AddTracker()\0AddPeer()\0ChnageTorrentFilter()\0"
    "startBackUpWizard()\0OnGotSerachResults()\0"
    "maximizeBtnClicked()\0minimizeBtnClicked()\0"
};

void CuteTorrent::qt_static_metacall(QObject* _o, QMetaObject::Call _c, int _id, void** _a)
{
    if(_c == QMetaObject::InvokeMetaMethod)
    {
        Q_ASSERT(staticMetaObject.cast(_o));
        CuteTorrent* _t = static_cast<CuteTorrent*>(_o);

        switch(_id)
        {
            case 0:
                _t->HandleNewTorrent((*reinterpret_cast< const QString(*) > (_a[1])));
                break;

            case 1:
                _t->UpdateUL((*reinterpret_cast< int (*) > (_a[1])));
                break;

            case 2:
                _t->UpdateDL((*reinterpret_cast< int (*) > (_a[1])));
                break;

            case 3:
                _t->ShowAbout();
                break;

            case 4:
                _t->CheckForUpdates();
                break;

            case 5:
                _t->ShowUpdateNitify((*reinterpret_cast< const QString(*) > (_a[1])));
                break;

            case 6:
                _t->ShowNoUpdateNitify((*reinterpret_cast< const QString(*) > (_a[1])));
                break;

            case 7:
                _t->Retranslate();
                break;

            case 8:
                _t->ShowTorrentError((*reinterpret_cast< const QString(*) > (_a[1])), (*reinterpret_cast< const QString(*) > (_a[2])));
                break;

            case 9:
                _t->EnableNitifyShow();
                break;

            case 10:
                _t->ShowTorrentCompletedNotyfy((*reinterpret_cast< const QString(*) > (_a[1])), (*reinterpret_cast< const QString(*) > (_a[2])));
                break;

            case 11:
                _t->ShowTorrentInfoNotyfy((*reinterpret_cast< const QString(*) > (_a[1])), (*reinterpret_cast< const QString(*) > (_a[2])));
                break;

            case 12:
                _t->ShowCreateTorrentDialog();
                break;

            case 13:
                _t->ShowOpenTorrentDialog();
                break;

            case 14:
                _t->PauseSelected();
                break;

            case 15:
                _t->StopSelected();
                break;

            case 16:
                _t->ResumeSelected();
                break;

            case 17:
                _t->DeleteSelected();
                break;

            case 18:
                _t->UpdateInfoTab();
                break;

            case 19:
                _t->UpdatePeerTab();
                break;

            case 20:
                _t->UpdateFileTab();
                break;

            case 21:
                _t->UpadteTrackerTab();
                break;

            case 22:
                _t->OpenSettingsDialog();
                break;

            case 23:
                _t->IconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*) > (_a[1])));
                break;

            case 24:
                _t->UpdateTabWidget((*reinterpret_cast< int (*) > (_a[1])));
                break;

            case 25:
                _t->ProcessMagnet();
                break;

            case 26:
                _t->PeformSearch();
                break;

            case 27:
                _t->CopyDiscribtion();
                break;

            case 28:
                _t->ClearPieceDisplay();
                break;

            case 29:
                _t->initWindowIcons();
                break;

            case 30:
                _t->AddTracker();
                break;

            case 31:
                _t->AddPeer();
                break;

            case 32:
                _t->ChnageTorrentFilter();
                break;

            case 33:
                _t->startBackUpWizard();
                break;

            case 34:
                _t->OnGotSerachResults();
                break;

            case 35:
                _t->maximizeBtnClicked();
                break;

            case 36:
                _t->minimizeBtnClicked();
                break;

            default:
                ;
        }
    }
}

const QMetaObjectExtraData CuteTorrent::staticMetaObjectExtraData =
{
    0,  qt_static_metacall
};

const QMetaObject CuteTorrent::staticMetaObject =
{
    {
        &BaseWindow<QWidget>::staticMetaObject, qt_meta_stringdata_CuteTorrent,
        qt_meta_data_CuteTorrent, &staticMetaObjectExtraData
    }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject& CuteTorrent::getStaticMetaObject()
{
    return staticMetaObject;
}
#endif //Q_NO_DATA_RELOCATION

const QMetaObject* CuteTorrent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void* CuteTorrent::qt_metacast(const char* _clname)
{
    if(!_clname) { return 0; }

    if(!strcmp(_clname, qt_meta_stringdata_CuteTorrent))
    { return static_cast<void*>(const_cast< CuteTorrent*>(this)); }

    return BaseWindow<QWidget>::qt_metacast(_clname);
}

int CuteTorrent::qt_metacall(QMetaObject::Call _c, int _id, void** _a)
{
    _id = BaseWindow<QWidget>::qt_metacall(_c, _id, _a);

    if(_id < 0)
    { return _id; }

    if(_c == QMetaObject::InvokeMetaMethod)
    {
        if(_id < 37)
        { qt_static_metacall(this, _c, _id, _a); }

        _id -= 37;
    }

    return _id;
}
QT_END_MOC_NAMESPACE
