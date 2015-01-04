/****************************************************************************
** Meta object code from reading C++ file 'VideoPlayerWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../VideoPlayer/VideoPlayerWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VideoPlayerWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VideoPlayerWindow[] =
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
    19,   18,   18,   18, 0x0a,
    34,   18,   18,   18, 0x0a,
    64,   18,   18,   18, 0x0a,
    98,   18,   18,   18, 0x0a,
    135,   18,   18,   18, 0x0a,
    157,   18,   18,   18, 0x0a,
    183,   18,   18,   18, 0x0a,

    0        // eod
};

static const char qt_meta_stringdata_VideoPlayerWindow[] =
{
    "VideoPlayerWindow\0\0goFullScreen()\0"
    "OnAvailableSubtitlesChanged()\0"
    "OnAvailableAudioChannelsChanged()\0"
    "OnCustomContextMenuRequested(QPoint)\0"
    "OnSubtitleChosen(int)\0OnAudioChannelChosen(int)\0"
    "updateWindowActiveState()\0"
};

void VideoPlayerWindow::qt_static_metacall(QObject* _o, QMetaObject::Call _c, int _id, void** _a)
{
    if(_c == QMetaObject::InvokeMetaMethod)
    {
        Q_ASSERT(staticMetaObject.cast(_o));
        VideoPlayerWindow* _t = static_cast<VideoPlayerWindow*>(_o);

        switch(_id)
        {
            case 0:
                _t->goFullScreen();
                break;

            case 1:
                _t->OnAvailableSubtitlesChanged();
                break;

            case 2:
                _t->OnAvailableAudioChannelsChanged();
                break;

            case 3:
                _t->OnCustomContextMenuRequested((*reinterpret_cast< QPoint(*) > (_a[1])));
                break;

            case 4:
                _t->OnSubtitleChosen((*reinterpret_cast< int (*) > (_a[1])));
                break;

            case 5:
                _t->OnAudioChannelChosen((*reinterpret_cast< int (*) > (_a[1])));
                break;

            case 6:
                _t->updateWindowActiveState();
                break;

            default:
                ;
        }
    }
}

const QMetaObjectExtraData VideoPlayerWindow::staticMetaObjectExtraData =
{
    0,  qt_static_metacall
};

const QMetaObject VideoPlayerWindow::staticMetaObject =
{
    {
        &QMainWindow::staticMetaObject, qt_meta_stringdata_VideoPlayerWindow,
        qt_meta_data_VideoPlayerWindow, &staticMetaObjectExtraData
    }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject& VideoPlayerWindow::getStaticMetaObject()
{
    return staticMetaObject;
}
#endif //Q_NO_DATA_RELOCATION

const QMetaObject* VideoPlayerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void* VideoPlayerWindow::qt_metacast(const char* _clname)
{
    if(!_clname) { return 0; }

    if(!strcmp(_clname, qt_meta_stringdata_VideoPlayerWindow))
    { return static_cast<void*>(const_cast< VideoPlayerWindow*>(this)); }

    return QMainWindow::qt_metacast(_clname);
}

int VideoPlayerWindow::qt_metacall(QMetaObject::Call _c, int _id, void** _a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);

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
