#include "FileViewModel.h"
#include "CuteTorrent.h"

void FileViewModel::retranslateUI()
{
	headerStringsData.clear();
	headerStringsData << trUtf8("FILES_PATH") << trUtf8("FILES_SIZE") << trUtf8("FILES_READY") << trUtf8("FILES_PRIORITY");
	openFile->setText(tr("FILETAB_OPEN_FILE"));
	openDir->setText(tr("FILETAB_OPEN_FOLDER"));
	priority->setTitle(tr("FILETAB_PRIORITY"));
	lowPriority->setText(tr("FILETAB_PRIORITY_LOW"));
	mediumPriority->setText(tr("FILETAB_PRIORITY_MEDIUM"));
	highPriority->setText(tr("FILETAB_PRIORITY_HIGH"));
	dontDownload->setText(tr("FILETAB_PRIORITY_ZERO"));
}

FileViewModel::FileViewModel(QTreeView* view, QObject* parent /* = NULL */) : QAbstractItemModel(parent), m_pView(view)
{
	headerStringsData << trUtf8("FILES_PATH") << trUtf8("FILES_SIZE") << trUtf8("FILES_READY") << trUtf8("FILES_PRIORITY");
	m_pRoot = new FileViewTreeItem();
	m_pProxyModel = static_cast<FileViewSortProxyModel*>(m_pView->model());
	setupFileTabelContextMenu();
}

FileViewModel::~FileViewModel(void)
{
	delete m_pRoot;
}

int FileViewModel::rowCount(const QModelIndex& parent /*= QModelIndex( ) */) const
{
	FileViewTreeItem* parentItem;

	if(parent.column() > 0)
	{
		return 0;
	}

	if(!parent.isValid())
	{
		parentItem = m_pRoot;
	}
	else
	{
		parentItem = static_cast<FileViewTreeItem*>(parent.internalPointer());
	}

	return parentItem->GetChildrenCount();
}

int FileViewModel::columnCount(const QModelIndex& parent /*= QModelIndex( ) */) const
{
	Q_UNUSED(parent)
	return COLUMN_COUNT;
}

void FileViewModel::OpenFileSelected()
{
	FileViewTreeItem* pItem = static_cast<FileViewTreeItem*>(m_pProxyModel->mapToSource(m_pView->selectionModel()->currentIndex()).internalPointer());
	std::string save_path = dataSource.status(torrent_handle::query_save_path).save_path;
	QString path = QFileInfo(QDir::toNativeSeparators(QString::fromStdString(save_path + pItem->GetFileEntery().path))).absoluteFilePath();
	QDesktopServices::openUrl(QUrl("file:///" + path));
}

void FileViewModel::OpenDirSelected()
{
	FileViewTreeItem* pItem = static_cast<FileViewTreeItem*>(m_pProxyModel->mapToSource(m_pView->selectionModel()->currentIndex()).internalPointer());
	std::string save_path = dataSource.status(torrent_handle::query_save_path).save_path;
	QString path = QFileInfo(QDir::toNativeSeparators(QString::fromStdString(save_path + pItem->GetFileEntery().path))).absoluteFilePath();
#ifdef Q_WS_MAC
	QStringList args;
	args << "-e";
	args << "tell application \"Finder\"";
	args << "-e";
	args << "activate";
	args << "-e";
	args << "select POSIX file \"" + path + "\"";
	args << "-e";
	args << "end tell";
	QProcess::startDetached("osascript", args);
#endif
#ifdef Q_WS_WIN
	QStringList args;
	args << "/select," << QDir::toNativeSeparators(path);
	QProcess::startDetached("explorer", args);
#endif
}

void FileViewModel::SetLowForCurrentFile()
{
	setFilePriority(2);
}

void FileViewModel::SetMediumForCurrentFile()
{
	setFilePriority(5);
}

void FileViewModel::SetHighForCurrentFile()
{
	setFilePriority(7);
}

void FileViewModel::SetNotDownloadForCurrentFile()
{
	setFilePriority(0);
}


void FileViewModel::setFilePriority(int priorityToSet)
{
	//qDebug() << " CuteTorrent::setHighForCurrentFile";
	QModelIndexList selection = m_pView->selectionModel()->selectedIndexes();

	for each(QModelIndex index in selection)
	{
		QModelIndex sourceIndex = m_pProxyModel->mapToSource(index);
		FileViewTreeItem* item = static_cast<FileViewTreeItem*>(sourceIndex.internalPointer());
		SetItemPriority(item, priorityToSet, sourceIndex);
	}
}


