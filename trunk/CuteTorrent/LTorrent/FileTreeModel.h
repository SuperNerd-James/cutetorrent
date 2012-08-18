#include <QAbstractItemModel>
#include "TreeItem.h"
#include <QCheckBox>
#include <QMap>
class FileTreeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	FileTreeModel( QObject *parent = 0);
	~FileTreeModel();
	bool setData ( const QModelIndex &, const QVariant &, int role = Qt::EditRole );
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
		const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	void addPath(QString path,QString size);
	QMap<QString,int> getFilePiorites();
private:
	void GetFilePrioritiesInternal(FileTreeItem* current,QMap<QString,int>* priorities);

	FileTreeItem *rootItem;
};


