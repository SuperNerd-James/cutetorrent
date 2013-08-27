#pragma once
#include <QSortFilterProxyModel>
#include <QTimer>
class QTorrentFilterModel :
	public QSortFilterProxyModel
{
	Q_OBJECT
public:
	enum FilterType{
		ACTIVE,
		SEEDING,
		DWONLOADING,
		COMPLETED,
		NONE
	};

	QTorrentFilterModel(QObject*);
	void setFilter(FilterType filter);
	void Update();
protected:
	bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const;
	
	

	


private:
	QTimer* timer;
	FilterType currentFilter;
private slots:
	void updateData();
};
