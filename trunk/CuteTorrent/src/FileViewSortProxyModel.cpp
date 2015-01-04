#include "FileViewSortProxyModel.h"
#include "FileViewTreeItem.h"

FileViewSortProxyModel::FileViewSortProxyModel(QObject* parrent /* = NULL */) : QSortFilterProxyModel(parrent)
{}


FileViewSortProxyModel::~FileViewSortProxyModel()
{}

bool FileViewSortProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    FileViewTreeItem* rightItem = static_cast<FileViewTreeItem*>(right.internalPointer());
    FileViewTreeItem* lefttItem = static_cast<FileViewTreeItem*>(left.internalPointer());

    if(rightItem->GetType() != lefttItem->GetType())
    {
        if(rightItem->GetType() == FileViewTreeItem::FOLDER)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int sortCol = sortColumn();

    // name
    if(sortCol == 0)
    {
        return rightItem->GetName().compare(lefttItem->GetName()) < 0;
    }
    //file size
    else if(sortCol == 1)
    {
        libtorrent::size_type szLeft = sourceModel()->data(left).toLongLong();
        libtorrent::size_type szRight = sourceModel()->data(right).toLongLong();
        return szLeft < szRight;
    }
    else if(sortCol == 2)
    {
        float leftReady = sourceModel()->data(left).toFloat();
        float rightReady = sourceModel()->data(right).toFloat();
        return leftReady < rightReady;
    }
    else if(sortCol == 3)
    {
        int leftPriority = sourceModel()->data(left).toInt();
        int rightPriority = sourceModel()->data(right).toInt();
        return leftPriority < rightPriority;
    }

    return true;
}
