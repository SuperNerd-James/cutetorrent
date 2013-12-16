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
#include "QTorrentDisplayModel.h"
#include <QClipboard>
#include "QApplicationSettings.h"
#include <QProcess>
#include <QItemSelectionModel>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include <QThread>
#include "MultipleDTDialog.h"
#include "VideoPlayer/VideoPlayerWindow.h"
#include "DT_mounter.h"
#include "CuteTorrent.h"
#include "StyleEngene.h"
QTorrentDisplayModel::QTorrentDisplayModel(CuteTorrent* _mainWindow,QListView* _parrent,QObject* __parrent):QAbstractListModel(__parrent)
{
    mainWindow = _mainWindow;
	parrent=_parrent;
	mgr = TorrentManager::getInstance();
	torrents = TorrentStorrage::getInstance();
    selectedRow=-1;
	setupContextMenu();
	
	locker = new QMutex();

	timer = new QTimer(this);
    QObject::connect(mgr,SIGNAL(TorrentRemove(QString)),this,SLOT(onTorrentRemove(QString)));
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateVisibleTorrents()));
	timer->start(1000);

}
void QTorrentDisplayModel::Rehash()
{
	ActionOnSelectedItem(rehash);
}
void QTorrentDisplayModel::DellTorrentOnly()
{
	ActionOnSelectedItem(remove);
}

void QTorrentDisplayModel::DellAll()
{
	
	ActionOnSelectedItem(remove_all);
}
class MountDialogThread : private QThread
{
	MultipleDTDialog *dlg;
public:
	void create(QStringList _files)
	{
		dlg = new MultipleDTDialog(_files);
		start();
	}
protected:
	void run()
	{
		dlg->exec();
		delete dlg;
	}
};

void QTorrentDisplayModel::MountDT()
{

		Torrent* tor=GetSelectedTorrent();
		if (tor!=NULL)
		{
			if (tor->isDaemonToolsMountable() && (tor->isSeeding() || tor->isPaused()))
			{
				if (!tor->isPaused())
							tor->pause();
				QStringList images = tor->GetImageFiles();
				if (images.size() > 1)
				{
					MultipleDTDialog dlg(images,parrent);
					dlg.exec();
				}
				else
				{
					
					DT_mounter::mountImage(images.first());
				}
			}
		}


}
void QTorrentDisplayModel::updateVisibleTorrents()
{
	locker->lock();
	try
	{


	
		mgr->PostTorrentUpdate();
		if (!parrent->isVisible())
		{
			locker->unlock();
			return;
		}
		QModelIndex top(index(0,0)),bot(index(torrents->count(),0));
		emit dataChanged( top,bot );
        emit updateTabSender(-1);
		
	}
	catch (...)
	{

	}
	locker->unlock();
}
void QTorrentDisplayModel::OpenDirSelected()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{
		QString path = QFileInfo(QDir::toNativeSeparators(tor->GetSavePath()+tor->GetName())).absoluteFilePath();
#ifdef Q_WS_MAC
		QStringList args;
		args << "-e";
		args << "tell application \"Finder\"";
		args << "-e";
		args << "activate";
		args << "-e";
		args << "select POSIX file \""+path+"\"";
		args << "-e";
		args << "end tell";
		QProcess::startDetached("osascript", args);
#endif
			
#ifdef Q_WS_WIN
		QStringList args;
		args << "/select," << QDir::toNativeSeparators(path);
		QProcess::startDetached("explorer", args);
#endif

	}
}

void QTorrentDisplayModel::contextualMenu(const QPoint & point)
{
	QModelIndex qmi=parrent->indexAt(point);
	if (qmi.isValid())
	{
		Torrent* torrent=GetSelectedTorrent();
		if (torrent!=NULL)
		{
			
			
            if (!torrent->isDaemonToolsMountable() || !torrent->isSeeding())
			{
                DTmount->setEnabled(false);
			}
			else
            {
                DTmount->setEnabled(true);
            }
			if (!torrent->isSuperSeed())
			{
                superSeed->setChecked(false);
			}
			else
			{
                superSeed->setChecked(true);
			}
			if (!torrent->hasMediaFiles())
			{
                PlayInPlayer->setEnabled(false);
			}
			else
			{
                if (!PlayInPlayer->isEnabled())
				{
                    PlayInPlayer->setEnabled(true);
				}
			}

            QString group = torrent->GetGroup();
            if (!group.isEmpty())
            {
                QList<QAction*> actions = groupsMenu->actions();
                for(int i=0;i<actions.size();i++)
                {
                    actions[i]->setChecked(actions[i]->text()==group);
                }
            }

            setSequentual->setChecked(torrent->isSquential());
			menu->exec(parrent->mapToGlobal(point));
		}
	}
	else
	{
		parrent->selectionModel()->reset();
		selectedRow=-1;
	}
	emit updateTabSender(-1);
}


