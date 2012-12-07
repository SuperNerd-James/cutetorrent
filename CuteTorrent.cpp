/*
CuteTorrent BitTorrenttClient with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) <year>  <name of author>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "application.h"
#include "CuteTorrent.h"
#include <QMessageBox>
#include <QComboBox>
#include <QDateTime>
#include <QAbstractItemView>
#include "QTorrentListView.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QIcon>
#include "QBaloon.h"
#include <QProcess>
CuteTorrent::CuteTorrent(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	setupUi(this);
	model = new QTorrentDisplayModel(listView,this);
	//qDebug() << "QMainWindow ascked TorrentManager::getInstance";
	mng = TorrentManager::getInstance();
	notyfire = new UpdateNotifier();
	mayShowNotifies = false;
	fileinfosLocker = new QMutex(QMutex::NonRecursive);
	//setAcceptDrops(true);
	setupStatusBar();
	setupTray();
	setupToolBar();
	setupListView();
	setupTabelWidgets();
	setupFileTabel();
	setupConnections();

	QApplicationSettings* settings=QApplicationSettings::getInstance();
	
	Application::setLanguage("cutetorrent_"+settings->valueString("System","Lang","RUSSIAN"));


	 
	
	mng->initSession();

	QTimer::singleShot(10000,this,SLOT(checkForUpdates()));

	
	
}
void CuteTorrent::checkForUpdates()
{
		notyfire->fetch();

}
void CuteTorrent::ShowAbout()
{
	QMessageBox::about(this,tr("ABAUT_TITLE"),tr("ABAUT_TEXT").arg(CT_VERSION));
}
void CuteTorrent::ShowUpdateNitify(const QString& newVersion)
{
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_NEW_VERSION %1").arg(newVersion), icon,
		5* 1000);
}
/*
void CuteTorrent::mousePressEvent(QMouseEvent* event)
{      
	m_nMouseClick_X_Coordinate = event->x();
	m_nMouseClick_Y_Coordinate = event->y();
	QMainWindow::mousePressEvent(event);
};

void CuteTorrent::mouseMoveEvent(QMouseEvent* event)
{
	move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
	QMainWindow::mouseMoveEvent(event);
};*/
void CuteTorrent::setupStatusBar()
{
	
	upLabel = new QLabel(this);
	upLabelText = new QLabel(this);
	upLabelText->setMinimumWidth(140);
	upLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/upload.ico")));
	
	downLabel = new QLabel(this);
	downLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/download.ico")));
	downLabelText = new QLabel(this);
	downLabel->setToolTip(tr("STATUS_DWONLOAD"));
	downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
	upLabel->setToolTip(tr("STATUS_UPLOAD"));
	upLabelText->setToolTip(tr("STATUS_UPLOAD"));
	downLabelText->setMinimumWidth(140);
	statusBar()->addPermanentWidget(upLabel);
	statusBar()->addPermanentWidget(upLabelText);
	statusBar()->addPermanentWidget(downLabel);
	statusBar()->addPermanentWidget(downLabelText);
	
}
void CuteTorrent::setupListView()
{
	
	listView->setModel(model);
	listView->setSelectionMode(QAbstractItemView::SingleSelection );
	listView->setContextMenuPolicy(Qt::CustomContextMenu);
}
void CuteTorrent::setupTabelWidgets()
{
	trackerTableWidget->verticalHeader()->hide();
	trackerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	trackerTableWidget->setColumnWidth(2,120);
	peerTableWidget->verticalHeader()->hide();
	peerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	peerTableWidget->setSortingEnabled(true);

}
void CuteTorrent::setupToolBar()
{
	QWidget* spacer=new QWidget(this);
	listView->setItemDelegate(new QTorrentItemDelegat(this));
	QLineEdit* lEdit=new QLineEdit(this);
	QComboBox *comboBox;
	comboBox = new QComboBox(this);
    comboBox->setObjectName(QString::fromUtf8("searchEngineCombobox"));
	comboBox->clear();
    comboBox->insertItems(0, QStringList()
         << QApplication::translate("LTorrentClass", "rutracker.org", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LTorrentClass", "thepiratebay.se", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LTorrentClass", "rutor.org", 0, QApplication::UnicodeUTF8));
	spacer->setMinimumWidth(200);
	lEdit->setMinimumWidth(100);
	toolBar->addWidget(spacer);
	toolBar->addWidget(comboBox);
	toolBar->addWidget(lEdit);
}
void CuteTorrent::setupConnections()
{
	
	QObject::connect(listView,SIGNAL(clicked(const QModelIndex &)),model,SLOT(UpdateSelectedIndex(const QModelIndex &)));
	QObject::connect(listView,SIGNAL(customContextMenuRequested(const QPoint &)),model,SLOT(contextualMenu(const QPoint &)));
	//QObject::connect(listView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(UpdateInfoTab()));
	//QObject::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	QObject::connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	QObject::connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateTabWidget(int)));
	QObject::connect(model,SIGNAL(updateTabSender(int)),this,SLOT(updateTabWidget(int)));
	QObject::connect(mng,SIGNAL(TorrentError(const QString&,const QString&)),this,SLOT(ShowTorrentError(const QString&,const QString&)));
	QObject::connect(mng,SIGNAL(TorrentCompleted(const QString)),
									this,SLOT(showTorrentCompletedNotyfy(const QString)));
	QObject::connect(mng,SIGNAL(AddTorrentGui(Torrent*)),model,SLOT(AddTorrent(Torrent*)));
	QObject::connect(notyfire,SIGNAL(showUpdateNitify(const QString &)),this,SLOT(ShowUpdateNitify(const QString &)));
	QObject::connect(notyfire,SIGNAL(showNoUpdateNitify(const QString &)),this,SLOT(ShowNoUpdateNitify(const QString &)));
	QObject::connect(fileTableWidget,SIGNAL(customContextMenuRequested ( const QPoint &)),this,SLOT(fileTabContextMenu(const QPoint &)));
}
void CuteTorrent::fileTabContextMenu(const QPoint & point)
{
	QModelIndex qmi=fileTableWidget->indexAt(point);
	if (qmi.isValid())
	{
		fileinfosLocker->lock();
		int currentPriority=file_infos.at(qmi.row()).prioiry;
		fileinfosLocker->unlock();
		switch(currentPriority)
		{
			case 1:
			case 2:
				lowPriority->setChecked(true);
				mediumPriority->setChecked(false);
				highPriority->setChecked(false);
				dontDownload->setChecked(false);
				break;
			case 3:
			case 4:
			case 5:
				lowPriority->setChecked(false);
				mediumPriority->setChecked(true);
				highPriority->setChecked(false);
				dontDownload->setChecked(false);
				break;
			case 6:
			case 7:
				lowPriority->setChecked(false);
				mediumPriority->setChecked(false);
				highPriority->setChecked(true);
				dontDownload->setChecked(false);
				break;
			case 0:
				highPriority->setChecked(false);
				mediumPriority->setChecked(false);
				highPriority->setChecked(false);
				dontDownload->setChecked(true);
				break;
			default:
				highPriority->setChecked(false);
				mediumPriority->setChecked(false);
				highPriority->setChecked(false);
				dontDownload->setChecked(false);
				break;
		}
		fileTabMenu->exec(fileTableWidget->mapToGlobal(point));
	}
	else
	{

		fileTableWidget->selectionModel()->reset();

	}
}
void CuteTorrent::ShowNoUpdateNitify(const QString & ver)
{
	
	
	
}
void CuteTorrent::ShowTorrentError(const QString& name,const QString& error)
{
	
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_ERROR %1\n%2").arg(name).arg(error)
		,QSystemTrayIcon::Critical,15000,false);
	
}
void CuteTorrent::showTorrentCompletedNotyfy(const QString name)
{
	
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_DOWNLOAD_COMPLETE %1").arg(name),QSystemTrayIcon::Information,15000,false);
	
}

