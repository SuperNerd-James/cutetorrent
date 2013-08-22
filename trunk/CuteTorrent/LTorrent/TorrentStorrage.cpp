#include "TorrentStorrage.h"

TorrentStorrage* TorrentStorrage::instance=NULL;

int TorrentStorrage::instance_count=0;

TorrentStorrage* TorrentStorrage::getInstance()
{
	//qDebug() << "TorrentStorrage::getInstance";
	if (instance==NULL)
	{
		instance = new TorrentStorrage();
	}
	instance_count++;
	return instance;
}

void TorrentStorrage::freeInstance()
{
	//qDebug() << "TorrentStorrage::freeInstance";
	instance_count--;
	if (instance_count==0)
	{
		delete instance;
	}
}
bool LessThan(QMap<QString,Torrent*>::Iterator left,QMap<QString,Torrent*>::Iterator right)
{
	return *(left.value()) < *(right.value());
}
void TorrentStorrage::append( Torrent* torrent)
{
	QString infoHash=torrent->GetInfoHash();
	if (hasTorrent(infoHash))
	{
	//	delete torrent;
		return;
	}
	locker->lock();
	//qDebug() << "TorrentStorrage::append";
	torrents.append(torrentsMap.insert(infoHash,torrent));
	locker->unlock();
	
}

void TorrentStorrage::remove( Torrent* torrent )
{
	locker->lock();
	//qDebug() << "TorrentStorrage::remove(Torrent*)";
	remove(torrent->GetInfoHash());
	locker->unlock();
}

void TorrentStorrage::remove( QString infoHash)
{
	//qDebug() << "TorrentStorrage::remove(QString)";
	QMap<QString,Torrent*>::Iterator it=torrentsMap.find(infoHash);
	torrentsMap.remove(infoHash);
	int index=torrents.indexOf(it);
	if (index>0)
		torrents.remove(index);
}

bool TorrentStorrage::hasTorrent( Torrent* tor )
{
	//qDebug() << "TorrentStorrage::hasTorrent(Torrent*)";
	return hasTorrent(tor->GetInfoHash());
}

bool TorrentStorrage::hasTorrent( QString infoHash)
{
	//qDebug() << "TorrentStorrage::hasTorrent(QString)";
	return torrentsMap.contains(infoHash);
}

TorrentStorrage::~TorrentStorrage( void )
{
	//qDebug() << "TorrentStorrage::~TorrentStorrage";
	qDeleteAll(torrentsMap);
	qDeleteAll(torrents);
}

Torrent* TorrentStorrage::at( int index )
{
	//qDebug() << "TorrentStorrage::at";
	if (index < torrentsMap.size()) {
		return torrents.at(index).value();
	} else {
		return NULL;
	}
}

QList<Torrent*>::iterator TorrentStorrage::begin()
{
	//qDebug() << "TorrentStorrage::begin";
	return torrentsMap.values().begin();
}

QList<Torrent*>::iterator TorrentStorrage::end()
{
	//qDebug() << "TorrentStorrage::end";
	return torrentsMap.values().end();
}

void TorrentStorrage::clear()
{
	//qDebug() << "TorrentStorrage::clear";
	torrentsMap.clear();
	torrents.clear();
}

int TorrentStorrage::count()
{
	//qDebug() << "TorrentStorrage::count";
	return torrentsMap.count();
}

TorrentStorrage::TorrentStorrage( QObject* parrent/*=NULL*/ ) : QObject(parrent)
{
	locker = new QMutex();
}

Torrent*& TorrentStorrage::operator[]( int index )
{
	return torrents.at(index).value();
}

void TorrentStorrage::sort()
{
	qSort(torrents.begin(),torrents.end(),LessThan);
}
