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
#include "QApplicationSettings.h"
#include <QProcess>
#include <QItemSelectionModel>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QDebug>
#include "MultipleDTDialog.h"
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
	locker = new QMutex(QMutex::NonRecursive);

	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateVisibleTorrents()));
	timer->start(1000);

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
	
	if (rowCount() == 0)
	{
	
		return ;
	}
	if (selectedRow >= rowCount())
	{
	
		return ;
	}
	if (selectedRow < 0)
	{
	
		return ;
	}
	qDebug() << "receving " << selectedRow <<" torrent";
	qDebug() << "torrents count " << torrents.count();
	Torrent* tor=torrents.at(selectedRow);
	if (tor!=NULL)
	{
		tor->pause();	
		QString path=tor->GetSavePath();
		qDebug() << "Save path is " << path;
		qDebug() << "removing from gui";
		qDebug() << "Locking mutex at DellAll";
		locker->lock();
		torrents_to_remove.append(tor);
		locker->unlock();
		mgr->PostTorrentUpdate();
		QFileInfo info(path);
		if (info.isDir())
		{

			qDebug() << "it is a dir";
			StaticHelpers::dellDir(path);
		}
		else
		{
			qDebug() << "it is a file";
			QFile::remove(path);
		}
		parrent->selectionModel()->reset();
		
		
	}
	qDebug() << "unlocking mutex at DellAll";


}
void QTorrentDisplayModel::MountDT()
{

		Torrent* tor=GetSelectedTorrent();
		if (tor!=NULL)
		{
			if (tor->isDaemonToolsMountable() && (tor->isSeeding() || tor->isPaused()))
			{
				if (!tor->isPaused())
							tor->pause();
				QStringList* images = tor->GetImageFiles();
				qDebug() << "receved imageFiles first item : " << images->at(0);
				if (images->count() > 1)
				{
					qDebug() << "images.count>1";
					MultipleDTDialog *dlg = new MultipleDTDialog(images);
					qDebug() << "MultipleDTDialog created now will be executed";
					dlg->exec();

					delete dlg;
					
				}
				else
				{
					qDebug() << "going to else brunch"	;
					QApplicationSettings* settings=QApplicationSettings::getInstance();
					QString exe = settings->valueString("DT","Executable");
					if (exe.isEmpty())
					{
						QApplicationSettings::FreeInstance();
						QMessageBox::warning(parrent,"DT Mounter",QString::fromLocal8Bit("Укажите в настройках путь к Daemon Tools!"));
						return;
					}
					bool useCustomCmd = settings->valueBool("DT","UseCustomCommand");
					int driveNum = settings->valueInt("DT","Drive");
					QString command = useCustomCmd ?  settings->valueString("DT","CustomtCommand"): settings->valueString("DT","DefaultCommand"); 
					QProcess *dt = new QProcess(this);
					QStringList args;
					/*args << "-mount";
					args << command.arg(QString::number(driveNum)).arg(images.first());*/
					qDebug() << exe << command.arg(QString::number(driveNum)).arg(images->first());
					dt->setNativeArguments(command.arg(QString::number(driveNum)).arg(images->first()));
					dt->start(exe,args);
					QApplicationSettings::FreeInstance();
					if (!dt->waitForStarted(5000))
					{
						QMessageBox::warning(parrent,"DT Mounter",QString::fromLocal8Bit("Не удалось запустить ")+exe);
						return;
					}
					
					dt->waitForFinished();
					delete dt;
				}
			}
		}


}
void QTorrentDisplayModel::updateVisibleTorrents()
{
	locker->lock();
	for (int i=0;i<torrents_to_remove.count();i++)
	{
		int index=torrents.indexOf(torrents_to_remove.at(i));
		qDebug() << "removing " << index << "torrent of " << torrents.count();
		torrents.at( index )->RemoveTorrent(mgr);
		torrents.erase(torrents.begin() + index);
		int id=id_to_row[index];
		id_to_row.remove(index);
		id_to_torrent.remove(id);
	}
	torrents_to_remove.clear();
	locker->unlock();
	torrents_to_remove.clear();
	for (int i=0;i<torrents.count();i++)
	{
		QModelIndex qmi( index( i, 0 ) );
		emit dataChanged( qmi, qmi );
		emit updateTabSender(-1);
	}
}
void QTorrentDisplayModel::OpenDirSelected()
{
	Torrent* tor=GetSelectedTorrent();
	if (tor!=NULL)
	{
		QString path = QFileInfo(QDir::toNativeSeparators(tor->GetSavePath())).absoluteFilePath();
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
	
	try
	{
		selectedRow= index.row();
	}
	catch (std::exception e)
	{
		QMessageBox::warning(0,"UpdateSelectedIndex",e.what());
	}


	
	
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
		QObject::connect(tr,SIGNAL(TorrentError(const QString)),this,SLOT(TorrentErrorProxy(const QString)));
		torrents.append(tr);
		id_to_torrent.insert(auto_id,tr);
		id_to_row.insert(torrents.count()-1,auto_id);
		auto_id++;
	}
}
void QTorrentDisplayModel::TorrentErrorProxy(const QString& name)
{
	emit TorrentErrorPoxySender(name);
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
	try
	{
		return torrents.count();
	}
	catch (std::exception e)
	{
		QMessageBox::warning(0,"rowCount",e.what());
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
		qDebug() << "giving " << selectedRow << " torrent";
	
		return torrents.at(selectedRow);
	}
	catch (std::exception e)
	{
		QMessageBox::warning(0,"GetSelectedTorrent",e.what());
	}
	
	return NULL;

}
bool QTorrentDisplayModel::updateTorrent(QString InfoHash,torrent_status status)
{
	try
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
	}
	catch (std::exception e)
	{
		QMessageBox::warning(0,"emit updateTorrent",e.what());
	}

	return false;
}
void QTorrentDisplayModel::ActionOnSelectedItem(action wtf)
{
	
	try
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
				//removeRow(oldSelection);
				selectedRow=-1;
				torrents.at( oldSelection )->RemoveTorrent(mgr);
				torrents.erase(torrents.begin() + oldSelection);
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
	catch (std::exception e)
	{
		QMessageBox::warning(0,"ActionOnSelectedItem",e.what());
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
	timer->stop();
	TorrentManager::freeInstance();
}