void CuteTorrent::updateTabWidget(int tab)
{
	if (this->isMinimized())
		return;
	if (tab==-1)
		tab=tabWidget->currentIndex();
	try
	{
		switch(tab)
		{
		case 0:
			//qDebug() << "Updating InfoTab";
			UpdateInfoTab();
			break;
		case 1:
			//qDebug() << "Updating PeerTab";
			UpdatePeerTab();
			break;
		case 2:
			//qDebug() << "Updating TrackerTab";
			UpadteTrackerTab();
			break;
		case 3:
			//qDebug() << "Updating FileTab";
			UpdateFileTab();
			break;
		}
		upLabelText->setText(QString("%1(%2)").arg(mng->GetSessionUploaded()).arg(mng->GetSessionUploadSpeed()));
		downLabelText->setText(QString("%1(%2)").arg(mng->GetSessionDownloaded()).arg(mng->GetSessionDownloadSpeed()));
	}
	catch (std::exception e)
	{
		QMessageBox::warning(this,"Error",QString("CuteTorrent::updateTabWidget()\n")+e.what());
	}
	update();
}
void CuteTorrent::UpdateFileTab()
{
	Torrent* tor=model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		//qDebug() << "Torrent is not null " << tor << " so begin updating tab";
		fileinfosLocker->lock();
		file_infos=tor->GetFileDownloadInfo();
		fileTableWidget->setRowCount(file_infos.count());
		//qDebug() << "file_infos.count() " <<file_infos.count();
		
		for (int i=0;i<file_infos.count();i++)
		{
			//qDebug() << "updating  " << i;
			file_info current=file_infos.at(i);
			//qDebug() << "updating  name" ;
			fileTableWidget->setItem(i,0,new QTableWidgetItem(current.name));
			//qDebug() << "updating  size" ;
			fileTableWidget->setItem(i,1,new QTableWidgetItem(StaticHelpers::toKbMbGb(current.size)));
			//qDebug() << "updating  progress " <<  current.progrss;
			fileTableWidget->setItem(i,2,new QTableWidgetItem(QString::number(current.progrss,'f',0)+" %"));
			//qDebug() << "updating  priority" ;
			fileTableWidget->setItem(i,3,new QTableWidgetItem(StaticHelpers::filePriorityToString(current.prioiry)));
		}
		fileinfosLocker->unlock();
	}
}
void CuteTorrent::setupTray()
{
	 createActions();
     createTrayIcon();
	 trayIcon->setToolTip("CuteTorrent "CT_VERSION);
	 trayIcon->show();
}