void QTorrentDisplayModel::UpdateSelectedIndex(const QItemSelection & index)
{
	
	try
	{
		QModelIndexList indexes = index.indexes();
		if (indexes.count()==1)
		{
			
			selectedRow= indexes[0].row();
			CurrentTorrent=torrents->at(selectedRow);
			
		}
		else
		{
			parrent->selectionModel()->reset();
			selectedRow=-1;
			CurrentTorrent=NULL;
		}
		emit updateTabSender(-2);
	}
	catch (std::exception e)
	{
        MyMessageBox::warning(0,"UpdateSelectedIndex",e.what());
	}


	
	
}
/*
void QTorrentDisplayModel::TorrentCompletedProxy(const QString data)
{
	emit TorrentCompletedProxySender(data);
}/*
void QTorrentDisplayModel::AddTorrent(Torrent* tr)
{
	
	if (tr!=NULL)
	{
	//	locker->lock();
		torrents->append(tr);
	//	sort();
	//	locker->unlock();
	}
}*//*
void QTorrentDisplayModel::TorrentErrorProxy(const QString& name)
{
	emit TorrentErrorPoxySender(name);
}/*
void QTorrentDisplayModel::ChangeData(int row)
{
	if (row<rowCount())
	{
		QModelIndex qmi( index( row, 0 ) );
	    emit dataChanged( qmi, qmi );
	}
	
}
int QTorrentDisplayModel::hasTorrent(const QString& InfoHash) const
{
	int i=0;
	for (QList<Torrent*>::const_iterator tor=torrents->begin();
		tor!=torrents->end();
		tor++
		)
	{
		if( (*tor)->GetHashString() == InfoHash )
			return i;
		i++;
	}
	return -1;
}
void QTorrentDisplayModel::clear()
{
	torrents->clear();
	reset( );
}*/
int QTorrentDisplayModel::rowCount( const QModelIndex& parent ) const
{
	try
	{
		return torrents->count();
	}
	catch (std::exception e)
	{
        MyMessageBox::warning(0,"rowCount",e.what());
	}
	return 0;

}
Torrent* QTorrentDisplayModel::GetSelectedTorrent()
{

	
	try
	{
		if (rowCount() == 0)
		{
	
			return NULL;
		}
		if (selectedRow >= rowCount())
		{
	
			return NULL;
		}
		if (selectedRow < 0)
		{
	
			return NULL;
		}
        return CurrentTorrent;
	}
	catch (std::exception e)
	{
        MyMessageBox::warning(0,"GetSelectedTorrent",e.what());
	}
	
	return NULL;

}

