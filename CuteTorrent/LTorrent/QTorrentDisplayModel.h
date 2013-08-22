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
#include <QSignalMapper>
#include <QListView>
#include "Torrent.h"
#include "TorrentManager.h"
#include <QMenu>
#include <QTimer>
#include <QMutex>
#include <QThread>
#include <TorrentStorrage.h>
class Torrent;
class TorrentManager;
class QTorrentDisplayModel : public QAbstractListModel
{
	Q_OBJECT
private:
	
	TorrentStorrage* torrents;
	int auto_id;
	QListView* parrent;
	int selectedRow;
	QMenu* menu;
	QAction* openDir;
	QAction* superSeed;
	QAction* DTmount;
	QAction* HashRecheck;
	QAction* DelAll;
	QAction* DelTorrentOnly;
	QAction* setSequentual;
	QAction* updateTrackers;
	QAction* MoveStorrage;
	QAction* PlayInPlayer;
	QAction* GenerateMagnet;
	TorrentManager* mgr;
    Torrent* CurrentTorrent;
	QTimer* timer;
	QMutex* locker;
private:
	void setupContextMenu();
public:
	QTorrentDisplayModel(QListView* parrent,QObject* __parrent);
	~QTorrentDisplayModel();
	enum action{stop,pause,resume,remove,removeAll};
	void ActionOnSelectedItem(action wtf);
	void ChangeData(int row);
	QTorrentDisplayModel(QObject* parrent=NULL);
	int hasTorrent(const QString& InfoHash) const;
	void clear();
	void retranslate();
	void sort();
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    bool removeRow(int row,bool delFiles);
    virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    enum Role { TorrentRole = Qt::UserRole };
	Torrent* GetSelectedTorrent();
	
signals:
	void initCompleted();
	void updateTabSender(int);
	void TorrentCompletedProxySender(const QString);
	void TorrentErrorPoxySender(const QString&);
public slots:
	void AddTorrent(Torrent*);
    void onTorrentRemove(QString);
	void UpdateSelectedIndex(const QItemSelection &);
	void TorrentErrorProxy(const QString&);
	void TorrentCompletedProxy(const QString);
	void contextualMenu(const QPoint &);
	void OpenDirSelected();
	void DellTorrentOnly();
	void Rehash();
	void UpdateTrackers();
	void DellAll();
	void MountDT();
	void playInPlayer();
	void setSequentualDL();
	void moveStorrage();
	void updateVisibleTorrents();
	void SetSuperSeed();
	void initSessionFinished();
	void generateMagnetLink();

	
};


#endif
