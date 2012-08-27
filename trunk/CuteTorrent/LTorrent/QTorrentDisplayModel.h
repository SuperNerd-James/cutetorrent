/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _TOR_DIPL_MODEL_H
#define _TOR_DIPL_MODEL_H
#include <QMap>
#include <QVector>
#include <QSet>
#include <QAbstractListModel>
#include <QListView>
#include "Torrent.h"
#include "TorrentManager.h"
#include <QMenu>
#include <QTimer>
#include <QMutex>
class Torrent;
class TorrentManager;

class QTorrentDisplayModel : public QAbstractListModel
{
	Q_OBJECT
private:
	QMap<int,int> id_to_row;
	QMap<int,Torrent*> id_to_torrent;
	QVector<Torrent*> torrents;
	QVector<Torrent*> torrents_to_remove,torrents_to_add;
	int auto_id;
	QListView* parrent;
	int selectedRow;
	QMenu* menu;
	QAction* openDir;
	QAction* DTmount;
	QAction* HashRecheck;
	QAction* DelAll;
	QAction* DelTorrentOnly;
	QAction* setSequentual;
	TorrentManager* mgr;
	QTimer* timer;
	QMutex* locker;
public:
	QTorrentDisplayModel(QListView* parrent);
	~QTorrentDisplayModel();
	enum action{pause,resume,remove};
	void ActionOnSelectedItem(action wtf);
	void ChangeData(int row);
	QTorrentDisplayModel(QObject* parrent=NULL);
	bool hasTorrent(const QString& InfoHash) const;
	void clear();
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    enum Role { TorrentRole = Qt::UserRole };
	Torrent* GetSelectedTorrent();
	
	bool updateTorrent(QString infohash,torrent_status status);
signals:
	void updateTabSender(int);
	void TorrentCompletedProxySender(const QString);
	void TorrentErrorPoxySender(const QString&);
public slots:
	void AddTorrent(Torrent*);
	void UpdateSelectedIndex(const QModelIndex &);
	void TorrentErrorProxy(const QString&);
	void TorrentCompletedProxy(const QString);
	void contextualMenu(const QPoint &);
	void OpenDirSelected();
	void DellTorrentOnly();
	void Rehash();
	void DellAll();
	void MountDT();
	void updateVisibleTorrents();
	
};


#endif