void QTorrentDisplayModel::ActionOnSelectedItem(action wtf)
{
	
	try
	{
		QList<Torrent*> selectedTorrents;
		QList<int> rows;
		QModelIndexList indexes = parrent->selectionModel()->selectedIndexes();
		
		foreach(QModelIndex index,indexes)
		{
			rows.append(index.row());
			selectedTorrents.append(index.data(TorrentRole).value<Torrent*>());
		}
		qSort(rows);
        rows=StaticHelpers::reversed(rows);
		if (rowCount()==0)
				return;
			if (selectedRow < 0)
				return;
			if (selectedRow >= rowCount())
				return;
			switch(wtf)
			{
			case stop:
				{
					foreach(Torrent* torrent,selectedTorrents)
						torrent->stop();
					break;
				}
				
			case pause:
				{
					foreach(Torrent* torrent,selectedTorrents)
						torrent->pause();
					break;
				}
				
			case remove:
				{
                    if  (QMessageBox::Cancel != MyMessageBox::warning(parrent,tr("TORRENT_DELITION"),tr("TORRENT_DELITION_MSG"),QMessageBox::Ok | QMessageBox::Cancel))
					foreach(int row,rows)
					{
						//qDebug() << "removing row " << row;
						removeRow(row,false);
					}
				}
				break;
			case remove_all:
				{
                    if  (QMessageBox::Cancel != MyMessageBox::warning(parrent,tr("TORRENT_DELITION"),tr("TORRENT_ALL_DELITION_MSG"),QMessageBox::Ok | QMessageBox::Cancel))
					foreach(int row,rows)
					{
						//qDebug() << "removing row " << row;
						removeRow(row,true);
					}
					break;
				}
			case resume:
				foreach(Torrent* torrent,selectedTorrents)
					torrent->resume();
				break;
			case rehash:
				{
					foreach(Torrent* torrent,selectedTorrents)
					{
						torrent->announceRehash();
					}
					break;
				}
			case move_storrage:
				{
					QString path = QFileDialog::getExistingDirectory(parrent, tr("DIALOG_OPEN_FOLDER"),
						selectedTorrents[0]->GetSavePath(),
						QFileDialog::ShowDirsOnly
						| QFileDialog::DontResolveSymlinks);
					if (!path.isEmpty())
					{
						foreach(Torrent* torrent,selectedTorrents)
						{
							torrent->MoveStorrage(path+QDir::separator());
						}
					}
					break;	
				}
			case set_sequntial:
				{
					foreach(Torrent* torrent,selectedTorrents)
					{
						torrent->seqensialDownload();
					}
					break;
				}
			case set_superseed:
				{
					foreach(Torrent* torrent,selectedTorrents)
					{
						torrent->SuperSeed();
					}
					break;
				}
			case update_trackers:
				{
					foreach(Torrent* torrent,selectedTorrents)
					{
						torrent->updateTrackers();
					}
					break;
				}
			case generate_magmet:
				{
					QString clipboardData;
					foreach(Torrent* torrent,selectedTorrents)
					{
						clipboardData+=torrent->generateMagnetLink();
						clipboardData+="\n";
					}
                    QApplication::clipboard()->setText(clipboardData);
					break;
				}
			}
	}
	catch (std::exception e)
	{
        MyMessageBox::warning(0,"ActionOnSelectedItem",e.what());
	}


}
QVariant QTorrentDisplayModel::data( const QModelIndex& index, int role ) const
{
	
	QVariant var;
    const int row = index.row( );

    if( row<0 || row>=torrents->count() )
        return QVariant( );

	Torrent* t = torrents->at( row );
	
    switch( role )
    {
        case Qt::DisplayRole:
			var = t->GetName() ;
            break;

        case Qt::DecorationRole:
			var = t->GetMimeTypeIcon( );
            break;

        case TorrentRole:
            var = qVariantFromValue( t );
            break;
	
        default:
            break;
	}
	
    return var;
}

bool QTorrentDisplayModel::removeRow(int row, bool delFiles)
{
    if ((row > torrents->count()) || (row < 0))
        return false;
    if (rowCount()==0)
        return false;
    parrent->selectionModel()->reset();
	locker->lock();
	if (torrents->at(row)==CurrentTorrent)
		CurrentTorrent=NULL;
    torrents->at( row )->RemoveTorrent(delFiles);

	locker->unlock();
    return true;
}

bool QTorrentDisplayModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if ((row+count > torrents->count()) || (row < 0))
        return false;
    if (rowCount()==0)
        return false;
    selectedRow=-1;
    parrent->selectionModel()->reset();
    beginRemoveRows(parent,row,row+count);
    for  (int i=row;i<row+count;i++)
    {
        torrents->at( i )->RemoveTorrent(mgr);

    }
    endRemoveRows();
    return true;
}

QTorrentDisplayModel::~QTorrentDisplayModel()
{
	timer->stop();
	TorrentManager::freeInstance();
	TorrentStorrage::freeInstance();
}

