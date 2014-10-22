#include "FileViewModel.h"
#include "CuteTorrent.h"
void FileViewModel::retranslateUI()
{
    headerStringsData.clear();
    headerStringsData << trUtf8("FILES_PATH") << trUtf8("FILES_SIZE") << trUtf8("FILES_READY") <<trUtf8("FILES_PRIORITY");
}

FileViewModel::FileViewModel(QObject* parent) : QAbstractTableModel(parent)
{
	headerStringsData << trUtf8("FILES_PATH") << trUtf8("FILES_SIZE") << trUtf8("FILES_READY") <<trUtf8("FILES_PRIORITY");
}

FileViewModel::~FileViewModel(void)
{
}

int FileViewModel::rowCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    Q_UNUSED(parent)
    return dataSource.storrage.is_valid() ? dataSource.storrage.num_files() : 0;
}

int FileViewModel::columnCount( const QModelIndex &parent /*= QModelIndex( ) */ ) const
{
    Q_UNUSED(parent)
    return COLUMN_COUNT;
}

QVariant FileViewModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole */ ) const
{
    int column = index.column();
	int row = index.row();
	if (role == Qt::DisplayRole && dataSource.storrage.is_valid() )
	{
		
		
		file_entry file = dataSource.storrage.at(row);
		
		switch (column)
		{
			case 0:
				return QString::fromStdString(file.path);
			case 1:
				return file.size;
			case 2:
				return dataSource.progresses[row]*100.0f;
			case 3:
				return dataSource.priorities[row];
		}
	}
    if (role==Qt::DecorationRole && column==0)
	{
		int row = index.row();
		file_entry file = dataSource.storrage.at(row);
		QString pathCur = QString::fromStdString(file.path);

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
		switch(role)
		{
		case Qt::DisplayRole:
			return headerStringsData.at(section);
		}

	 }
	return QVariant();
}

bool FileViewModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role /*= Qt::EditRole*/ )
{
    headerStringsData[section]=value.toString();
	emit headerDataChanged(orientation,section,section);
	return true;
}

void FileViewModel::setDataSource(const files_info& storage )
{
	
	if(dataSource.storrage.is_valid())
	{	
		if (dataSource.infoHash != storage.infoHash)
		{
			dataSource = storage;
			reset();
		} else {
			dataSource = storage;
			QModelIndex topleft=index(0,0),botright=index(rowCount()-1,columnCount()-1);
			emit dataChanged(topleft,botright);
		}
	} else {
		dataSource = storage;
		reset();
	}
}



void FileViewModel::Update()
{
	QModelIndex topleft=index(0,0),botright=index(rowCount()-1,columnCount()-1);
	emit dataChanged(topleft,botright);
}

Qt::ItemFlags FileViewModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QPixmapCache FileViewModel::iconCache;




