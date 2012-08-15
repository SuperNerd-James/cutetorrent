#include "ltorrent.h"
#include <QMessageBox>
#include <QComboBox>
#include <QDateTime>
#include <QAbstractItemView>
#include "QTorrentListView.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QIcon>
LTorrent::LTorrent(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	//QMessageBox::warning(0,"","LTorrent::LTorrent(QWidget *parent, Qt::WFlags flags)");
	setupUi(this);
	model = new QTorrentDisplayModel(listView);
	qDebug() << "QMainWindow ascked TorrentManager::getInstance";
	mng = TorrentManager::getInstance();
	mayShowNotifies = false;
	setupStatusBar();
	setupTray();
	setupToolBar();
	// TO-DO: reimplement to torrent changed signal
	setupTimer();

	setupListView();
	setupTabelWidgets();
	
	setupConnections();

	
	mng->initSession();
	QTimer::singleShot(5000,this,SLOT(enableNitifyShow()));
	qDebug() << "QMainWindow::QMainWindow()";
	
}
void LTorrent::setupStatusBar()
{
	/*QIcon up(QString::fromUtf8(":/icons/upload.ico"));
	QIcon down(QString::fromUtf8(":/icons/download.ico"));*/
	QLabel* upLabel = new QLabel(this);
	upLabelText = new QLabel(this);
	upLabelText->setMinimumWidth(140);
	upLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/upload.ico")));
	QLabel* downLabel = new QLabel(this);
	downLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/download.ico")));
	downLabelText = new QLabel(this);
	downLabelText->setMinimumWidth(140);
	statusBar()->addPermanentWidget(upLabel);
	statusBar()->addPermanentWidget(upLabelText);
	statusBar()->addPermanentWidget(downLabel);
	statusBar()->addPermanentWidget(downLabelText);
	
}
void LTorrent::setupListView()
{
	
	listView->setModel(model);
	listView->setSelectionMode(QAbstractItemView::SingleSelection );
	listView->setContextMenuPolicy(Qt::CustomContextMenu);
	SetupListViewActions();

}
void LTorrent::SetupListViewActions()
{
	

}

void LTorrent::setupTabelWidgets()
{
	trackerTableWidget->verticalHeader()->hide();
	trackerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	trackerTableWidget->setColumnWidth(2,120);
	peerTableWidget->verticalHeader()->hide();
	peerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	peerTableWidget->setSortingEnabled(true);

}
void LTorrent::setupTimer()
{
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateVisibleTorrents()));
    timer->start(1000);
}
void LTorrent::setupToolBar()
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
void LTorrent::setupConnections()
{
	QObject::connect(listView,SIGNAL(clicked(const QModelIndex &)),model,SLOT(UpdateSelectedIndex(const QModelIndex &)));
	QObject::connect(listView,SIGNAL(customContextMenuRequested(const QPoint &)),model,SLOT(contextualMenu(const QPoint &)));
	QObject::connect(listView,SIGNAL(clicked(const QModelIndex &)),this,SLOT(UpdateInfoTab()));
	QObject::connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	QObject::connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
	QObject::connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateTabWidget(int)));
	QObject::connect(model,SIGNAL(TorrentCompletedProxySender(const QString)),
									this,SLOT(showTorrentCompletedNotyfy(const QString)));
	QObject::connect(mng,SIGNAL(AddTorrentGui(Torrent*)),model,SLOT(AddTorrent(Torrent*)));
}
void LTorrent::showTorrentCompletedNotyfy(const QString name)
{
	if (!mayShowNotifies)
		return;
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
			trayIcon->showMessage("CuteTorrent", QString::fromLocal8Bit(tr(("CuteTorrent завершил загрузку торрента "+
																		   name).toAscii().data()
																		   ).toAscii().data()), icon,
                           5* 1000);
}
void LTorrent::updateTabWidget(int tab)
{
	switch(tab)
	{
	case 0:
		UpdateInfoTab();
		break;
	case 1:
		UpdatePeerTab();
		break;
	case 2:
		UpadteTrackerTab();
		break;

	}
}
void LTorrent::setupTray()
{
	 createActions();
     createTrayIcon();
	 trayIcon->setToolTip("CuteTorrent "CT_VERSION);
	 trayIcon->show();
}
void LTorrent::changeEvent(QEvent *event)
{

	 if(event->type() == QEvent::ActivationChange) {     
    if(isVisible() && isMinimized()) {
      event->ignore();
	  QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Warning;
			trayIcon->showMessage("CuteTorrent", QString::fromLocal8Bit(tr("CuteTorrent продолжает работать."
																		   "Что бы выйти выберете пункт Выход из контекстного меню.").toAscii().data()), icon,
                           5* 1000);
      hide();
      return;
    }
  }
  QMainWindow::changeEvent(event);


}
void LTorrent::iconActivated(QSystemTrayIcon::ActivationReason reason)
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
void LTorrent::showMessage()
 {
	 QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Warning;
     trayIcon->showMessage("Title of message", "Some info can be placed here", icon,
                           5* 1000);
 }
 void LTorrent::messageClicked()
 {
     
     showNormal();
 }