void QTorrentDisplayModel::retranslate()
{
    openDir->setText(tr("ACTION_OPEN_FOLDER"));
    DTmount->setText(tr("ACTION_DT_MOUNT"));
    DelAll->setText(tr("ACTION_DELETE_ALL"));
    DelTorrentOnly->setText(tr("ACTION_DELETE_TORRENT"));
    HashRecheck->setText(tr("ACTION_REHASH"));
    setSequentual->setText(tr("ACTION_SET_SEQUENTIAL"));
    updateTrackers->setText(tr("ACTION_UPDATE_TRACKERS"));
    MoveStorrage->setText(tr("ACTION_MOVE_STORRAGE"));
    PlayInPlayer->setText(tr("ACTION_PLAY_IN_PLAYER"));
    superSeed->setText(tr("ACTION_SET_SUPERSEED"));
    GenerateMagnet->setText(tr("ACTION_GENERATE_MAGNET"));
    groupsMenu->setTitle(tr("ACTION_CHANGE_GROUP"));
}

void QTorrentDisplayModel::setSequentualDL()
{
	ActionOnSelectedItem(set_sequntial);
}


void QTorrentDisplayModel::UpdateTrackers()
{
	ActionOnSelectedItem(update_trackers);
}

void QTorrentDisplayModel::moveStorrage()
{
	
	ActionOnSelectedItem(move_storrage);
}

void QTorrentDisplayModel::playInPlayer()
{
	try 
	{
        VideoPlayerWindow* vpw = new VideoPlayerWindow(parrent);
		vpw->openFile(CurrentTorrent->GetSavePath()+QString::fromStdString(CurrentTorrent->GetFileDownloadInfo().storrage.at(0).path));
        vpw->show();
	
	}
	catch(...)
	{}
}

