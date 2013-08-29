#pragma once
#include <QAbstractTableModel>
#include <QVariant>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include "TorrentManager.h"
#include "defs.h"
#include <QPixmapCache>
class FileViewModel : public QAbstractTableModel
{
	Q_OBJECT
private:
	const static int COLUMN_COUNT = 4;
	QList<QString> headerStringsData;
    files_info dataSource;


public:
	FileViewModel(QObject*);
	~FileViewModel(void);
	int rowCount( const QModelIndex &parent = QModelIndex( ) ) const;
	int columnCount( const QModelIndex &parent = QModelIndex( ) ) const;
	QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    void setDataSource(files_info storage);
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
		int role = Qt::EditRole);
	void Update();
    void retranslateUI();
private:
	static QPixmapCache iconCache;

};