void FileViewModel::setupFileTabelContextMenu()
{
	fileTabMenu = new QMenu(m_pView);
	openFile = new QAction(tr("FILETAB_OPEN_FILE"), fileTabMenu);
	openFile->setObjectName("ACTION_FILETAB_OPEN_FILE");
	QObject::connect(openFile, SIGNAL(triggered()), this, SLOT(OpenFileSelected()));
	fileTabMenu->addAction(openFile);
	openDir = new QAction(tr("FILETAB_OPEN_FOLDER"), this);
	openDir->setObjectName("ACTION_FILETAB_OPEN_DIR");
	QObject::connect(openDir, SIGNAL(triggered()), this, SLOT(OpenDirSelected()));
	fileTabMenu->addAction(openDir);
	fileTabMenu->addSeparator();
	QActionGroup* priorityGroup = new QActionGroup(fileTabMenu);
	priority = new QMenu(fileTabMenu);
	priority->setTitle(tr("FILETAB_PRIORITY"));
	lowPriority = new QAction(tr("FILETAB_PRIORITY_LOW"), fileTabMenu);
	lowPriority->setObjectName("ACTION_FILETAB_LOW_PRIORITY");
	lowPriority->setCheckable(true);
	priorityGroup->addAction(lowPriority);
	QObject::connect(lowPriority, SIGNAL(triggered()), this, SLOT(SetLowForCurrentFile()));
	priority->addAction(lowPriority);
	mediumPriority = new QAction(tr("FILETAB_PRIORITY_MEDIUM"), fileTabMenu);
	mediumPriority->setObjectName("ACTION_FILETAB_MEDIUM_PRIORITY");
	mediumPriority->setCheckable(true);
	priorityGroup->addAction(mediumPriority);
	QObject::connect(mediumPriority, SIGNAL(triggered()), this, SLOT(SetMediumForCurrentFile()));
	priority->addAction(mediumPriority);
	highPriority = new QAction(tr("FILETAB_PRIORITY_HIGH"), fileTabMenu);
	highPriority->setObjectName("ACTION_FILETAB_HIGH_PRIORITY");
	highPriority->setCheckable(true);
	priorityGroup->addAction(highPriority);
	QObject::connect(highPriority, SIGNAL(triggered()), this, SLOT(SetHighForCurrentFile()));
	priority->addAction(highPriority);
	fileTabMenu->addMenu(priority);
	fileTabMenu->addSeparator();
	dontDownload = new QAction(tr("FILETAB_PRIORITY_ZERO"), fileTabMenu);
	dontDownload->setObjectName("ACTION_FILETAB_ZERO_PRIORITY");
	dontDownload->setCheckable(true);
	priorityGroup->addAction(dontDownload);
	QObject::connect(dontDownload, SIGNAL(triggered()), this, SLOT(SetNotDownloadForCurrentFile()));
	fileTabMenu->addAction(dontDownload);
}
void FileViewModel::FileTabContextMenu(const QPoint& point)
{
	QModelIndex qmi = m_pView->indexAt(point);
	qmi = m_pProxyModel->mapToSource(qmi);
	bool isPriorityItemsEnabled = !dataSource.status().is_seeding;
	priority->setEnabled(isPriorityItemsEnabled);
	dontDownload->setEnabled(isPriorityItemsEnabled);

	if(qmi.isValid())
	{
		FileViewTreeItem* item = static_cast<FileViewTreeItem*>(qmi.internalPointer());
		openFile->setEnabled(item->GetType() == FileViewTreeItem::FILE);
		int currentPriority = data(index(qmi.row(), 3, qmi.parent())).toInt();

		//qDebug() << qmi;
		//qDebug() << qmi.data();
		switch(currentPriority)
		{
			case 1:
			case 2:
				lowPriority->setChecked(true);
				break;

			case 3:
			case 4:
			case 5:
				mediumPriority->setChecked(true);
				break;

			case 6:
			case 7:
				highPriority->setChecked(true);
				break;

			case 0:
				dontDownload->setChecked(true);
				break;
		}

		fileTabMenu->exec(m_pView->mapToGlobal(point));
	}
	else
	{
		m_pView->selectionModel()->reset();
	}
}
QVariant FileViewModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole */) const
{
	int column = index.column();
	FileViewTreeItem* item = NULL;

	if(index.isValid())
	{
		item = static_cast<FileViewTreeItem*>(index.internalPointer());
	}

	if(role == Qt::DisplayRole && dataSource.is_valid() && item != NULL)
	{
		file_entry file = item->GetFileEntery();
		int storrage_index = 0;

		if(item->GetType() == FileViewTreeItem::FILE)
		{
			storrage_index = dataSource.torrent_file()->files().file_index_at_offset(file.offset);
		}

		switch(column)
		{
			case 0:
				return item->GetName();

			case 1:
				if(item->GetType() == FileViewTreeItem::FILE)
				{
					return file.size;
				}
				else
				{
					return CalculateFolderSize(item);
				}

			case 2:
				if(item->GetType() == FileViewTreeItem::FILE)
				{
					return m_Progresses[storrage_index] * 100.0f / file.size;
				}
				else
				{
					return 0;
				}

			case 3:
				if(item->GetType() == FileViewTreeItem::FILE && !dataSource.status().is_seeding)
				{
					return dataSource.file_priority(storrage_index);
				}
				else
				{
					return -1;
				}
		}
	}

	if(role == Qt::DecorationRole && column == 0 && item != NULL)
	{
		QString pathCur = item->GetName();
		QIcon icon;
		QFileInfo info(pathCur);
		QFileIconProvider iPorv;

		if(item->GetChildrenCount() == 0)
		{
			QPixmap pixmap;

			if(!iconCache.find(info.suffix(), &pixmap))
			{
				QTemporaryFile tmpfile("tempFileXXXXXX." + info.suffix());
				tmpfile.open();
				tmpfile.close();
				QFileInfo info2(tmpfile.fileName());
				icon = iPorv.icon(info2);
				tmpfile.remove();
				iconCache.insert(info.suffix(), icon.pixmap(QSize(64, 64)));
			}
			else
			{
				icon = QIcon(pixmap);
			}
		}
		else
		{
			icon = iPorv.icon(QFileIconProvider::Folder);
		}

		return icon;
	}

	return QVariant();
}

