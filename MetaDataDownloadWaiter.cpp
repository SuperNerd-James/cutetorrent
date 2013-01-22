#include "MetaDataDownloadWaiter.h"



MetaDataDownloadWaiter::MetaDataDownloadWaiter( QString metaLink,QObject* parrent/*=NULL*/ ) : QThread(parrent)
{
	MetaLink=metaLink;
	_tManager = TorrentManager::getInstance();
}

MetaDataDownloadWaiter::~MetaDataDownloadWaiter()
{
	TorrentManager::freeInstance();
}

void MetaDataDownloadWaiter::run()
{
	torrent_handle h=_tManager->ProcessMagnetLink(MetaLink);

	openmagnet_info ti =*_tManager->GetTorrentInfo(h);
	emit DownloadCompleted(ti);

	
}
