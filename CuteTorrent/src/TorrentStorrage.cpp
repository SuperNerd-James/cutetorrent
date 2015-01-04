#include "TorrentStorrage.h"

TorrentStorrage* TorrentStorrage::m_pInstance = NULL;

int TorrentStorrage::m_nInstanceCount = 0;

TorrentStorrage* TorrentStorrage::getInstance()
{
	//qDebug() << "TorrentStorrage::getInstance";
	if(m_pInstance == NULL)
	{
		m_pInstance = new TorrentStorrage();
	}

	m_nInstanceCount++;
	return m_pInstance;
}

void TorrentStorrage::freeInstance()
{
	//qDebug() << "TorrentStorrage::freeInstance";
	m_nInstanceCount--;

	if(m_nInstanceCount == 0)
	{
		delete m_pInstance;
	}
}
bool LessThan(QMap<QString, Torrent*>::Iterator left, QMap<QString, Torrent*>::Iterator right)
{
	return * (left.value()) < * (right.value());
}
void TorrentStorrage::append(Torrent* torrent)
{
	QString infoHash = torrent->GetInfoHash();

	if(hasTorrent(infoHash))
	{
		//	delete torrent;
		return;
	}

	locker->lock();
	//qDebug() << "TorrentStorrage::append";
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.insert(infoHash, torrent);
	m_torrents.append(it);

	switch(m_torrentFilter)
	{
		case ACTIVE:
		{
			if(it.value()->isActive())
			{
				m_filteredTorrents.append(it);
			}

			break;
		}

		case DOWNLOADING:
		{
			if(it.value()->isDownloading())
			{
				m_filteredTorrents.append(it);
			}

			break;
		}

		case SEEDING:
		{
			if(it.value()->isSeeding())
			{
				m_filteredTorrents.append(it);
			}

			break;
		}

		case COMPLETED:
		{
			if(it.value()->GetProgress() == 100)
			{
				m_filteredTorrents.append(it);
			}

			break;
		}

		case NONE:
		default:
		{
			m_filteredTorrents.append(it);
			break;
		}
	}

	locker->unlock();
}

void TorrentStorrage::remove(Torrent* torrent)
{
	locker->lock();
	//qDebug() << "TorrentStorrage::remove(Torrent*)";
	remove(torrent->GetInfoHash());
	locker->unlock();
}

Torrent* TorrentStorrage::getTorrent(QString infoHash)
{
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.find(infoHash);

	if(it == m_torrentsMap.end())
	{
		return NULL;
	}

	return it.value();
}

void TorrentStorrage::remove(QString infoHash)
{
	//qDebug() << "TorrentStorrage::remove" << infoHash;
	QMap<QString, Torrent*>::Iterator it = m_torrentsMap.find(infoHash);
	int index = m_torrents.indexOf(it);
	int index2 = m_filteredTorrents.lastIndexOf(it);

	//qDebug() << "Found " << index << it.value();
	if(index >= 0)
	{
		//qDebug() << "Before Removing" << torrentsMap.count();
		m_torrents.remove(index);
		m_torrentsMap.remove(infoHash);
		//qDebug() << "After Removing" << torrentsMap.count();
	}

	if(index2 >= 0)
	{
		m_filteredTorrents.remove(index2);
	}
}

bool TorrentStorrage::hasTorrent(Torrent* tor)
{
	//qDebug() << "TorrentStorrage::hasTorrent(Torrent*)";
	return hasTorrent(tor->GetInfoHash());
}

bool TorrentStorrage::hasTorrent(QString infoHash)
{
	//qDebug() << "TorrentStorrage::hasTorrent(QString)";
	return m_torrentsMap.contains(infoHash);
}

TorrentStorrage::~TorrentStorrage(void)
{
	//qDebug() << "TorrentStorrage::~TorrentStorrage";
	qDeleteAll(m_torrentsMap);
	qDeleteAll(m_torrents);
}

Torrent* TorrentStorrage::at(int index)
{
	//qDebug() << "TorrentStorrage::at";
	if(index < m_torrentsMap.size())
	{
		return m_filteredTorrents.at(index).value();
	}
	else
	{
		return NULL;
	}
}

