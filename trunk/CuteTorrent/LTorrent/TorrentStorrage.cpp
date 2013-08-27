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
	QMap<QString,Torrent*>::Iterator it= torrentsMap.insert(infoHash,torrent);
	torrents.append(it);
	switch(currentFilter)
	{
	case ACTIVE:
		{
			
				if (!it.value()->GetUploadSpeed().isEmpty() || !it.value()->GetDwonloadSpeed().isEmpty())
				{
					filteredTorrents.append(it);
				}
			
			break;
		}
	case DWONLOADING:
		{
		
				if (it.value()->isDownloading())
				{
					filteredTorrents.append(it);
				}
		
			break;
		}
	case SEEDING:
		{
		
				if (it.value()->isSeeding())
				{
					filteredTorrents.append(it);
				}
		
			break;
		}
	case COMPLETED:
		{
		
				if (it.value()->GetProgress()==100)
				{
					filteredTorrents.append(it);
				}
		
			break;
		}
	case NONE:
		{
			filteredTorrents.append(it);
			break;
		}
	}
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
	//qDebug() << "TorrentStorrage::remove" << infoHash;
	QMap<QString,Torrent*>::Iterator it=torrentsMap.find(infoHash);
	int index=torrents.indexOf(it);
	int index2 = filteredTorrents.lastIndexOf(it);
	//qDebug() << "Found " << index << it.value();
	if (index>=0)
	{
		//qDebug() << "Before Removing" << torrentsMap.count();
		torrents.remove(index);
		torrentsMap.remove(infoHash);
		//qDebug() << "After Removing" << torrentsMap.count();
	}
	if (index2>=0)
	{
		filteredTorrents.remove(index2);
	}

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
		return filteredTorrents.at(index).value();
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
	filteredTorrents.clear();
}

int TorrentStorrage::count()
{
	//qDebug() << "TorrentStorrage::count";
	return filteredTorrents.count();
}

TorrentStorrage::TorrentStorrage( QObject* parrent/*=NULL*/ ) : QObject(parrent)
{
	locker = new QMutex();
	currentFilter=NONE;
	timer = new QTimer();
	timer->setInterval(1000);
	QObject::connect(timer,SIGNAL(timeout()), this, SLOT(filterData()));
	timer->start();
}

Torrent*& TorrentStorrage::operator[]( int index )
{
	return torrents.at(index).value();
}

void TorrentStorrage::sort()
{
	qSort(torrents.begin(),torrents.end(),LessThan);
}

void TorrentStorrage::setFilter( FilterType filter )
{
	currentFilter = filter;
	groupFilter="";
    searchFilter="";
	filterData();
}

void TorrentStorrage::filterData()
{
    filteredTorrents.clear();
    switch(currentFilter)
    {
    case ACTIVE:
        {
            for(int i=0;i<torrents.count();i++)
            {
                if (!torrents[i].value()->GetUploadSpeed().isEmpty() || !torrents[i].value()->GetDwonloadSpeed().isEmpty())
                {
                    filteredTorrents.append(torrents[i]);
                }
            }
            break;
        }
    case NOT_ACTIVE:
        {
            for(int i=0;i<torrents.count();i++)
            {
                if (torrents[i].value()->GetUploadSpeed().isEmpty() && torrents[i].value()->GetDwonloadSpeed().isEmpty())
                {
                    filteredTorrents.append(torrents[i]);
                }
            }
            break;
        }
    case DWONLOADING:
        {
            for(int i=0;i<torrents.count();i++)
            {
                if (torrents[i].value()->isDownloading())
                {
                    filteredTorrents.append(torrents[i]);
                }
            }
            break;
        }
    case SEEDING:
        {
            for(int i=0;i<torrents.count();i++)
            {
                if (torrents[i].value()->isSeeding())
                {
                    filteredTorrents.append(torrents[i]);
                }
            }
            break;
        }
    case COMPLETED:
        {
            for(int i=0;i<torrents.count();i++)
            {
                if (torrents[i].value()->GetProgress()==100)
                {
                    filteredTorrents.append(torrents[i]);
                }
            }
            break;
        }
    case NONE:
        {
            filteredTorrents = torrents;
            break;
        }
    case MY_GROUP_FILTER:
        {
            filterByGroup();
            break;
        }
    case SEARCH:
        {
            filterBySearch();
            break;
        }
    }
}

void TorrentStorrage::setGroupFilter( QString filter )
{
    currentFilter = MY_GROUP_FILTER;
	groupFilter = filter;
    filterData();
}

void TorrentStorrage::setSearchFilter(QString filter)
{
    currentFilter = SEARCH;
    searchFilter = filter;
    filterData();
}

void TorrentStorrage::filterByGroup()
{
    for(int i=0;i<torrents.count();i++)
	{
		if (torrents[i].value()->GetGroup().toLower() == groupFilter.toLower())
		{
			filteredTorrents.append(torrents[i]);
		}
    }
}

void TorrentStorrage::filterBySearch()
{

    if (searchFilter.isEmpty())
    {
        filteredTorrents=torrents;
        return;
    }
    for(int i=0;i<torrents.count();i++)
    {
        if (torrents[i].value()->GetName().toLower().contains(searchFilter.toLower(),Qt::CaseInsensitive))
        {
            filteredTorrents.append(torrents[i]);
        }
    }
}


