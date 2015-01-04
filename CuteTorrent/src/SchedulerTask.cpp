#include "SchedulerTask.h"
#include "TorrentManager.h"
#include <QDebug>
SchedulerTask::SchedulerTask(QString name, TaskType type, QVariant limit, QDateTime begin, QObject* parent /*= 0*/) : QThread(parent)
{
	_name = name;
	iType = type;
	bool ok;

	if(type == LIMIT_DOWNLOAD || type == LIMIT_UPLOAD)
	{
		_limit = limit.toInt(&ok);

		if(!ok)
		{
			
		}
	}

	_begin = begin;
}

SchedulerTask::SchedulerTask(const SchedulerTask& other)
{
	iType = other.iType;
	_name  = other._name;
	_limit = other._limit;
	_begin = other._begin;
}
SchedulerTask::SchedulerTask()
{
	iType = UNKNOWN;
}

void SchedulerTask::run()
{
}

SchedulerTask::TaskType SchedulerTask::type() const
{
	return iType;
}

void SchedulerTask::pefromTask()
{
	
	TorrentManager* tManager = TorrentManager::getInstance();
	
	libtorrent::session_settings current = tManager->readSettings();

	
	switch(iType)
	{
		case START_ALL		:
			
			tManager->StartAllTorrents();
			break;

		case PAUSE_ALL		:
			
			tManager->PauseAllTorrents();
			break;

		case LIMIT_UPLOAD	:
		{
			
			current.upload_rate_limit = _limit;
			tManager->updateSettings(current);
		}
		break;

		case LIMIT_DOWNLOAD	:
		{
			
			current.download_rate_limit = _limit;
			tManager->updateSettings(current);
		}
		break;

		default:
			
			break;
	}

	TorrentManager::freeInstance();
}

bool SchedulerTask::operator< (const SchedulerTask& other) const
{
	return _begin < other.startTime();
}

QDateTime SchedulerTask::startTime() const
{
	return _begin;
}


int SchedulerTask::limit()
{
	return _limit;
}

SchedulerTask& SchedulerTask::operator= (const SchedulerTask& other)
{
	if(this == &other)
	{
		return *this;
	}

	iType = other.iType;
	_name  = other._name;
	_limit = other._limit;
	_begin = other._begin;
	return *this;
}

QString SchedulerTask::name() const
{
	return _name;
}