void CuteTorrent::changeEvent(QEvent *event)
{

	 if(event->type() == QEvent::ActivationChange) 
	 {     
		if(isVisible() && isMinimized()) 
		{
			event->ignore();
			QBalloonTip::showBalloon("CuteTorrent", tr("CT_HIDE_MSG"),QSystemTrayIcon::Information,5000,false);
			setUpdatesEnabled(false);
			hide();
			return;
		}
		else
		{
			showNormal();
			raise();
			activateWindow();
			setUpdatesEnabled(true);
		}

	}
	 if (event->type()==QEvent::LanguageChange)
	 {
		//QMessageBox::warning(this,"","retramslate event occured");
		retranslateUi(this);

		openFile->setText(tr("FILETAB_OPEN_FILE"));
		openDir->setText(tr("FILETAB_OPEN_FOLDER"));
		priority->setTitle(tr("FILETAB_PRIORITY"));
		lowPriority->setText(tr("FILETAB_PRIORITY_LOW"));
		mediumPriority->setText(tr("FILETAB_PRIORITY_MEDIUM"));
		highPriority->setText(tr("FILETAB_PRIORITY_HIGH"));
		dontDownload->setText(tr("FILETAB_PRIORITY_ZERO"));

		minimizeAction->setText(tr("ACTION_HIDE"));
		maximizeAction->setText(tr("ACTION_MAXIMIZE_FULLSCREEN"));
		restoreAction->setText(tr("ACTION_MAXIMIZE"));
		quitAction->setText(tr("ACTION_EXIT"));

		downLabel->setToolTip(tr("STATUS_DWONLOAD"));
		downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
		upLabel->setToolTip(tr("STATUS_UPLOAD"));
		upLabelText->setToolTip(tr("STATUS_UPLOAD"));
		model->retranslate();
	 }
  QMainWindow::changeEvent(event);


}
void CuteTorrent::iconActivated(QSystemTrayIcon::ActivationReason reason)
 {
     switch (reason) {
     case QSystemTrayIcon::Trigger:
		 break;
     case QSystemTrayIcon::DoubleClick:
		 if (isVisible())
			 hide();
		 else
		 {
			showNormal();
		}
         break;
     case QSystemTrayIcon::MiddleClick:
         showMessage();
         break;
     default:
         ;
     }
 }
void CuteTorrent::showMessage()
 {
	 
 }
 void CuteTorrent::messageClicked()
 {
     
     showNormal();
 }
