#pragma once
#include <QtGui>
#include "TorrentManager.h"
#include "FileViewTreeItem.h"
#include "FileViewSortProxyModel.h"
#include "defs.h"

class FileViewModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	const static int COLUMN_COUNT = 4;
	QList<QString> headerStringsData;
    torrent_handle dataSource;


public:
	FileViewModel(QTreeView* view, QObject* parrent = NULL);
	~FileViewModel(void);
	int rowCount( const QModelIndex &parent = QModelIndex( ) ) const;
	int columnCount( const QModelIndex &parent = QModelIndex( ) ) const;
	QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
	bool setDataSource(const torrent_handle& storage);
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
	bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value,
		int role = Qt::EditRole);
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	void Update();
    void retranslateUI();

	virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
	void BuildTree();
	int GetRow(FileViewTreeItem * parentItem) const;
	void AddPath(std::string path, file_entry fe);
public slots:
	void FileTabContextMenu(const QPoint & point);
	void OpenDirSelected();
	void OpenFileSelected();
	void SetLowForCurrentFile();
	void SetMediumForCurrentFile();
	void SetHighForCurrentFile();
	void SetNotDownloadForCurrentFile();
private:
	long long  CalculateFolderSize(FileViewTreeItem* item) const;
	float CalculateFolderReady(FileViewTreeItem* item) const;
	void setupFileTabelContextMenu();
	
	void setFilePriority(int priorityToSet);
	void SetItemPriority(FileViewTreeItem* item, int priority, const QModelIndex& sourceIndex);
	QMenu* fileTabMenu;
	QAction* openFile;
	QAction* openDir;
	QMenu* priority;
	QAction* lowPriority;
	QAction* mediumPriority;
	QAction* highPriority;
	QAction* dontDownload;
	QTreeView* m_pView;
	FileViewSortProxyModel* m_pProxyModel;
	std::vector<libtorrent::size_type> m_Progresses;
	static QPixmapCache iconCache;
	FileViewTreeItem* m_pRoot;
};