void LTorrent::createTrayIcon()
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
 void LTorrent::createActions()
 {
	 minimizeAction = new QAction(QString::fromLocal8Bit(tr("Свернуть").toAscii().data()), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

     maximizeAction = new QAction(QString::fromLocal8Bit(tr("Развернуть на весь экран").toAscii().data()), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

     restoreAction = new QAction(QString::fromLocal8Bit(tr("Развернуть").toAscii().data()), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

     quitAction = new QAction(QString::fromLocal8Bit(tr("Выход").toAscii().data()), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
 }
void LTorrent::ConnectMessageReceved(QtSingleApplication* a)
{
	QObject::connect(a,SIGNAL(messageReceived ( const QString & )), this, SLOT(HandleNewTorrent(const QString &)));
}
void LTorrent::HandleNewTorrent(const QString & path)
{
	showNormal();
	OpenTorrentDialog dlg(this);
	dlg.SetData(path);
	dlg.exec();
}
void LTorrent::ShowCreateTorrentDialog(void)
{
	CreateTorrentDialog dlg(this);
	
	dlg.exec();
}
void LTorrent::ShowOpenTorrentDialog()
{
	QApplicationSettings* settings=QApplicationSettings::getInstance();
	QString lastDir=settings->valueString("System","LastOpenTorrentDir","");
	QString filename =  QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit(tr("Открыть торрент").toAscii().data()), lastDir , QString::fromLocal8Bit("Торрент файлы (*.torrent);;Any File (*.*)"));
	if (!filename.isEmpty())
	{
		OpenTorrentDialog dlg(this);
		settings->setValue("System","LastOpenTorrentDir",filename);
		dlg.SetData(filename);
		dlg.execConditional();
	}
	QApplicationSettings::FreeInstance();
}
void LTorrent::enableNitifyShow()
{
	mayShowNotifies = true;
}

void LTorrent::UpdateInfoTab()
{
	
	Torrent* tor=model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		downloadedBytesLabel->setText(tor->GetTotalDownloaded());
		uploadedBytesLabel->setText(tor->GetTotalUploaded());
		downloadSpeedLabel->setText(tor->GetDwonloadSpeed());
		activetimeLabel->setText(tor->GetActiveTime());
		timeleftLabel->setText("");
		uploadSpeedLabel->setText(tor->GetUploadSpeed());
		pathLabel->setText(tor->GetSavePath());
		totalSizeLabel->setText(tor->GetTotalSize());
		seedCoutLabel->setText(tor->GetSeedString());
		peerCoutLabel->setText(tor->GetPeerString());
		describtionLabel->setText("");
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
void LTorrent::updateVisibleTorrents()
{
	mng->PostTorrentUpdate();
	torrent_filter filter=active;
	
	std::vector<torrent_status> torrents_to_display=mng->GetTorrents();
	setUpdatesEnabled( false );
	
	//model->clear();
	for (int i=0;i<torrents_to_display.size();i++)
	{
		model->updateTorrent(to_hex(torrents_to_display[i].info_hash.to_string()).c_str(),torrents_to_display[i]);
	}
	for(int k=0;k<model->rowCount();k++)
	{
	
		model->ChangeData(k);
		
		
	}
	
	
	torrents_to_display.~vector();
	updateTabWidget(tabWidget->currentIndex());
	setUpdatesEnabled( true );
	/*QMessageBox::warning(0,"","");/*/
//	statusBar()->removeWidget(upLabelText);
	
	upLabelText->setText(QString("%1(%2)").arg(mng->GetSessionUploaded()).arg(mng->GetSessionUploadSpeed()));
	downLabelText->setText(QString("%1(%2)").arg(mng->GetSessionDownloaded()).arg(mng->GetSessionDownloadSpeed()));
//	statusBar()->addWidget(upLabelText);
	mng->PostTorrentUpdate();
	//*/
}
void LTorrent::UpdatePeerTab()
{
	
	Torrent* tor=model->GetSelectedTorrent();
	
	if (tor!=NULL)
	{
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
		peerInfos.~vector();
	}
}
void LTorrent::UpadteTrackerTab()
{
	Torrent* tor=model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		std::vector<announce_entry> trackers=tor->GetTrackerInfo();
		//trackerTableWidget->clear();
		trackerTableWidget->setRowCount(trackers.size());
		for (int i=0;i<trackers.size();i++)
		{
			trackerTableWidget->setItem(i,0,new QTableWidgetItem(trackers[i].url.c_str()));
			trackerTableWidget->setItem(i,1,new QTableWidgetItem(trackers[i].message.length() >0 ? QString::fromUtf8(trackers[i].message.c_str()) : "Ok" ));
			QDateTime dt = QDateTime::fromTime_t(  trackers[i].next_announce_in());
			trackerTableWidget->setItem(i,2,new QTableWidgetItem(dt.toString("hh:mm:ss")));
		}
		trackers.~vector();
	}
}
void LTorrent::PauseSelected()
{
	model->ActionOnSelectedItem(QTorrentDisplayModel::pause);
	mng->PostTorrentUpdate();
}
void LTorrent::ResumeSelected()
{
	model->ActionOnSelectedItem(QTorrentDisplayModel::resume);
	mng->PostTorrentUpdate();
}
void LTorrent::DeleteSelected()
{
	model->ActionOnSelectedItem(QTorrentDisplayModel::remove);
	mng->PostTorrentUpdate();
}
void LTorrent::OpenSettingsDialog()
{
	SettingsDialog dlg(this);
	dlg.exec();
}
void LTorrent::closeEvent(QCloseEvent* ce)
{
	qDebug() << "QMainWindow::~QMainWindow()";
	trayIcon->hide();
	qDebug() << "TorrentManager::freeInstance()";
	mng->freeInstance();
	qDebug() << "QTorrentDisplayModel::~QTorrentDisplayModel()";
	model->~QTorrentDisplayModel();
	QMainWindow::closeEvent(ce);
}
LTorrent::~LTorrent()
{
	
}
