#include "TreeItem.h"

#include <QDebug>
#include <QDir>
FileTreeItem::FileTreeItem(const QPair<QString,QString> &data, FileTreeItem *parent)
{
	parentItem = parent;
	itemData = data;
	checkedState=Qt::Checked;

}
Qt::CheckState FileTreeItem::Checked()
{
	
	
	return checkedState;
}
void FileTreeItem::setChecked(Qt::CheckState _checked)
{
	checkedState=_checked;
	qDebug() << "setCheced " << _checked << " for "  << itemData.first;

	if (_checked!=Qt::PartiallyChecked)
	for (int i=0;i<childItems.count();i++)
	{
		childItems.at(i)->setChecked(_checked);
	}
	

}
FileTreeItem::~FileTreeItem()
{
	qDeleteAll(childItems);
}
void FileTreeItem::appendChild(FileTreeItem *item)
{
	childItems.append(item);
}

QString FileTreeItem::getPath()
{
	QString res = data(0).toString();
	FileTreeItem* parrent = parent();
	while (parrent!=NULL && parrent->parent()!=NULL)
	{
		res = parrent->data(0).toString()+QDir::separator()+res;
		parrent = parrent->parent();
	}
	return res;
}
FileTreeItem *FileTreeItem::child(int row)
{
	return childItems.value(row);
}

int FileTreeItem::childCount() const
{
	return childItems.count();
}
int FileTreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<FileTreeItem*>(this));

	return 0;
}

int FileTreeItem::columnCount() const
{
	return 2;
}
QVariant FileTreeItem::data(int column) const
{
	if (column > 2)
		return QVariant();
	if (column==0)
		return itemData.first;
	else
		return itemData.second;
}

FileTreeItem *FileTreeItem::parent()
{
	return parentItem;
}
