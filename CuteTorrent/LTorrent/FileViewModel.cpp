#include "FileViewModel.h"
#include "CuteTorrent.h"
FileViewModel::FileViewModel(QObject* parent) : QAbstractTableModel(parent)
{
  //  qDebug() << "FileViewModel::FileViewModel";
	headerStringsData << trUtf8("FILES_PATH") << trUtf8("FILES_SIZE") << trUtf8("FILES_READY") <<trUtf8("FILES_PRIORITY");
}

FileViewModel::~FileViewModel(void)
{
}

int FileViewModel::rowCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
   // qDebug() << "FileViewModel::rowCount" << (dataSource.storrage.is_valid() ? dataSource.storrage.num_files() : 0);
    return dataSource.storrage.is_valid() ? dataSource.storrage.num_files() : 0;
}

int FileViewModel::columnCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
  //  qDebug() << "FileViewModel::columnCount";
	return COLUMN_COUNT;
}

QVariant FileViewModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
  //  qDebug() << "FileViewModel::data" << index.row() << index.column();
	int column = index.column();
	int row = index.row();
	if (role == Qt::DisplayRole && dataSource.storrage.is_valid() )
	{
		
		
		internal_file_entry file = dataSource.storrage.internal_at(row);
		switch (column)
		{
			case 0:
				return QString::fromStdString(dataSource.storrage.file_path(file));
			case 1:
				return dataSource.storrage.file_size(file); 
			case 2:
				return dataSource.progresses[row]*100.0f;
			case 3:
				return dataSource.priorities[row];
		}
	}
	if(role==Qt::UserRole)
	{
		int row = index.row();
		return dataSource.storrage.file_index(row);
	}
	if (role==Qt::DecorationRole && column==0)
	{
		int row = index.row();
		internal_file_entry file = dataSource.storrage.internal_at(row);
		QString pathCur=QString::fromStdString(dataSource.storrage.file_path(file));

		QIcon icon;
		QFileInfo info(pathCur);
		QFileIconProvider iPorv;
		
		if (!info.suffix().isEmpty() )
		{
			QPixmap pixmap;
			if (!iconCache.find(info.suffix(),&pixmap))
			{
				QTemporaryFile tmpfile("tempFileXXXXXX."+info.suffix());
				tmpfile.open();
				tmpfile.close();
				QFileInfo info2(tmpfile.fileName());
				icon= iPorv.icon(info2);
				tmpfile.remove();
				iconCache.insert(info.suffix(),icon.pixmap(QSize(64,64)));
			}
			else
			{
				icon = QIcon(pixmap);
			}
			//qDebug() << info.suffix();
			
		}
		else
		{

			icon=iPorv.icon(QFileIconProvider::Folder);
		}
		return icon;
	}
	return QVariant();
}

QVariant FileViewModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
    
	 if(orientation==Qt::Horizontal)
	 {
	//	qDebug() << "FileViewModel::headerData" << section;
		switch(role)
		{
		case Qt::DisplayRole:
/*			qDebug() << "Qt::DisplayRole";
			qDebug() << section;
			qDebug() << headerStringsData;*/
			return headerStringsData.at(section);
/*		case Qt::SizeHintRole:
/*			qDebug() << "Qt::SizeHintRole";
			qDebug() << section;
			switch(section)
			{
			case 0:
			
			case 1:
			
			case 2:
			
			case 3:
				return QSize(300,25);

			 default:
				return QSize(60,25);

			}*/
		}

		//qDebug() << "not Qt::DisplayRole" << role;
	 }
	return QVariant();
}

bool FileViewModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role /*= Qt::EditRole*/ )
{
  //qDebug() << "FileViewModel::setHeaderData" << section << value;
	headerStringsData[section]=value.toString();
	emit headerDataChanged(orientation,section,section);
	return true;
}

void FileViewModel::setDataSource( files_info storage )
{
	
	if(dataSource.storrage.is_valid())
	{	
	//	qDebug() << "dataSource.storrage.is_valid=true";
		if (dataSource.infoHash != storage.infoHash)
		{
	//		qDebug() << "reseting";
			dataSource = storage;
			reset();
		} else {
	//		qDebug() << "updating data";
			dataSource = storage;
			QModelIndex topleft=index(0,0),botright=index(rowCount()-1,columnCount()-1);
			emit dataChanged(topleft,botright);
		}
	} else {
	//	qDebug() << "dataSource.storrage.is_valid=false";
	//	qDebug() << "reseting";
		dataSource = storage;
		reset();
	}
}



void FileViewModel::Update()
{
	QModelIndex topleft=index(0,0),botright=index(rowCount()-1,columnCount()-1);
	emit dataChanged(topleft,botright);
}

QPixmapCache FileViewModel::iconCache;




