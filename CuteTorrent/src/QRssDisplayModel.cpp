
#include "QRssDisplayModel.h"

int QRssDisplayModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 1;
}

QVariant QRssDisplayModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	throw std::logic_error("The method or operation is not implemented.");
}

QModelIndex QRssDisplayModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
	throw std::logic_error("The method or operation is not implemented.");
}

int QRssDisplayModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	throw std::logic_error("The method or operation is not implemented.");
}

