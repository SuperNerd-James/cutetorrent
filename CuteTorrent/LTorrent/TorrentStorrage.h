#pragma once
#include <QObject>
#include "Torrent.h"
#include <QHash>
#include <QString>
#include <QMutex>
class Torrent;
class TorrentStorrage :
	public QObject
{
	Q_OBJECT
private:
	static TorrentStorrage* instance;
	static int instance_count;
	QMap<QString,Torrent*> torrentsMap;
	QVector<QMap<QString,Torrent*>::Iterator> torrents;
	QMutex* locker;
public:
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
};