void CuteTorrent::createTrayIcon()
 {
     trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(maximizeAction);
     trayIconMenu->addAction(restoreAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);

     trayIcon = new QSystemTrayIcon(this);
	 trayIcon->setIcon(QIcon(QString::fromUtf8(":/icons/app.ico")));
     trayIcon->setContextMenu(trayIconMenu);
 }
 void CuteTorrent::createActions()
 {
	 minimizeAction = new QAction(tr("ACTION_HIDE"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

     maximizeAction = new QAction(tr("ACTION_MAXIMIZE_FULLSCREEN"), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

     restoreAction = new QAction(tr("ACTION_MAXIMIZE"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

     quitAction = new QAction(tr("ACTION_EXIT"), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
 }
void CuteTorrent::ConnectMessageReceved(QtSingleApplication* a)
{
	QObject::connect(a,SIGNAL(messageReceived ( const QString & )), this, SLOT(HandleNewTorrent(const QString &)));
}
void CuteTorrent::HandleNewTorrent(const QString & path)
{
	showNormal();
	OpenTorrentDialog dlg(this);
	dlg.SetData(path);
	dlg.exec();
}
void CuteTorrent::ShowCreateTorrentDialog(void)
{
	CreateTorrentDialog dlg(this);
	
	dlg.exec();
}
void CuteTorrent::ShowOpenTorrentDialog()
{
	QApplicationSettings* settings=QApplicationSettings::getInstance();
	QString lastDir=settings->valueString("System","LastOpenTorrentDir","");
	QString filename =  QFileDialog::getOpenFileName(this,tr("OPEN_TOORENT_DIALOG"),
		lastDir , QString::fromLocal8Bit("Торрент файлы (*.torrent);;Any File (*.*)"));
	if (!filename.isEmpty())
	{
		OpenTorrentDialog dlg(this);
		settings->setValue("System","LastOpenTorrentDir",filename);
		dlg.SetData(filename);
		dlg.execConditional();
	}
	QApplicationSettings::FreeInstance();
}
void CuteTorrent::enableNitifyShow()
{
	mayShowNotifies = true;
}

void CuteTorrent::UpdateInfoTab()
{
	
	Torrent* tor=model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		if (tor==NULL) return;
		downloadedBytesLabel->setText(tor->GetTotalDownloaded());
		if (tor==NULL) return;
		uploadedBytesLabel->setText(tor->GetTotalUploaded());
		if (tor==NULL) return;
		downloadSpeedLabel->setText(tor->GetDwonloadSpeed());
		if (tor==NULL) return;
		activetimeLabel->setText(tor->GetActiveTime());
		if (tor==NULL) return;
		uploadSpeedLabel->setText(tor->GetUploadSpeed());
		if (tor==NULL) return;
		pathLabel->setText(tor->GetSavePath());
		if (tor==NULL) return;
		totalSizeLabel->setText(tor->GetTotalSize());
		if (tor==NULL) return;
		seedCoutLabel->setText(tor->GetSeedString());
		if (tor==NULL) return;
		peerCoutLabel->setText(tor->GetPeerString());
		if (tor==NULL) return;
		describtionLabel->setText("");
		if (tor==NULL) return;
		timeleftLabel->setText(tor->GetRemainingTime());
		if (tor==NULL) return;
	}
	else
	{
		downloadedBytesLabel->setText("");
		downloadSpeedLabel->setText("");
		uploadedBytesLabel->setText("");
		activetimeLabel->setText("");
		timeleftLabel->setText("");
		uploadSpeedLabel->setText("");
		pathLabel->setText("");
		totalSizeLabel->setText("");
		seedCoutLabel->setText("");
		peerCoutLabel->setText("");
		describtionLabel->setText("");
	}
	mng->PostTorrentUpdate();
}
void CuteTorrent::UpdatePeerTab()
{
	
	Torrent* tor=model->GetSelectedTorrent();
	
	if (tor!=NULL)
	{
		//qDebug() << "Torrent is not null " << tor << " so begin updating tab";
		std::vector<peer_info>	peerInfos=tor->GetPeerInfo();
		
		peerTableWidget->setRowCount(peerInfos.size());
		
		for(int i=0;i<peerInfos.size();i++)
		{
			peerTableWidget->setItem(i,0,new QTableWidgetItem(peerInfos[i].ip.address().to_string().c_str()));
			peerTableWidget->setItem(i,1,new QTableWidgetItem(QString::fromUtf8(peerInfos[i].client.c_str())));
			peerTableWidget->setItem(i,2,new QTableWidgetItem(QString::number(peerInfos[i].progress_ppm/10000.f,'f',1) + "%"));
			peerTableWidget->setItem(i,3,new QTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].down_speed)+"/s"));
			peerTableWidget->setItem(i,4,new QTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].up_speed)+"/s"));
			peerTableWidget->setItem(i,5,new QTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_download)));
			peerTableWidget->setItem(i,6,new QTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_upload)));
		
		}
		
	//	peerInfos.~vector();
	}
}
void CuteTorrent::UpadteTrackerTab()
{
	Torrent* tor=model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		//qDebug() << "Torrent is not null " << tor << " so begin updating tab";
		std::vector<announce_entry> trackers=tor->GetTrackerInfo();
		
		trackerTableWidget->setRowCount(trackers.size());
		
		for (int i=0;i<trackers.size();i++)
		{
			trackerTableWidget->setItem(i,0,new QTableWidgetItem(trackers[i].url.c_str()));
			trackerTableWidget->setItem(i,1,new QTableWidgetItem(trackers[i].message.length() >0 ? QString::fromUtf8(trackers[i].message.c_str()) : "Ok" ));
			trackerTableWidget->setItem(i,2,new QTableWidgetItem(StaticHelpers::toTimeString(trackers[i].next_announce_in())));
		
		}
		
	//	trackers.~vector();
	}
}
void CuteTorrent::PauseSelected()
{
	model->ActionOnSelectedItem(QTorrentDisplayModel::pause);
	mng->PostTorrentUpdate();
}
void CuteTorrent::ResumeSelected()
{
	model->ActionOnSelectedItem(QTorrentDisplayModel::resume);
	mng->PostTorrentUpdate();
}
void CuteTorrent::DeleteSelected()
{
	model->ActionOnSelectedItem(QTorrentDisplayModel::remove);
	mng->PostTorrentUpdate();
}
void CuteTorrent::retranslate()
{
	QMessageBox::warning(this,"","retranslateUi(this);");
	retranslateUi(this);
}
void CuteTorrent::OpenSettingsDialog()
{
	SettingsDialog* dlg = new SettingsDialog(this);
	QObject::connect(dlg,SIGNAL(needRetranslate()),this,SLOT(retranslate()));
	dlg->exec();
	delete dlg;
}
void CuteTorrent::closeEvent(QCloseEvent* ce)
{
	QMainWindow::closeEvent(ce);
	//qDebug() << "QMainWindow::~QMainWindow()";
	trayIcon->hide();
	//qDebug() << "TorrentManager::freeInstance()";
	mng->freeInstance();
	//qDebug() << "QTorrentDisplayModel::~QTorrentDisplayModel()";
	model->~QTorrentDisplayModel();
	QApplicationSettings::FreeInstance();
	delete notyfire;
}
CuteTorrent::~CuteTorrent()
{

}

