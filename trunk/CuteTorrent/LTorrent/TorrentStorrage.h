#pragma once
#include <QObject>
#include "Torrent.h"
#include <QHash>
#include <QString>
#include <QMutex>
#include <QTimer>
class Torrent;
#include "defs.h"
class TorrentStorrage :
	public QObject
{
	Q_OBJECT

public:
	
	void setFilter(FilterType filter);
	void setGroupFilter(QString filter);
	static TorrentStorrage* getInstance();
	static void freeInstance();
	QList<Torrent*>::iterator begin();
	QList<Torrent*>::iterator end();
	void append(Torrent*);
	void remove(Torrent*);
	void remove(QString);
	void sort();
	bool hasTorrent(Torrent*);
	bool hasTorrent(QString);
	Torrent* at(int index);
	void clear();
	int count();
	Torrent* & operator [] (int index);
protected:
	TorrentStorrage(QObject* parrent=NULL);
	~TorrentStorrage(void);
private:
	static TorrentStorrage* instance;
	static int instance_count;
	QMap<QString,Torrent*> torrentsMap;
	QVector<QMap<QString,Torrent*>::Iterator> torrents;
	QVector<QMap<QString,Torrent*>::Iterator> filteredTorrents;
	QMutex* locker;
	FilterType currentFilter;
	QString groupFilter;
	QTimer* timer;
	void filterBybasicFilter();
	void filterByGroup();
private slots:	
	void filterData();
};
