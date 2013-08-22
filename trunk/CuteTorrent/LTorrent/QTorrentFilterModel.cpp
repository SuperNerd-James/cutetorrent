#include "QTorrentFilterModel.h"
#include "Torrent.h"
#include "QTorrentDisplayModel.h"
#include <QMessageBox>
bool QTorrentFilterModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
	QMessageBox::warning(NULL,"QTorrentFilterModel::filterAcceptsRow","filterAcceptsRow");
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
	case NONE:
		QMessageBox::warning(NULL,"QTorrentFilterModel::filterAcceptsRow","currentFilter = NONE,return true;");
		return true;
	}
	return true;
}

void QTorrentFilterModel::setFilter( FilterType filter )
{
	currentFilter=filter;
	invalidateFilter();
}

QTorrentFilterModel::QTorrentFilterModel( QObject* parent) : QSortFilterProxyModel(parent)
{
	/*currentFilter=NONE;
	invalidateFilter();*/
}

