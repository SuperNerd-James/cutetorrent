#include "QTorrentDisplayModel.h"
#include <QItemSelectionModel>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QDebug>
QTorrentDisplayModel::QTorrentDisplayModel(QListView* _parrent)
{
	parrent=_parrent;
	qDebug() << "QTorrentDisplayModel asking TorrentManager Instance";
	mgr = TorrentManager::getInstance();
	auto_id=0;
	selectedRow=-1;
	menu = new QMenu;
	openDir = new QAction(QString::fromLocal8Bit(tr("Открыть папку").toAscii().data()), this);
	QObject::connect(openDir, SIGNAL(triggered()), this, SLOT(OpenDirSelected()));
	menu->addAction(openDir);
	DTmount = new QAction(QString::fromLocal8Bit(tr("Смонтировать образ в DaemonTools").toAscii().data()), this);
	QObject::connect(DTmount, SIGNAL(triggered()), this, SLOT(MountDT()));
	menu->addAction(DTmount);
	DelAll = new QAction(QString::fromLocal8Bit(tr("Удалить файлы и торрент").toAscii().data()), this);
	QObject::connect(DelAll, SIGNAL(triggered()), this, SLOT(DellAll()));
	menu->addAction(DelAll);
	HashRecheck = new QAction(QString::fromLocal8Bit(tr("Обновить хеш").toAscii().data()), this);
	QObject::connect(HashRecheck, SIGNAL(triggered()), this, SLOT(Rehash()));
	menu->addAction(HashRecheck);

}
void QTorrentDisplayModel::Rehash()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{
		tor->announceRehash();
	}
}
void QTorrentDisplayModel::DellAll()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{
		
		QString path=tor->GetSavePath();
		
		ActionOnSelectedItem(remove);
		
		mgr->PostTorrentUpdate();
		QFileInfo info(path);
		if (info.isDir())
		{
			
			StaticHelpers::dellDir(path);
		}
		else
		{
		
			QFile::remove(path);
		}
		
		delete tor;
	}

}
void QTorrentDisplayModel::MountDT()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{

	}
}
void QTorrentDisplayModel::SetSequentual()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{
		tor->seqensialDownload();
	}
}
void QTorrentDisplayModel::OpenDirSelected()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{
		QString path = QFileInfo(QDir::toNativeSeparators(tor->GetSavePath())).dir().absolutePath();
		QDesktopServices::openUrl(QUrl("file:///" + path));
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
			
			if (!torrent->isDaemonToolsMountable())
			{
				DTmount->setEnabled(false);
			}
			else
			{
				if (!DTmount->isEnabled())
				{
					DTmount->setEnabled(true);
				}
			}
			menu->exec(parrent->mapToGlobal(point));
		}
	}
	else
	{
		
		parrent->selectionModel()->reset();
		selectedRow=-1;
	}
}


void QTorrentDisplayModel::UpdateSelectedIndex(const QModelIndex & index)
{
	
	selectedRow= index.row();
	
	
}

void QTorrentDisplayModel::TorrentCompletedProxy(const QString data)
{
	emit TorrentCompletedProxySender(data);
}
void QTorrentDisplayModel::AddTorrent(Torrent* tr)
{
	
	if (tr!=NULL)
	{
		QObject::connect(tr,SIGNAL(TorrentCompleted(const QString)),this,SLOT(TorrentCompletedProxy(const QString)));
		torrents.append(tr);
		id_to_torrent.insert(auto_id,tr);
		id_to_row.insert(torrents.size(),auto_id);
		auto_id++;
	}
}
void QTorrentDisplayModel::ChangeData(int row)
{
	if (row<rowCount())
	{
		QModelIndex qmi( index( row, 0 ) );
		//torrents.at(row)->update();
        emit dataChanged( qmi, qmi );
	}
	//	reset( );
}
bool QTorrentDisplayModel::hasTorrent(const QString& InfoHash) const
{
	for (QVector<Torrent*>::const_iterator tor=torrents.begin();
		tor!=torrents.end();
		tor++
		)
	{
		
	        if( (*tor)->GetHashString() == InfoHash )
	            return true;
	}
	return false;
}
void QTorrentDisplayModel::clear()
{
	auto_id=0;
	torrents.clear();
	id_to_torrent.clear();
	id_to_row.clear();
	reset( );
}
int QTorrentDisplayModel::rowCount( const QModelIndex& parent ) const
{
	return torrents.count();
}
Torrent* QTorrentDisplayModel::GetSelectedTorrent()
{
	
	if (rowCount() == 0)
		return NULL;
	if (selectedRow >= rowCount())
		return NULL;
	if (selectedRow < 0)
		return NULL;

	return torrents.at(selectedRow);

}
bool QTorrentDisplayModel::updateTorrent(QString InfoHash,torrent_status status)
{
	for (QVector<Torrent*>::const_iterator tor=torrents.begin();
		tor!=torrents.end();
		tor++
		)
	        if( (*tor)->GetHashString() == InfoHash )
			{
				(*tor)->updateTorrent(status);
				return true;
			}
	return false;
}
void QTorrentDisplayModel::ActionOnSelectedItem(action wtf)
{
	
	if (rowCount()==0)
		return;
	if (selectedRow < 0)
		return;
	if (selectedRow >= rowCount())
		return;
	switch(wtf)
	{
	case pause:
		torrents.at( selectedRow )->pause();
		break;
	case remove:
		{
			int oldSelection=selectedRow;
			selectedRow=-1;
			torrents.at( oldSelection )->RemoveTorrent(mgr);
			torrents.remove(oldSelection);
			int id=id_to_row[oldSelection];
			id_to_row.remove(oldSelection);
			
			id_to_torrent.remove(id);
			parrent->selectionModel()->reset();
			
		}
		break;
	case resume:
		torrents.at( selectedRow )->resume();
		break;
	}

}
QVariant QTorrentDisplayModel::data( const QModelIndex& index, int role ) const
{
	QVariant var;
    const int row = index.row( );
    if( row<0 || row>=torrents.size() )
        return QVariant( );

	Torrent* t = torrents.at( row );
	
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

QTorrentDisplayModel::~QTorrentDisplayModel()
{
	TorrentManager::freeInstance();
}