QVariant FileViewModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		return headerStringsData.at(section);
	}

	return QVariant();
}

bool FileViewModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role /*= Qt::EditRole*/)
{
	headerStringsData[section] = value.toString();
	emit headerDataChanged(orientation, section, section);
	return true;
}

bool FileViewModel::setDataSource(const torrent_handle& hTorrent)
{
	if(hTorrent.is_valid())
	{
		if(dataSource.info_hash() != hTorrent.info_hash())
		{
			delete m_pRoot;
			m_pRoot = new FileViewTreeItem();
			dataSource = hTorrent;
			BuildTree();
			reset();
			return true;
		}
		else
		{
			Update();
		}
	}
	else
	{
		dataSource = torrent_handle();
		delete m_pRoot;
		m_pRoot = new FileViewTreeItem();
		reset();
		return true;
	}

	return false;
}



void FileViewModel::Update()
{
	m_Progresses.clear();
	dataSource.file_progress(m_Progresses, torrent_handle::piece_granularity);
	QModelIndex topleft = index(0, 0), botright = index(rowCount() - 1, columnCount() - 1);
	emit dataChanged(topleft, botright);
}

Qt::ItemFlags FileViewModel::flags(const QModelIndex& index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void FileViewModel::BuildTree()
{
	m_Progresses.clear();
	dataSource.file_progress(m_Progresses, torrent_handle::piece_granularity);
	file_storage storrage = dataSource.torrent_file()->files();
	int nFilesCount = storrage.num_files();

	for(int i = 0; i < nFilesCount; i++)
	{
		if(!storrage.at(i).pad_file)
		{
			AddPath(storrage.file_path(i), storrage.at(i));
		}
	}
}

QModelIndex FileViewModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
	if(!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}

	FileViewTreeItem* parentItem;

	if(!parent.isValid())
	{
		parentItem = m_pRoot;
	}
	else
	{
		parentItem = static_cast<FileViewTreeItem*>(parent.internalPointer());
	}

	FileViewTreeItem* childItem = parentItem->GetNthChild(row);

	if(childItem != NULL)
	{
		return createIndex(row, column, childItem);
	}
	else
	{
		return QModelIndex();
	}
}

QModelIndex FileViewModel::parent(const QModelIndex& child) const
{
	if(!child.isValid())
	{
		return QModelIndex();
	}

	FileViewTreeItem* childItem = static_cast<FileViewTreeItem*>(child.internalPointer());
	FileViewTreeItem* parentItem = childItem->GetParent();

	if(parentItem == m_pRoot)
	{
		return QModelIndex();
	}

	int row = GetRow(parentItem);
	return createIndex(row, 0, parentItem);
}

int FileViewModel::GetRow(FileViewTreeItem* item) const
{
	FileViewTreeItem* parent = item->GetParent();

	if(parent != NULL)
	{
		return parent->m_pChildren.indexOf(item);
	}

	return 0;
}