void QTorrentDisplayModel::setupContextMenu()
{
	menu = new QMenu(parrent);
    StyleEngene* style = StyleEngene::getInstance();
    openDir = new QAction(style->getIcon("open_folder"),tr("ACTION_OPEN_FOLDER"),this);
    openDir->setObjectName("ACTION_TORRENTLIST_OPEN_DIR");
    QObject::connect(openDir, SIGNAL(triggered()), this, SLOT(OpenDirSelected()));
    openDir->setShortcut(Qt::Key_Enter);
    menu->addAction(openDir);
    menu->addSeparator();
    DTmount = new QAction(style->getIcon("daemon_tools"),tr("ACTION_DT_MOUNT"), this);
    DTmount->setObjectName("ACTION_TORRENTLIST_DT_MOUNT");
    QObject::connect(DTmount, SIGNAL(triggered()), this, SLOT(MountDT()));
    menu->addAction(DTmount);
    PlayInPlayer = new QAction(style->getIcon("play"),tr("ACTION_PLAY_IN_PLAYER"), this);
    PlayInPlayer->setObjectName("ACTION_TORRENTLIST_PLAY");
    QObject::connect(PlayInPlayer, SIGNAL(triggered()), this, SLOT(playInPlayer()));
    menu->addAction(PlayInPlayer);
    MoveStorrage = new QAction(style->getIcon("move_folder"),tr("ACTION_MOVE_STORRAGE"), this);
    MoveStorrage->setObjectName("ACTION_TORRENTLIST_MOVE_STORRAGE");
    QObject::connect(MoveStorrage, SIGNAL(triggered()), this, SLOT(moveStorrage()));
    menu->addAction(MoveStorrage);
    menu->addSeparator();
    superSeed = new QAction(style->getIcon("super_seed"),tr("ACTION_SET_SUPERSEED"),this);
    superSeed->setObjectName("ACTION_TORRENTLIST_SUPER_SEED");
    superSeed->setCheckable(true);
    QObject::connect(superSeed, SIGNAL(triggered()), this, SLOT(SetSuperSeed()));
    menu->addAction(superSeed);
    HashRecheck = new QAction(style->getIcon("recheck"),tr("ACTION_REHASH"), this);
    HashRecheck->setObjectName("ACTION_TORRENTLIST_RECHECK");
    QObject::connect(HashRecheck, SIGNAL(triggered()), this, SLOT(Rehash()));
    menu->addAction(HashRecheck);
    updateTrackers = new QAction(style->getIcon("update_trackers"),tr("ACTION_UPDATE_TRACKERS"), this);
    updateTrackers->setObjectName("ACTION_TORRENTLIST_UPDATE_TRACKERS");
    QObject::connect(updateTrackers, SIGNAL(triggered()), this, SLOT(UpdateTrackers()));
    menu->addAction(updateTrackers);
    setSequentual = new QAction(style->getIcon("sequential"),tr("ACTION_SET_SEQUENTIAL"), this);
    setSequentual->setObjectName("ACTION_TORRENTLIST_SET_SEQUNTIAL");
    setSequentual->setCheckable(true);
    QObject::connect(setSequentual, SIGNAL(triggered()), this, SLOT(setSequentualDL()));
    menu->addAction(setSequentual);
    GenerateMagnet = new QAction(style->getIcon("magnet"),tr("ACTION_GENERATE_MAGNET"), this);
    GenerateMagnet->setObjectName("ACTION_TORRENTLIST_GENERATE_MAGNET");
    QObject::connect(GenerateMagnet, SIGNAL(triggered()), this, SLOT(generateMagnetLink()));
    menu->addAction(GenerateMagnet);
    menu->addSeparator();
    DelAll = new QAction(style->getIcon("delete"),tr("ACTION_DELETE_ALL"), this);
    DelAll->setObjectName("ACTION_TORRENTLIST_DEL_ALL");
    QObject::connect(DelAll, SIGNAL(triggered()), this, SLOT(DellAll()));
    DelAll->setShortcut(Qt::Key_Shift|Qt::Key_Delete);
    menu->addAction(DelAll);
    DelTorrentOnly = new QAction(style->getIcon("delete"),tr("ACTION_DELETE_TORRENT"), this);
    DelTorrentOnly->setObjectName("ACTION_TORRENTLIST_DEL_TORRENT");
    QObject::connect(DelTorrentOnly, SIGNAL(triggered()), this, SLOT(DellTorrentOnly()));
    DelTorrentOnly->setShortcut(Qt::Key_Delete);
    menu->addAction(DelTorrentOnly);
    groupsMenu = new QMenu(tr("ACTION_CHANGE_GROUP"),menu);
    groupsMenu->setIcon(style->getIcon("groups"));
	//qDebug() << "QApplicationSettings::getInstance from QTorrentDisplayModel::setupContextMenu";
    QList<GroupForFileFiltering> filters = QApplicationSettings::getInstance()->GetFileFilterGroups();
	//qDebug() << "QApplicationSettings::FreeInstance from QTorrentDisplayModel::setupContextMenu";
    QApplicationSettings::FreeInstance();
    QString type;
    for(int i=0;i<filters.size();i++)
    {
        QAction* changeGroupAction = new QAction(style->guessMimeIcon(filters[i].Extensions().split('|')[0],type),filters[i].Name(),groupsMenu);
        changeGroupAction->setObjectName(filters[i].Name());
        QObject::connect(changeGroupAction, SIGNAL(triggered()), this, SLOT(changeGroup()));
        changeGroupAction->setCheckable(true);
        groupsMenu->addAction(changeGroupAction);
    }
    menu->addSeparator();
    menu->addMenu(groupsMenu);

}
bool LessThan(const Torrent* a,const Torrent* b)
{
	return (*a) < (*b);
}


void QTorrentDisplayModel::SetSuperSeed()
{
	ActionOnSelectedItem(set_superseed);
}

void QTorrentDisplayModel::initSessionFinished()
{
	torrents->sort();
    QTimer::singleShot(3000,this,SIGNAL(initCompleted()));
}



void QTorrentDisplayModel::onTorrentRemove(  QString InfoHash )
{
    torrents->remove(InfoHash);
	reset();
}

void QTorrentDisplayModel::generateMagnetLink()
{
	ActionOnSelectedItem(generate_magmet);
}



void QTorrentDisplayModel::changeGroup()
{
    try{
        QAction* senderAct = (QAction*)sender();
        QString group=senderAct->text();
        QList<QAction*> actions = groupsMenu->actions();
        for(int i=0;i<actions.size();i++)
        {
            actions[i]->setChecked(actions[i]==senderAct);
        }
        QModelIndexList indexes = parrent->selectionModel()->selectedIndexes();
        foreach(QModelIndex index,indexes)
        {
            index.data(TorrentRole).value<Torrent*>()->setGroup(group);
        }
    }
    catch(...)
    {

    }


}



