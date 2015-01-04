#include "MetaDataDownloadWaiter.h"

MetaDataDownloadWaiter::MetaDataDownloadWaiter(QString metaLink, QObject* parrent/*=NULL*/, bool autoAdd/*=false*/) : QThread(parrent)
{
    MetaLink = metaLink;
    _autoAdd = autoAdd;
    _tManager = TorrentManager::getInstance();
}

MetaDataDownloadWaiter::~MetaDataDownloadWaiter()
{
    TorrentManager::freeInstance();
}

void MetaDataDownloadWaiter::run()
{
    error_code ec;
    torrent_handle h = _tManager->ProcessMagnetLink(MetaLink, ec);

    if(!_autoAdd)
    {
        openmagnet_info* ti = _tManager->GetTorrentInfo(h);
        ti->link = MetaLink;
        emit DownloadCompleted(*ti);
    }
    else
    {
        _tManager->AddMagnet(h, QString::fromStdString(h.save_path()), "", QMap<QString, qint8>());
    }

    //this->deleteLater();
}
