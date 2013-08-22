#pragma once
#include <QSortFilterProxyModel>

class QTorrentFilterModel :
	public QSortFilterProxyModel
{

public:
	enum FilterType{
		ACTIVE,
		SEEDING,
		DWONLOADING,
		NONE
	};

	QTorrentFilterModel(QObject*);
	void setFilter(FilterType filter);
protected:
	bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const;
	
	


private:
	FilterType currentFilter;
};
