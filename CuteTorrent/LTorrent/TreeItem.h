#include <QPair>
#include <QVariant>

class FileTreeItem
{
public:
	FileTreeItem(const QPair<QString,QString> & data, FileTreeItem *parent = 0);
	~FileTreeItem();

	void appendChild(FileTreeItem *child);

	FileTreeItem *child(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	FileTreeItem *parent();
	Qt::CheckState Checked();
	QString getPath();
	void setChecked(Qt::CheckState checked);
private:
	Qt::CheckState checkedState;
	QList<FileTreeItem*> childItems;
	QPair<QString,QString> itemData;
	FileTreeItem *parentItem;
};