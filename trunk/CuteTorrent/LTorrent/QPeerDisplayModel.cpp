#include "QPeerDisplayModel.h"

QPeerDisplayModel::QPeerDisplayModel(void)
{
}

QPeerDisplayModel::~QPeerDisplayModel(void)
{
}

void QPeerDisplayModel::upadtePeerInfo( QVector<peer_info> newPeerInfos )
{
	currentPeerInfos=newPeerInfos;
}

int QPeerDisplayModel::rowCount( const QModelIndex& parent /*= QModelIndex() */ ) const
{
	return currentPeerInfos.count();
}

QVariant QPeerDisplayModel::data( const QModelIndex& index, int role /*= Qt::DisplayRole */ ) const
{
	QVariant var;
	const int row = index.row();
	const int column = index.column();
	if( row<0 || row>=currentPeerInfos.size() )
		return QVariant( );
	switch( role )
	{
		case Qt::DisplayRole:
			switch(column)
			{
				case 0 :
					break;
				case 1 :
					break;
				case 2 :
					break;
				case 3 :
					break;
				case 4 :
					break;
				case 5 :
					break;
				case 6 :
					break;
			}
			break;
		default:
			break;
	}
	return var;

}