void CuteTorrent::setupFileTabel()
{
	fileTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	fileTableWidget->setColumnWidth(0,300);
	fileTableWidget->setColumnWidth(1,60);
	fileTableWidget->setColumnWidth(2,60);
	fileTableWidget->setColumnWidth(3,60);
	
	setupFileTabelContextMenu();
}
void CuteTorrent::setupFileTabelContextMenu()
{
	fileTabMenu = new QMenu(this);
	openFile = new QAction(tr("FILETAB_OPEN_FILE"), fileTabMenu);
	QObject::connect(openFile, SIGNAL(triggered()), this, SLOT(OpenFileSelected()));
	fileTabMenu->addAction(openFile);
	openDir = new QAction(tr("FILETAB_OPEN_FOLDER"), this);
	QObject::connect(openDir, SIGNAL(triggered()), this, SLOT(OpenDirSelected()));
	fileTabMenu->addAction(openDir);
	fileTabMenu->addSeparator();
	priority = new QMenu(fileTabMenu);
	priority->setTitle(tr("FILETAB_PRIORITY"));
	lowPriority = new QAction(tr("FILETAB_PRIORITY_LOW"), fileTabMenu);
	lowPriority->setCheckable(true);
	QObject::connect(lowPriority, SIGNAL(triggered()), this, SLOT(setLowForCurrentFile()));
	priority->addAction(lowPriority);
	mediumPriority = new QAction(tr("FILETAB_PRIORITY_MEDIUM"), fileTabMenu);
	mediumPriority->setCheckable(true);
	QObject::connect(mediumPriority, SIGNAL(triggered()), this, SLOT(setMediumForCurrentFile()));
	priority->addAction(mediumPriority);
	highPriority = new QAction(tr("FILETAB_PRIORITY_HIGH"), fileTabMenu);
	highPriority->setCheckable(true);
	QObject::connect(highPriority, SIGNAL(triggered()), this, SLOT(setHighForCurrentFile()));
	priority->addAction(highPriority);
	fileTabMenu->addMenu(priority);
	fileTabMenu->addSeparator();
	dontDownload = new QAction(tr("FILETAB_PRIORITY_ZERO"), fileTabMenu);
	dontDownload->setCheckable(true);
	QObject::connect(dontDownload, SIGNAL(triggered()), this, SLOT(setNotDownloadForCurrentFile()));
	fileTabMenu->addAction(dontDownload);

}