void FileViewModel::AddPath(std::string path, file_entry fe)
{
	QString qpath = QDir::toNativeSeparators(QString::fromStdString(path));
	QStringList pathparts = qpath.split(QDir::separator());
	int nPartsCount = pathparts.count();
	FileViewTreeItem* iterator = m_pRoot;
	file_entry fake_entery;

	if(m_pRoot->GetChildrenCount() == 0)
	{
		qDebug() << "root item has no children appending current path";
		FileViewTreeItem* curitem = m_pRoot;

		for(int i = 0; i < nPartsCount; i++)
		{
			if(i != nPartsCount - 1)
			{
				fake_entery.path = (QString::fromStdString(fake_entery.path) + QDir::separator() + pathparts[i]).toStdString();
				curitem->AddChild(new FileViewTreeItem(fake_entery, FileViewTreeItem::FOLDER, pathparts[i], curitem));
				curitem = curitem->GetNthChild(0);
			}
			else
			{
				QDir tempPath(QString::fromStdString(fake_entery.path));
				tempPath.cd(pathparts[i]);
				fake_entery.path = tempPath.path().toStdString();
				curitem->AddChild(new FileViewTreeItem(fe, FileViewTreeItem::FILE, pathparts[i], curitem));
				curitem = curitem->GetNthChild(0);
			}
		}

		return;
	}

	for(int i = 0; i < pathparts.count(); i++)
	{
		int foundnum = -1;
		int nChildrenCounmt = iterator->GetChildrenCount();

		for(int j = 0; j < nChildrenCounmt; j++)
		{
			//qDebug() << "Searching for " << pathparts.at(i) << "Current item is " <<iterator->GetNthChild(j)->GetName();
			if(iterator->GetNthChild(j)->GetName().compare(pathparts.at(i)) == 0)
			{
				//qDebug() <<"Found :" << iterator->child(j)->data(0) << " " << pathparts.at(i);
				foundnum = j;
				break;
			}
		}

		if(foundnum >= 0)
		{
			iterator = iterator->GetNthChild(foundnum);
		}
		else
		{
			if(i != nPartsCount - 1)
			{
				//qDebug() << "appending new child" << pathparts.at(i) << " to " << iterator->GetName();
				fake_entery.path = (QString::fromStdString(iterator->GetFileEntery().path) + QDir::separator() + pathparts[i]).toStdString();
				iterator->AddChild(new FileViewTreeItem(fake_entery, FileViewTreeItem::FOLDER, pathparts[i], iterator));
				iterator = iterator->GetNthChild(iterator->GetChildrenCount() - 1);
			}
			else
			{
				//qDebug() << "appending new child" << pathparts.at(i) << " to " << iterator->GetName();
				iterator->AddChild(new FileViewTreeItem(fe, FileViewTreeItem::FILE, pathparts[i], iterator));
				iterator = iterator->GetNthChild(iterator->GetChildrenCount() - 1);
			}
		}
	}
}

long long  FileViewModel::CalculateFolderSize(FileViewTreeItem* item) const
{
	long long result = 0;
	int nChildrenCount = item->GetChildrenCount();

	for(int i = 0; i < nChildrenCount ; i++)
	{
		FileViewTreeItem* child = item->GetNthChild(i);

		if(child->GetType() == FileViewTreeItem::FILE)
		{
			result += child->GetFileEntery().size;
		}
		else
		{
			result += CalculateFolderSize(child);
		}
	}

	return result;
}

float FileViewModel::CalculateFolderReady(FileViewTreeItem* item) const
{
	float result = 0.0f;
	int nChildrenCount = item->GetChildrenCount();

	for(int i = 0; i < nChildrenCount; i++)
	{
		FileViewTreeItem* child = item->GetNthChild(i);

		if(child->GetType() == FileViewTreeItem::FILE)
		{
			file_entry fe = child->GetFileEntery();
			int storrage_index = dataSource.torrent_file()->files().file_index_at_offset(fe.offset);
			result += m_Progresses[storrage_index] * 100.0f / fe.size;
		}
		else
		{
			result += CalculateFolderReady(child);
		}
	}

	return result / nChildrenCount;
}

void FileViewModel::SetItemPriority(FileViewTreeItem* item, int priority, const QModelIndex& sourceIndex)
{
	if(item->GetType() == FileViewTreeItem::FILE)
	{
		int file_index = dataSource.torrent_file()->files().file_index_at_offset(item->GetFileEntery().offset);
		dataSource.file_priority(file_index, priority);
		emit dataChanged(sourceIndex, sourceIndex);
	}
	else
	{
		int nChildrenCount = item->GetChildrenCount();

		for(int i = 0; i < nChildrenCount; i++)
		{
			FileViewTreeItem* child = item->GetNthChild(i);
			SetItemPriority(child, priority, index(i, 0, sourceIndex));
		}
	}
}

QPixmapCache FileViewModel::iconCache;




