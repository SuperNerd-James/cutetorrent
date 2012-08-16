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
CuteTorrent::CuteTorrent(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
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
void CuteTorrent::setupStatusBar()
{
	/*QIcon up(QString::fromUtf8(":/icons/upload.ico"));
	QIcon down(QString::fromUtf8(":/icons/download.ico"));*/
	QLabel* upLabel = new QLabel(this);
	upLabelText = new QLabel(this);
	upLabelText->setMinimumWidth(140);
	upLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/upload.ico")));
	upLabel->setToolTip(tr(QString::fromLocal8Bit("Отданно(Отдача)").toUtf8().data()));
	upLabelText->setToolTip(tr(QString::fromLocal8Bit("Отданно(Отдача)").toUtf8().data()));
	QLabel* downLabel = new QLabel(this);
	downLabel->setToolTip(tr(QString::fromLocal8Bit("Загружнно(Загрузка)").toUtf8().data()));
	downLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/download.ico")));
	downLabelText = new QLabel(this);
	downLabelText->setToolTip(tr(QString::fromLocal8Bit("Загружнно(Загрузка)").toUtf8().data()));
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
	SetupListViewActions();

}
void CuteTorrent::SetupListViewActions()
{
	

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
void CuteTorrent::setupTimer()
{
	QTimer *timer = new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateVisibleTorrents()));
    timer->start(1000);
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
	QObject::connect(model,SIGNAL(TorrentErrorPoxySender(const QString&)),this,SLOT(ShowTorrentError(const QString&)));
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
void CuteTorrent::ShowTorrentError(const QString& name)
{
	if (!mayShowNotifies)
		return;
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Critical;
	trayIcon->showMessage("CuteTorrent", QString::fromUtf8(tr(("При загрузке торрента %1 произошла ошибка")).arg(name).toUtf8().data()), icon,
		5* 1000);
}
void CuteTorrent::showTorrentCompletedNotyfy(const QString name)
{
	if (!mayShowNotifies)
		return;
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
			trayIcon->showMessage("CuteTorrent", QString::fromLocal8Bit(tr(("CuteTorrent завершил загрузку торрента "+
																		   name).toAscii().data()
																		   ).toAscii().data()), icon,
                           5* 1000);
}
void CuteTorrent::updateTabWidget(int tab)
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
void CuteTorrent::setupTray()
{
	 createActions();
     createTrayIcon();
	 trayIcon->setToolTip("CuteTorrent "CT_VERSION);
	 trayIcon->show();
}
void CuteTorrent::changeEvent(QEvent *event)
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
	 QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Warning;
     trayIcon->showMessage("Title of message", "Some info can be placed here", icon,
                           5* 1000);
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
	 minimizeAction = new QAction(QString::fromLocal8Bit(tr("Свернуть").toAscii().data()), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

     maximizeAction = new QAction(QString::fromLocal8Bit(tr("Развернуть на весь экран").toAscii().data()), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

     restoreAction = new QAction(QString::fromLocal8Bit(tr("Развернуть").toAscii().data()), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

     quitAction = new QAction(QString::fromLocal8Bit(tr("Выход").toAscii().data()), this);
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
void CuteTorrent::enableNitifyShow()
{
	mayShowNotifies = true;
}

void CuteTorrent::UpdateInfoTab()
{
	
	Torrent* tor=model->GetSelectedTorrent();
	if (tor!=NULL)
	{
		downloadedBytesLabel->setText(tor->GetTotalDownloaded());
		uploadedBytesLabel->setText(tor->GetTotalUploaded());
		downloadSpeedLabel->setText(tor->GetDwonloadSpeed());
		activetimeLabel->setText(tor->GetActiveTime());
		uploadSpeedLabel->setText(tor->GetUploadSpeed());
		pathLabel->setText(tor->GetSavePath());
		totalSizeLabel->setText(tor->GetTotalSize());
		seedCoutLabel->setText(tor->GetSeedString());
		peerCoutLabel->setText(tor->GetPeerString());
		describtionLabel->setText("");
		timeleftLabel->setText(tor->GetRemainingTime());
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
void CuteTorrent::updateVisibleTorrents()
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
	
	
	upLabelText->setText(QString("%1(%2)").arg(mng->GetSessionUploaded()).arg(mng->GetSessionUploadSpeed()));
	downLabelText->setText(QString("%1(%2)").arg(mng->GetSessionDownloaded()).arg(mng->GetSessionDownloadSpeed()));

	mng->PostTorrentUpdate();

}
void CuteTorrent::UpdatePeerTab()
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
void CuteTorrent::UpadteTrackerTab()
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
			trackerTableWidget->setItem(i,2,new QTableWidgetItem(StaticHelpers::toTimeString(trackers[i].next_announce_in())));
		}
		trackers.~vector();
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
void CuteTorrent::OpenSettingsDialog()
{
	SettingsDialog dlg(this);
	dlg.exec();
}
void CuteTorrent::closeEvent(QCloseEvent* ce)
{
	QMainWindow::closeEvent(ce);
	qDebug() << "QMainWindow::~QMainWindow()";
	trayIcon->hide();
	qDebug() << "TorrentManager::freeInstance()";
	mng->freeInstance();
	qDebug() << "QTorrentDisplayModel::~QTorrentDisplayModel()";
	model->~QTorrentDisplayModel();
	
}
CuteTorrent::~CuteTorrent()
{
	
}