void CuteTorrent::OpenFileSelected()
{
	Torrent* tor= model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		int file_num=fileTableWidget->currentRow();
		QDesktopServices desctopService;
		fileinfosLocker->lock();
		QString path=combine_path(tor->GetSavePath().toAscii().data(),file_infos.at(file_num).name.toAscii().data()).c_str();
		fileinfosLocker->unlock();
		//qDebug() << "trying to open file " << path;
		desctopService.openUrl(QUrl("file:///"+path));
	}
}

void CuteTorrent::OpenDirSelected()
{
	Torrent* tor= model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		int file_num=fileTableWidget->currentRow();
		fileinfosLocker->lock();
		QString path = QFileInfo(QDir::toNativeSeparators(tor->GetSavePath()+file_infos.at(file_num).name)).absoluteFilePath();
		fileinfosLocker->unlock();
#ifdef Q_WS_MAC
		QStringList args;
		args << "-e";
		args << "tell application \"Finder\"";
		args << "-e";
		args << "activate";
		args << "-e";
		args << "select POSIX file \""+path+"\"";
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
}

void CuteTorrent::setLowForCurrentFile()
{
	Torrent* tor= model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		
		mediumPriority->setChecked(false);
		highPriority->setChecked(false);
		dontDownload->setChecked(false);
		int file_num=fileTableWidget->currentRow();
		fileinfosLocker->lock();
		file_info current=file_infos.at(file_num);
		fileinfosLocker->unlock();
		tor->SetFilePriority(current.index,2);
	}
}

void CuteTorrent::setMediumForCurrentFile()
{
	Torrent* tor= model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		lowPriority->setChecked(false);
		highPriority->setChecked(false);
		dontDownload->setChecked(false);
		int file_num=fileTableWidget->currentRow();
		fileinfosLocker->lock();
		file_info current=file_infos.at(file_num);
		fileinfosLocker->unlock();
		tor->SetFilePriority(current.index,5);
	}
}

void CuteTorrent::setHighForCurrentFile()
{
	Torrent* tor= model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		lowPriority->setChecked(false);
		mediumPriority->setChecked(false);
		dontDownload->setChecked(false);
		int file_num=fileTableWidget->currentRow();
		fileinfosLocker->lock();
		file_info current=file_infos.at(file_num);
		fileinfosLocker->unlock();
		tor->SetFilePriority(current.index,7);
	}
}

void CuteTorrent::setNotDownloadForCurrentFile()
{
	Torrent* tor= model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		lowPriority->setChecked(false);
		mediumPriority->setChecked(false);
		highPriority->setChecked(false);
		int file_num=fileTableWidget->currentRow();
		fileinfosLocker->lock();
		file_info current=file_infos.at(file_num);
		fileinfosLocker->unlock();
		tor->SetFilePriority(current.index,0);
	}
}

void CuteTorrent::ProcessMagnet()
{
	OpenMagnetDialog* dlg = new OpenMagnetDialog(this);
	dlg->exec();
	delete dlg;
}

/*
void CuteTorrent::dropEvent( QDropEvent *event )
{
//	QMessageBox::warning(this,"",event->mimeData()->text());
}

void CuteTorrent::dragEnterEvent( QDragEnterEvent *event )
{
	QMessageBox::warning(this,"",event->mimeData()->text());
	if (event->mimeData()->hasFormat("application/x-bittorrent"))
		event->acceptProposedAction();
}
*/