QList<Torrent*>::iterator TorrentStorrage::begin()
{
	//qDebug() << "TorrentStorrage::begin";
	return m_torrentsMap.values().begin();
}

QList<Torrent*>::iterator TorrentStorrage::end()
{
	//qDebug() << "TorrentStorrage::end";
	return m_torrentsMap.values().end();
}

void TorrentStorrage::clear()
{
	//qDebug() << "TorrentStorrage::clear";
	m_torrentsMap.clear();
	m_torrents.clear();
	m_filteredTorrents.clear();
}

int TorrentStorrage::count()
{
	//qDebug() << "TorrentStorrage::count";
	return m_filteredTorrents.count();
}

TorrentStorrage::TorrentStorrage(QObject* parrent/*=NULL*/) : QObject(parrent)
{
	locker = new QMutex();
	m_torrentFilter = NONE;
	timer = new QTimer();
	timer->setInterval(1000);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(filterData()));
	timer->start();
}

Torrent* TorrentStorrage::operator[](int index)
{
	return m_torrents.at(index).value();
}

Torrent* TorrentStorrage::operator[](QString index)
{
	return m_torrentsMap[index];
}

void TorrentStorrage::sort()
{
	qSort(m_torrents.begin(), m_torrents.end(), LessThan);
}

void TorrentStorrage::setTorrentFilter(TorrentFilterType filter)
{
	m_filterType = TORRENT;
	m_torrentFilter = filter;
	m_groupFilter = "";
	filterData();
}

void TorrentStorrage::filterData()
{
	m_filteredTorrents.clear();

	switch(m_filterType)
	{
		case GROUP_FILTER_TYPE:
		{
			filterByGroup();
			break;
		}

		case TORRENT:
		{
			switch(m_torrentFilter)
			{
				case ACTIVE:
				{
					for(int i = 0; i < m_torrents.count(); i++)
					{
						if(m_torrents[i].value()->isActive())
						{
							m_filteredTorrents.append(m_torrents[i]);
						}
					}

					break;
				}

				case NOT_ACTIVE:
				{
					for(int i = 0; i < m_torrents.count(); i++)
					{
						if(m_torrents[i].value()->GetUploadSpeed().isEmpty() && m_torrents[i].value()->GetDwonloadSpeed().isEmpty())
						{
							m_filteredTorrents.append(m_torrents[i]);
						}
					}

					break;
				}

				case DOWNLOADING:
				{
					for(int i = 0; i < m_torrents.count(); i++)
					{
						if(m_torrents[i].value()->isDownloading())
						{
							m_filteredTorrents.append(m_torrents[i]);
						}
					}

					break;
				}

				case SEEDING:
				{
					for(int i = 0; i < m_torrents.count(); i++)
					{
						if(m_torrents[i].value()->isSeeding())
						{
							m_filteredTorrents.append(m_torrents[i]);
						}
					}

					break;
				}

				case COMPLETED:
				{
					for(int i = 0; i < m_torrents.count(); i++)
					{
						if(m_torrents[i].value()->GetProgress() == 100)
						{
							m_filteredTorrents.append(m_torrents[i]);
						}
					}

					break;
				}

				case NONE:
				{
					m_filteredTorrents = m_torrents;
					break;
				}
			}

			break;
		}

		default:
			m_filteredTorrents = m_torrents;
			break;
	}
}

void TorrentStorrage::setGroupFilter(QString filter)
{
	m_filterType = GROUP_FILTER_TYPE;
	m_groupFilter = filter;
	filterData();
}

void TorrentStorrage::filterByGroup()
{
	for(int i = 0; i < m_torrents.count(); i++)
	{
		qDebug() << m_torrents[i].value()->GetName() << m_torrents[i].value()->GetGroup() << m_groupFilter;

		if(m_torrents[i].value()->GetGroup().compare(m_groupFilter, Qt::CaseInsensitive) == 0)
		{
			qDebug() << "chosen";
			m_filteredTorrents.append(m_torrents[i]);
		}
	}
}



