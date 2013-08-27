#include "QTorrentFilterModel.h"
#include "Torrent.h"
#include "QTorrentDisplayModel.h"
#include <QMessageBox>
bool QTorrentFilterModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
	qDebug() << "QTorrentFilterModel::filterAcceptsRow" << source_row << source_parent << currentFilter;
	QModelIndex index = sourceModel()->index(source_row,0);
	Torrent*  tor( index.data( QTorrentDisplayModel::TorrentRole ).value<Torrent*>() );
	if (tor == NULL)
	{
		QMessageBox::warning(NULL,"QTorrentFilterModel::filterAcceptsRow","tor == NULL");
		return false;
	}
	switch(currentFilter)
	{
	case ACTIVE:
		{
			
			if (!tor->isPaused() && tor->GetDwonloadSpeed().isEmpty() && tor->GetUploadSpeed().isEmpty())
			{
				return false;
			}
			return true;
		}
	case DWONLOADING:
		return tor->isDownloading();
	case SEEDING:
		return tor->isSeeding();
	case COMPLETED:
		return tor->GetProgress()==100;
	case NONE:
		//QMessageBox::warning(NULL,"QTorrentFilterModel::filterAcceptsRow","currentFilter = NONE,return true;");
		return true;
	}
	return true;
}

void QTorrentFilterModel::setFilter( FilterType filter )
{
	currentFilter=filter;
	reset();
}

QTorrentFilterModel::QTorrentFilterModel( QObject* parent) : QSortFilterProxyModel(parent)
{
	currentFilter=NONE;
	reset();
	timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
	
}

void QTorrentFilterModel::Update()
{
	reset();
}

void QTorrentFilterModel::updateData()
{
	QModelIndex top(index(0,0)),bot(index(rowCount(),0));
	emit dataChanged( top,bot );
	reset();
}

/*
bool QTorrentFilterModel::filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const
{
	qDebug() << "QTorrentFilterModel::filterAcceptsColumn" << source_parent << source_column;
	return false;
}
*/
