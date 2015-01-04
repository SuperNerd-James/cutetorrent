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
#include "Scheduller.h"
#include <QComboBox>
#include <QDateTime>
#include <QAbstractItemView>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QIcon>
#include "QBaloon.h"
#include <QProcess>
#include <QTextCodec>
#include <QtNetwork/QHostAddress>
#include <QSortFilterProxyModel>
#include <QClipboard>
#include "FileSizeItemDelegate.h"
#include "ProgressItemDelegate.h"
#include "PriorityItemDelegate.h"
#include <QTreeWidgetItem>
#include <QDesktopServices>
#include <QShortcut>
#include "messagebox.h"
#include "backupwizard/backupwizard.h"
#include "SearchEngine.h"

Q_DECLARE_METATYPE(QList<int>)

CuteTorrent::CuteTorrent(QWidget* parent)
	: BaseWindow(BaseWindow::FullTitle, BaseWindow::AllowResize)
{
	settings = QApplicationSettings::getInstance();
	Application::setLanguage(settings->valueString("System", "Lang", "ru_RU"));
	Application::setLanguageQt(settings->valueString("System", "Lang", "ru_RU"));
	styleEngine = StyleEngene::getInstance();
	m_pSearchEngine = new SearchEngine();
	styleEngine->setStyle(settings->valueString("System", "Style", "CuteTorrent"));
	setWindowModality(Qt::NonModal);
	setupCustomeWindow();
	
	m_pTorrentDisplayModel = new QTorrentDisplayModel(this, m_pTorrentListView, this);
	m_pSearchDisplayModel = new QSearchDisplayModel(m_pSearchEngine);
	m_pSearchItemDelegate = new QSearchItemDelegate();
	tManager = TorrentManager::getInstance();
	notyfire = new UpdateNotifier();
	mayShowNotifies = false;
	setAcceptDrops(true);
	setupStatusBar();
	setupTray();
	setupFileTabel();
	setupToolBar();
	setupListView();
	setupTabelWidgets();
	setupGroupTreeWidget();
	setupConnections();
	setupKeyMappings();
	initWindowIcons();
	tracker = TorrentTracker::getInstance();
	rcon = RconWebService::getInstance();

	if(settings->valueBool("WebControl", "webui_enabled", false))
	{
		rcon->Start();

		if(settings->valueBool("WebControl", "enable_ipfilter", false))
		{
			rcon->parseIpFilter(settings->valueString("WebControl", "ipfilter"));
		}

		if(settings->valueBool("WebControl", "enable_upnp", false))
		{
			int port = settings->valueInt("WebControl", "port", 8080);
			tManager->AddPortMapping(session::tcp, port, port);
		}
	}

	if(settings->valueBool("TorrentTracker", "enabled", false))
	{
		tracker->start();
	}

	m_pTorrentListView->installEventFilter(this);
	m_pGroupTreeWidget->installEventFilter(this);
	QTextCodec* wantUnicode = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(wantUnicode);
	tManager->initSession();
	torrents = TorrentStorrage::getInstance();
	QTimer::singleShot(10000, this, SLOT(CheckForUpdates()));
	Scheduller* sch = Scheduller::getInstance();
	//m_pSearchEngine->DoSerach(QString("sao"), ISerachProvider::Anime, 1);
}
void CuteTorrent::CheckForUpdates()
{
	notyfire->fetch();
}
void CuteTorrent::ShowAbout()
{
	MyMessageBox::about(this, tr("ABAUT_TITLE"), tr("ABAUT_TEXT").arg(CT_VERSION));
}
void CuteTorrent::ShowUpdateNitify(const QString& newVersion)
{
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_NEW_VERSION %1").arg(newVersion), QBalloonTip::UpdateNotyfy, qVariantFromValue(0), icon,
	                         5 * 1000);
}
void CuteTorrent::initStatusBarIcons()
{
	upLabel->setPixmap(styleEngine->getIcon("status_bar_upload").pixmap(16, 16));
	downLabel->setPixmap(styleEngine->getIcon("status_bar_download").pixmap(16, 16));
}

void CuteTorrent::setupStatusBar()
{
	upLabel = new QLabel(this);
	upLabelText = new QLabel(this);
	upLabelText->setMinimumWidth(140);
	downLabel = new QLabel(this);
	downLabelText = new QLabel(this);
	downLabel->setToolTip(tr("STATUS_DWONLOAD"));
	downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
	upLabel->setToolTip(tr("STATUS_UPLOAD"));
	upLabelText->setToolTip(tr("STATUS_UPLOAD"));
	downLabelText->setMinimumWidth(140);
	mystatusBar->addPermanentWidget(upLabel);
	mystatusBar->addPermanentWidget(upLabelText);
	mystatusBar->addPermanentWidget(downLabel);
	mystatusBar->addPermanentWidget(downLabelText);
	pieceView = new PeiceDisplayWidget(this);
	gridLayout_4->addWidget(pieceView, 0, 0, 1, 5);
	initStatusBarIcons();
}
void CuteTorrent::setupListView()
{
	QTorrentItemDelegat::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_LargeIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
	QSearchItemDelegate::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_ToolBarIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
	m_pTorrentItemDelegate = new QTorrentItemDelegat(this);
	m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
	m_pTorrentListView->setModel(m_pTorrentDisplayModel);
	m_pTorrentListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_pTorrentListView->setContextMenuPolicy(Qt::CustomContextMenu);
	m_pTorrentListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}
void CuteTorrent::setupTabelWidgets()
{
	trackerTableWidget->verticalHeader()->hide();
	trackerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	QList<int> tracker_column_sizes = settings->value("Window", "trackers_sizes").value<QList<int>>();

	if(tracker_column_sizes.count() > 0)
	{
		for(int i = 0; i < tracker_column_sizes.count(); i++)
		{
			trackerTableWidget->setColumnWidth(i, tracker_column_sizes.at(i));
		}
	}
	else
	{
		trackerTableWidget->setColumnWidth(0, 420);
		trackerTableWidget->setColumnWidth(2, 120);
	}

	trackerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	addTracker = new QAction(QIcon(":/MenuIcons/addTorrent.ico"), tr("ADD_TRACKER"), trackerTableWidget);
	addTracker->setObjectName("ACTION_OTHER_ADD_TRACKER");
	QObject::connect(addTracker, SIGNAL(triggered()), this, SLOT(AddTracker()));
	trackerTableWidget->addAction(addTracker);
	peerTableWidget->verticalHeader()->hide();
	peerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	peerTableWidget->setSortingEnabled(true);
	QList<int> peer_column_sizes = settings->value("Window", "peers_sizes").value<QList<int>>();

	if(peer_column_sizes.count() > 0)
	{
		for(int i = 0; i < peer_column_sizes.count(); i++)
		{
			peerTableWidget->setColumnWidth(i, peer_column_sizes.at(i));
		}
	}
	else
	{
		peerTableWidget->setColumnWidth(2, 50);
	}

	peerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	addPeer = new QAction(QIcon(":/MenuIcons/addTorrent.ico"), tr("ADD_PEER"), peerTableWidget);
	addPeer->setObjectName("ACTION_OTHER_ADD_PPER");
	QObject::connect(addPeer, SIGNAL(triggered()), this, SLOT(AddPeer()));
	peerTableWidget->addAction(addPeer);
}
void CuteTorrent::initToolbarIcons()
{
	ACTION_TOOLBAR_START->setIcon(styleEngine->getIcon("toolbar_start"));
	ACTION_TOOLBAR_PAUSE->setIcon(styleEngine->getIcon("toolbar_pause"));
	ACTION_TOOLBAR_REMOVE->setIcon(styleEngine->getIcon("toolbar_remove"));
	ACTION_TOOLBAR_SETTINGS->setIcon(styleEngine->getIcon("toolbar_settings"));
}

void CuteTorrent::setupToolBar()
{
	initToolbarIcons();
	searchSource = new QComboBox(this);
	searchSources = settings->GetSearchSources();

	for(int i = 0; i < searchSources.size(); i++)
	{
		searchSource->addItem(searchSources[i].getName());
	}

	searchEdit = new QLineEdit(this);
	QObject::connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(PeformSearch()));
	ul = new QSpinBox(this);
	ul->setSuffix(" Kb\\s");
	ul->setMaximum(12000.0f);
	ul->setSingleStep(10.0);
	ul->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	QObject::connect(ul, SIGNAL(valueChanged(int)), this, SLOT(UpdateUL(int)));
	dl = new QSpinBox(this);
	ul->setSpecialValueText(tr("None"));
	dl->setSpecialValueText(tr("None"));
	dl->setMaximum(12000.0f);
	dl->setSuffix(" Kb\\s");
	dl->setSingleStep(10.0);
	dl->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	QObject::connect(dl, SIGNAL(valueChanged(int)), this, SLOT(UpdateDL(int)));
	uploadLimit = new QLabel(tr("LIMIT_UL"), this);
	uploadLimit->setBuddy(ul);
	downloadLimit = new QLabel(tr("LIMIT_DL"), this);
	downloadLimit->setBuddy(dl);
	searchEdit->setMaximumWidth(145);
	searchSource->setMaximumWidth(150);
	searchSource->setMinimumWidth(150);
//   mainToolbar->addSeparator();
	mainToolbar->addWidget(uploadLimit);
	mainToolbar->addWidget(ul);
	mainToolbar->addWidget(downloadLimit);
	mainToolbar->addWidget(dl);
//    mainToolbar->addSeparator();
	mainToolbar->addWidget(searchEdit);
	mainToolbar->addWidget(searchSource);
	UpdateTabWidget(-2);
}
void CuteTorrent::setupConnections()
{
	QObject::connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	                 m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
	QObject::connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pTorrentDisplayModel, SLOT(contextualMenu(const QPoint&)));
	QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	                 this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
	QObject::connect(ACTION_MENU_EXIT, SIGNAL(triggered()), qApp, SLOT(quit()));
	QObject::connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTabWidget(int)));
	QObject::connect(m_pTorrentDisplayModel, SIGNAL(updateTabSender(int)), this, SLOT(UpdateTabWidget(int)));
	QObject::connect(tManager, SIGNAL(TorrentError(const QString&, const QString&)), this, SLOT(ShowTorrentError(const QString&, const QString&)));
	QObject::connect(tManager, SIGNAL(TorrentCompleted(const QString&, const QString&)),
	                 this, SLOT(ShowTorrentCompletedNotyfy(const QString, const QString)));
	QObject::connect(tManager, SIGNAL(TorrentInfo(const QString&, const QString&)),
	                 this, SLOT(ShowTorrentInfoNotyfy(const QString, const QString)));
	//QObject::connect(mng,SIGNAL(AddTorrentGui(Torrent*)),model,SLOT(AddTorrent(Torrent*)));
	QObject::connect(notyfire, SIGNAL(showUpdateNitify(const QString&)), this, SLOT(ShowUpdateNitify(const QString&)));
	QObject::connect(notyfire, SIGNAL(showNoUpdateNitify(const QString&)), this, SLOT(ShowNoUpdateNitify(const QString&)));
	QObject::connect(fileTableView, SIGNAL(customContextMenuRequested(const QPoint&)), fileViewModel, SLOT(FileTabContextMenu(const QPoint&)));
	QObject::connect(tManager, SIGNAL(initCompleted()), m_pTorrentDisplayModel, SLOT(initSessionFinished()));
	QObject::connect(m_pTorrentDisplayModel, SIGNAL(initCompleted()), this, SLOT(EnableNitifyShow()));
	QObject::connect(m_pGroupTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ChnageTorrentFilter()));
	QObject::connect(styleEngine, SIGNAL(styleChanged()), this, SLOT(initWindowIcons()));
	QObject::connect(styleEngine, SIGNAL(styleChanged()), m_pTorrentDisplayModel, SLOT(setupContextMenu()));
	QObject::connect(m_pTorrentListView, SIGNAL(doubleClicked(const QModelIndex&)), m_pTorrentDisplayModel, SLOT(OpenDirSelected()));
	QObject::connect(m_pSearchEngine, SIGNAL(GotResults()), this, SLOT(OnGotSerachResults()));
}
void CuteTorrent::ShowNoUpdateNitify(const QString& ver)
{
}
void CuteTorrent::ShowTorrentError(const QString& name, const QString& error)
{
	if(!mayShowNotifies)
	{
		return;
	}

	QBalloonTip::showBalloon("CuteTorrent", tr("CT_ERROR %1\n%2").arg(name).arg(error), QBalloonTip::Error, qVariantFromValue(0),
	                         QSystemTrayIcon::Critical, 15000, false);
}
void CuteTorrent::ShowTorrentCompletedNotyfy(const QString& name, const QString& path)
{
	if(!mayShowNotifies)
	{
		return;
	}

	QBalloonTip::showBalloon("CuteTorrent", tr("CT_DOWNLOAD_COMPLETE %1").arg(name), QBalloonTip::TorrentCompleted, qVariantFromValue(path + name),
	                         QSystemTrayIcon::Information, 15000, false);
}
void CuteTorrent::UpdateTabWidget(int tabIndex)
{
	
	trayIcon->setToolTip("CuteTorrent "CT_VERSION"\nUpload: " + tManager->GetSessionUploadSpeed() + "\nDownload:" + tManager->GetSessionDownloadSpeed());

	if(this->isMinimized())
	{
		return;
	}

	bool udapteLimits = false;

	if(tabIndex < 0)
	{
		if(tabIndex == -2)
		{
			udapteLimits = true;
		}

		tabIndex = tabWidget->currentIndex();
	}

	
	try
	{
		switch(tabIndex)
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

			case 3:
				UpdateFileTab();
				break;
		}

		if(udapteLimits)
		{
			Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

			if(tor != NULL)
			{
				if(ul->value() != tor->GetUploadLimit() / 1024)
				{
					ul->setValue(tor->GetUploadLimit() / 1024);
				}

				if(dl->value() != tor->GetDownloadLimit() / 1024)
				{
					dl->setValue(tor->GetDownloadLimit() / 1024);
				}
			}
			else
			{
				if(ul->value() != tManager->GetUploadLimit() / 1024)
				{
					ul->setValue(tManager->GetUploadLimit() / 1024);
				}

				if(dl->value() != tManager->GetDownloadLimit() / 1024)
				{
					dl->setValue(tManager->GetDownloadLimit() / 1024);
				}
			}
		}

		upLabelText->setText(QString("%1(%2)").arg(tManager->GetSessionUploaded()).arg(tManager->GetSessionUploadSpeed()));
		downLabelText->setText(QString("%1(%2)").arg(tManager->GetSessionDownloaded()).arg(tManager->GetSessionDownloadSpeed()));
		//	torrentFilterModel->Update();
	}
	catch(std::exception e)
	{
		MyMessageBox::warning(this, "Error", QString("CuteTorrent::updateTabWidget()\n") + e.what());
	}
}

class MyTableWidgetItem : public QTableWidgetItem
{
public:
	MyTableWidgetItem(QString text) : QTableWidgetItem(text)
	{
	}
	bool operator < (const QTableWidgetItem& other) const
	{
		QHostAddress thisadr(text());

		if(!thisadr.isNull())
		{
			
			QHostAddress otheradr(other.text());
			return thisadr.toIPv4Address() < otheradr.toIPv4Address();
		}
		else if(text().contains(" tb", Qt::CaseInsensitive) || text().contains(" gb", Qt::CaseInsensitive) || text().contains(" kb", Qt::CaseInsensitive) || text().contains(" mb", Qt::CaseInsensitive) ||
		        text().contains(" b", Qt::CaseInsensitive))
		{
			QStringList parts1 = text().split(' ');
			bool ok;
			double speed1 = parts1[0].toDouble(&ok);

			if(ok)
			{
				QStringList parts2 = other.text().split(' ');
				double speed2 = parts2[0].toDouble();

				switch(parts1[1][0].toLower().toLatin1())
				{
					case 'k':
						speed1 *= 1024.0;
						break;

					case 'm':
						speed1 *= 1024 * 1024.0;
						break;

					case 'g':
						speed1 *= 1024 * 1024 * 1024.0;
						break;

					case 't':
						speed1 *= 1024 * 1024 * 1024 * 1024.0;
						break;

					case 'b':
						break;
				}

				
				switch(parts2[1][0].toLower().toLatin1())
				{
					case 'k':
						speed2 *= 1024;
						break;

					case 'm':
						speed2 *= 1024 * 1024;
						break;

					case 'g':
						speed2 *= 1024 * 1024 * 1024;
						break;

					case 't':
						speed2 *= 1024 * 1024 * 1024 * 1024.0;
						break;

					case 'b':
						break;
				}

				
				return speed1 < speed2;
			}
		}
		else if(text().endsWith('%'))
		{
			
			QString perc1 = text().remove('%'), perc2 = other.text().remove('%');
			return perc1.toDouble() < perc2.toDouble();
		}

		return QTableWidgetItem::operator < (other);
	}
};
void CuteTorrent::UpdateFileTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		if(fileViewModel->setDataSource(tor->GetInternalHandle()))
		{
			fileTableView->expand(proxymodel->index(0, 0));
		}
	}
	else
	{
		fileViewModel->setDataSource(torrent_handle());
	}
}
void CuteTorrent::setupTray()
{
	createActions();
	createTrayIcon();
	trayIcon->setToolTip("CuteTorrent "CT_VERSION);
	trayIcon->show();
}

void CuteTorrent::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::ActivationChange)
	{
		if(!isVisible() && isMinimized())
		{
			setUpdatesEnabled(false);
		}
		else
		{
			setUpdatesEnabled(true);
		}
	}

	if(event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);
		minimizeAction->setText(tr("ACTION_HIDE"));
		maximizeAction->setText(tr("ACTION_MAXIMIZE_FULLSCREEN"));
		restoreAction->setText(tr("ACTION_MAXIMIZE"));
		quitAction->setText(tr("ACTION_EXIT"));
		copyContext->setText(tr("ACTION_COPY"));
		downLabel->setToolTip(tr("STATUS_DWONLOAD"));
		downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
		upLabel->setToolTip(tr("STATUS_UPLOAD"));
		upLabelText->setToolTip(tr("STATUS_UPLOAD"));
		uploadLimit->setText(tr("LIMIT_UL"));
		downloadLimit->setText(tr("LIMIT_DL"));
		addTracker->setText(tr("ADD_TRACKER"));
		addPeer->setText(tr("ADD_PEER"));
		torrentTreeItem->setText(0, tr("TORRENTS_ACTIVITY"));
		dlTreeItem->setText(0, tr("DOWNLOADING_FLTR"));
		ulTreeItem->setText(0, tr("SEEDING_FLTR"));
		completedTreeItem->setText(0, tr("COMPLETED_FLTR"));
		activeTreeItem->setText(0, tr("ACTIVE_FLTR"));
		inactiveTreeItem->setText(0, tr("NOT_ACTIVE_FLTR"));
		groupsTreeItem->setText(0, tr("TORRENT_GROUPS"));
		ul->setSpecialValueText(tr("None"));
		dl->setSpecialValueText(tr("None"));
		fileViewModel->retranslateUI();
		m_pTorrentDisplayModel->retranslate();
	}

	QWidget::changeEvent(event);
}
void CuteTorrent::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
		case QSystemTrayIcon::DoubleClick:
			if(isVisible())
			{
				hide();
			}
			else
			{
				showNormal();
				raise();
				activateWindow();
			}

			break;

		default:
			break;
	}
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
	minimizeAction = new QAction(styleEngine->getIcon("app_min"), tr("ACTION_HIDE"), this);
	minimizeAction->setObjectName("ACTION_TRAY_MINIMIZE");
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
	maximizeAction = new QAction(styleEngine->getIcon("app_max"), tr("ACTION_MAXIMIZE_FULLSCREEN"), this);
	maximizeAction->setObjectName("ACTION_TRAY_MAXIMIZE");
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
	restoreAction = new QAction(styleEngine->getIcon("app_max"), tr("ACTION_MAXIMIZE"), this);
	restoreAction->setObjectName("ACTION_TRAY_RESTORE");
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	quitAction = new QAction(styleEngine->getIcon("app_close"), tr("ACTION_EXIT"), this);
	quitAction->setObjectName("ACTION_TRAY_EXIT");
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	copyContext = new QAction(styleEngine->getIcon("copy"), tr("ACTION_COPY"), describtionLabel);
	copyContext->setObjectName("ACTION_OTHER_DISCRIBTION");
	connect(copyContext, SIGNAL(triggered()), this, SLOT(CopyDiscribtion()));
	describtionLabel->addAction(copyContext);
}
void CuteTorrent::ConnectMessageReceved(Application* a)
{
	QObject::connect(a, SIGNAL(messageReceived(const QString&)), this, SLOT(HandleNewTorrent(const QString&)));
	QObject::connect(a, SIGNAL(OpenTorrent(QString)), this, SLOT(HandleNewTorrent(QString)));
}
void CuteTorrent::HandleNewTorrent(const QString& path)
{
	// This hack does not give the focus to the app but brings it to front so
	// the user sees it.
	::SetWindowPos(effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::SetWindowPos(effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	// HACK END
	showNormal();
	setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	raise();
	activateWindow();
	OpenTorrentDialog dlg(this);
	dlg.SetData(path);
	dlg.execConditional();
	QApplication::alert(&dlg);
}
void CuteTorrent::ShowCreateTorrentDialog(void)
{
	CreateTorrentDialog* dlg = new CreateTorrentDialog();
	dlg->exec();
}
void CuteTorrent::ShowOpenTorrentDialog()
{
	QString lastDir = settings->valueString("System", "LastOpenTorrentDir", "");
	QString filename =  QFileDialog::getOpenFileName(this, tr("OPEN_TOORENT_DIALOG"),
	                    lastDir , tr("TORRENT_FILES (*.torrent);;Any File (*.*)"));

	if(!filename.isEmpty())
	{
		OpenTorrentDialog* dlg = new OpenTorrentDialog(this);
		settings->setValue("System", "LastOpenTorrentDir", filename);
		dlg->SetData(filename);
		dlg->execConditional();
		delete dlg;
	}
}
void CuteTorrent::EnableNitifyShow()
{
	mayShowNotifies = true;
}

void CuteTorrent::UpdateInfoTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		fillPieceDisplay(pieceView->size());
		downloadedBytesLabel->setText(tor->GetTotalDownloaded());
		uploadedBytesLabel->setText(tor->GetTotalUploaded());
		downloadSpeedLabel->setText(tor->GetDwonloadSpeed());
		activetimeLabel->setText(tor->GetActiveTime());
		uploadSpeedLabel->setText(tor->GetUploadSpeed());
		pathLabel->setText(tor->GetSavePath());
		totalSizeLabel->setText(tor->GetTotalSize());
		seedCoutLabel->setText(tor->GetSeedString());
		peerCoutLabel->setText(tor->GetPeerString());
		QFontMetrics metrics(describtionLabel->font());
		QString elidedText = metrics.elidedText(tor->GetDiscribtion(), Qt::ElideRight, describtionLabel->width());
		describtionLabel->setText(elidedText);
		timeleftLabel->setText(tor->GetRemainingTime());
	}
	else
	{
		ClearPieceDisplay();
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
}
void CuteTorrent::UpdatePeerTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		QVector<peer_info> peerInfos = QVector<peer_info>::fromStdVector(tor->GetPeerInfo());
		peerTableWidget->setRowCount(peerInfos.size());

		for(int i = 0; i < peerInfos.size(); i++)
		{
			QString client = QString::fromStdString(peerInfos[i].client);

			if((peerInfos[i].flags & peer_info::rc4_encrypted) == peer_info::rc4_encrypted
			        || (peerInfos[i].flags & peer_info::plaintext_encrypted) == peer_info::plaintext_encrypted)
			{
				client = client.append(" *");
			}

			peerTableWidget->setItem(i, 0, new MyTableWidgetItem(QString::fromStdString(peerInfos[i].ip.address().to_string())));
			peerTableWidget->setItem(i, 1, new MyTableWidgetItem(client));
			peerTableWidget->setItem(i, 2, new MyTableWidgetItem(QString::number(peerInfos[i].progress_ppm / 10000.f, 'f', 1) + "%"));
			peerTableWidget->setItem(i, 3, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].down_speed) + "/s"));
			peerTableWidget->setItem(i, 4, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].up_speed) + "/s"));
			peerTableWidget->setItem(i, 5, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_download)));
			peerTableWidget->setItem(i, 6, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_upload)));
		}

		//peerTableWidget->resizeColumnsToContents();
	}
	else
	{
		peerTableWidget->setRowCount(0);
	}
}
void CuteTorrent::UpadteTrackerTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		std::vector<announce_entry> trackers = tor->GetTrackerInfo();
		trackerTableWidget->setRowCount(trackers.size());

		for(size_type i = 0; i < trackers.size(); i++)
		{
			trackerTableWidget->setItem(i, 0, new QTableWidgetItem(trackers[i].url.c_str()));
			trackerTableWidget->setItem(i, 1, new QTableWidgetItem(trackers[i].message.length() > 0 ? QString::fromUtf8(trackers[i].message.c_str()) : "Ok"));
			trackerTableWidget->setItem(i, 2, new QTableWidgetItem(StaticHelpers::toTimeString(trackers[i].next_announce_in())));
		}
	}
	else
	{
		trackerTableWidget->setRowCount(0);
	}
}
void CuteTorrent::PauseSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::pause);
}
void CuteTorrent::ResumeSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::resume);
}
void CuteTorrent::DeleteSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::remove);
}
void CuteTorrent::Retranslate()
{
	retranslateUi(this);
}
void CuteTorrent::OpenSettingsDialog()
{
	SettingsDialog* dlg = new SettingsDialog(this);
	QObject::connect(dlg, SIGNAL(needRetranslate()), this, SLOT(Retranslate()));
	dlg->exec();
	delete dlg;
	searchSources = settings->GetSearchSources();
	searchSource->clear();

	for(int i = 0; i < searchSources.size(); i++)
	{
		searchSource->addItem(searchSources[i].getName());
	}

	setupGroupTreeWidget();
	setupKeyMappings();
	UpdateTabWidget(-2);
}
void CuteTorrent::closeEvent(QCloseEvent* ce)
{
	ce->ignore();
	hide();
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_HIDE_MSG"), QBalloonTip::Info, qVariantFromValue(0),
	                         QSystemTrayIcon::Information, 5000, false);
	setUpdatesEnabled(false);
	return;
}

void CuteTorrent::dragEnterEvent(QDragEnterEvent* event)
{
	if(event->mimeData()->hasFormat(QString("text/plain")) || event->mimeData()->hasFormat(QString("text/uri-list")))
	{
		event->acceptProposedAction();
	}
}

void CuteTorrent::dropEvent(QDropEvent* event)
{
	QStringList files;

	if(event->mimeData()->hasUrls())
	{
		const QList<QUrl> urls = event->mimeData()->urls();

		foreach(const QUrl& url, urls)
		{
			if(!url.isEmpty())
			{
				if(url.scheme().compare("file", Qt::CaseInsensitive) == 0)
				{
					files << url.toLocalFile();
				}
				else
				{
					files << url.toString();
				}
			}
		}
	}
	else
	{
		files = event->mimeData()->text().split(QString::fromUtf8("\n"));
	}

	foreach(QString file, files)
	{
		if(file.startsWith("magnet:", Qt::CaseInsensitive) || file.endsWith(".torrent", Qt::CaseInsensitive))
		{
			OpenTorrentDialog* dlg2 = new OpenTorrentDialog();
			dlg2->SetData(file);
			dlg2->exec();
			delete dlg2;
			event->acceptProposedAction();
		}
		else
		{
			
		}
	}
}
CuteTorrent::~CuteTorrent()
{
	settings->setValue("Window", "geometry", geometry());
	settings->setValue("Window", "maximized", isMaximized());
	settings->setValue("Window", "selected_tab", tabWidget->currentIndex());
	settings->setValue("Window", "horizontal_sizes", QVariant::fromValue(spliiter1->sizes()));
	settings->setValue("Window", "vertical_sizes", QVariant::fromValue(spliiter->sizes()));
	QList<int> peer_columns_sizes;

	for(int i = 0; i < peerTableWidget->columnCount(); i++)
	{
		peer_columns_sizes.append(peerTableWidget->columnWidth(i));
	}

	settings->setValue("Window", "peers_sizes", QVariant::fromValue(peer_columns_sizes));
	QList<int> tracker_columns_sizes;

	for(int i = 0; i < trackerTableWidget->columnCount(); i++)
	{
		tracker_columns_sizes.append(trackerTableWidget->columnWidth(i));
	}

	settings->setValue("Window", "trackers_sizes", QVariant::fromValue(tracker_columns_sizes));
	QList<int> file_columns_sizes;

	for(int i = 0; i < 4; i++)
	{
		file_columns_sizes.append(fileTableView->columnWidth(i));
	}

	settings->setValue("Window", "files_sizes", QVariant::fromValue(file_columns_sizes));
	
	TorrentTracker::freeInstance();
	RconWebService::freeInstance();
	trayIcon->hide();
	TorrentManager::freeInstance();
	delete m_pSearchDisplayModel;
	delete m_pSearchEngine;
	delete m_pTorrentDisplayModel;
	delete m_pSearchItemDelegate;
	delete m_pTorrentItemDelegate;
	Scheduller::freeInstance();
	
	QApplicationSettings::FreeInstance();
	delete notyfire;
	delete trayIcon;
}

void CuteTorrent::setupFileTabel()
{
	proxymodel = new FileViewSortProxyModel(this);
	fileTableView->setModel(proxymodel);
	fileViewModel = new FileViewModel(fileTableView, this);
	proxymodel->setSourceModel(fileViewModel);
//    fileTableView->setShowGrid(false);
	fileTableView->setContextMenuPolicy(Qt::CustomContextMenu);
	fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	fileTableView->setItemDelegateForColumn(1, new FileSizeItemDelegate(this));
	fileTableView->setItemDelegateForColumn(2, new ProgressItemDelegate(this));
	fileTableView->setItemDelegateForColumn(3, new PriorityItemDelegate(this));
	fileTableView->setSortingEnabled(true);
	QList<int> file_column_sizes = settings->value("Window", "files_sizes").value<QList<int>>();

	if(file_column_sizes.count() > 0)
	{
		for(int i = 0; i < file_column_sizes.count(); i++)
		{
			fileTableView->setColumnWidth(i, file_column_sizes.at(i));
		}
	}
	else
	{
		fileTableView->setColumnWidth(0, 480);
		fileTableView->setColumnWidth(1, 65);
		fileTableView->setColumnWidth(2, 65);
		fileTableView->setColumnWidth(3, 70);
	}

	fileTableView->header()->setDefaultAlignment(Qt::AlignCenter);
	fileTableView->header()->setSortIndicatorShown(true);  // optional
	fileTableView->header()->setClickable(true);
}


void CuteTorrent::ProcessMagnet()
{
	bool ok;
	QString magnetLink = QInputDialog::getText(this, tr("MAGNET_LINK_DLG"), tr("MAGNET_LINK:"), QLineEdit::Normal, "", &ok);

	if(ok && !magnetLink.isEmpty())
	{
		OpenTorrentDialog* dlg2 = new OpenTorrentDialog();
		dlg2->SetData(magnetLink);
		dlg2->exec();
		delete dlg2;
	}
}

void CuteTorrent::PeformSearch()
{
	QString searchText = searchEdit->text();
	m_pSearchEngine->DoSerach(searchText, ISerachProvider::All, 1);
	switchToSearchModel();
}

void CuteTorrent::resizeEvent(QResizeEvent* event)
{
	fillPieceDisplay(pieceView->size());
	QTorrentItemDelegat::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_MessageBoxIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
	QSearchItemDelegate::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_MessageBoxIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
}

void CuteTorrent::ShowTorrentInfoNotyfy(const QString& name, const QString& info)
{
#ifndef Q_WS_MAC
	QBalloonTip::showBalloon("CuteTorrent", QString("%1\n%2").arg(name).arg(info), QBalloonTip::Info, qVariantFromValue(0),
	                         QSystemTrayIcon::Information, 15000, false);
#else
	trayIcon->showMessage("CuteTorrent", QString("%1\n%2").arg(name).arg(info), QSystemTrayIcon::Information);
#endif
}

void CuteTorrent::keyPressEvent(QKeyEvent* event)
{
	if(!isVisible())
	{
		QWidget::keyPressEvent(event);
		return;
	}

	QKeySequence pressedKey = QKeySequence((event->key() == Qt::Key_Return ? Qt::Key_Enter : event->key()) | event->modifiers());
	QVariantMap keyMap = settings->getGroupValues("KeyMap");
	QStringList keys = keyMap.keys();

	/* qDebug() << pressedKey;
	 qDebug() << keyMap;*/
	foreach(QString key, keys)
	{
		
		if(QKeySequence(keyMap[key].toString()) == pressedKey)
		{
			QAction* action = this->findChild<QAction*> (key);

			if(action != NULL)
				// qDebug() << "Matched action:" << key;
			{
				action->activate(QAction::Trigger);
			}
		}
	}
}

void CuteTorrent::fillPieceDisplay(QSize size)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		int piece_count = tor->GetPieceCount();
		QVector<int> avaliablePieces = tor->GetDownloadedPieces();
		QVector<int> dwonloadingPieces = tor->GetDownloadingPieces();
		pieceView->resize(size);
		pieceView->setDowloadedParts(avaliablePieces);
		pieceView->setDowloadingParts(dwonloadingPieces);
		pieceView->setPiceCount(piece_count);
		pieceView->update();
	}
}

void CuteTorrent::CopyDiscribtion()
{
	QClipboard* clipboard = QApplication::clipboard();
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		clipboard->setText(tor->GetDiscribtion());
	}
}

void CuteTorrent::ClearPieceDisplay()
{
	pieceView->setPiceCount(0);
	/*QGraphicsScene *scene = new QGraphicsScene(this);
	scene->clear();
	piceDwonloadedView->scene()->deleteLater();
	piceDwonloadedView->setScene(scene);
	piceDwonloadedView->show();*/
}

void CuteTorrent::UpdateUL(int kbps)
{
	
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		tor->SetUlLimit(kbps * 1024);
	}
	else
	{
		//settings = QApplicationSettings::getInstance();
		settings->setValue("Torrent", "upload_rate_limit", kbps * 1024);
		//QApplicationSettings::FreeInstance();
		tManager->SetUlLimit(kbps * 1024);
	}
}

void CuteTorrent::UpdateDL(int kbps)
{
	
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		tor->SetDlLimit(kbps * 1024);
	}
	else
	{
		//	QApplicationSettings* settings = QApplicationSettings::getInstance();
		settings->setValue("Torrent", "download_rate_limit", kbps * 1024);
		//	QApplicationSettings::FreeInstance();
		tManager->SetDlLimit(kbps * 1024);
	}
}

void CuteTorrent::AddPeer()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(torrent != NULL)
	{
		bool ok;
		QString peerStr = QInputDialog::getText(this, tr("ADD_PEER_DLG"), tr("PEER:"), QLineEdit::Normal, "", &ok);

		if(ok && !peerStr.isEmpty())
		{
			QStringList parts = peerStr.split(':');

			if(parts.count() == 2)
			{
				QHostAddress adr(parts[0]);

				if(!adr.isNull())
				{
					torrent->AddPeer(adr, parts[1].toUInt());
					return;
				}
			}

			MyMessageBox::critical(this, tr("PEER_ERR"), tr("INVALID_IP_STRING"));
		}
	}
}

void CuteTorrent::AddTracker()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(torrent != NULL)
	{
		bool ok;
		QString trackerUrl = QInputDialog::getText(this, tr("ADD_TRACKER_DLG"), tr("TRACKER:"), QLineEdit::Normal, "", &ok);

		if(ok && !trackerUrl.isEmpty())
		{
			torrent->AddTracker(trackerUrl);
		}
	}
}

void CuteTorrent::StopSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::stop);
}

void CuteTorrent::setupGroupTreeWidget()
{
	m_pGroupTreeWidget->clear();
	QIcon torrentsIcon = styleEngine->getIcon("torrents");
	QIcon downloadingIcon = styleEngine->getIcon("downloading");
	QIcon seedingIcon = styleEngine->getIcon("uploading");
	QIcon completedIcon = styleEngine->getIcon("completed");
	QIcon activeIcon = styleEngine->getIcon("active");
	QIcon groupsIcon = styleEngine->getIcon("groups");
	QIcon inactiveIcon(activeIcon.pixmap(QSize(16, 16), QIcon::Disabled, QIcon::On));
	QIcon rssIcon = styleEngine->getIcon("rss");
	torrentTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	torrentTreeItem->setIcon(0, torrentsIcon);
	torrentTreeItem->setText(0, tr("TORRENTS_ACTIVITY"));
	torrentTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	torrentTreeItem->setData(0, Qt::UserRole + 2, TorrentStorrage::NONE);
	dlTreeItem = new QTreeWidgetItem(torrentTreeItem);
	dlTreeItem->setIcon(0, downloadingIcon);
	dlTreeItem->setText(0, tr("DOWNLOADING_FLTR"));
	dlTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	dlTreeItem->setData(0, Qt::UserRole + 2, TorrentStorrage::DOWNLOADING);
	ulTreeItem = new QTreeWidgetItem(torrentTreeItem);
	ulTreeItem->setIcon(0, seedingIcon);
	ulTreeItem->setText(0, tr("SEEDING_FLTR"));
	ulTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	ulTreeItem->setData(0, Qt::UserRole + 2, TorrentStorrage::SEEDING);
	completedTreeItem = new QTreeWidgetItem(torrentTreeItem);
	completedTreeItem->setIcon(0, completedIcon);
	completedTreeItem->setText(0, tr("COMPLETED_FLTR"));
	completedTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	completedTreeItem->setData(0, Qt::UserRole + 2, TorrentStorrage::COMPLETED);
	activeTreeItem = new QTreeWidgetItem(torrentTreeItem);
	activeTreeItem->setIcon(0, activeIcon);
	activeTreeItem->setText(0, tr("ACTIVE_FLTR"));
	activeTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	activeTreeItem->setData(0, Qt::UserRole + 2, TorrentStorrage::ACTIVE);
	inactiveTreeItem = new QTreeWidgetItem(torrentTreeItem);
	inactiveTreeItem->setIcon(0, inactiveIcon);
	inactiveTreeItem->setText(0, tr("NOT_ACTIVE_FLTR"));
	inactiveTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	inactiveTreeItem->setData(0, Qt::UserRole + 2, TorrentStorrage::NOT_ACTIVE);
	groupsTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	groupsTreeItem->setText(0, tr("TORRENT_GROUPS"));
	groupsTreeItem->setData(0, Qt::UserRole + 1, GROUP_FILTER_TYPE);
	groupsTreeItem->setIcon(0, groupsIcon);
	QList<GroupForFileFiltering> groups = settings->GetFileFilterGroups();
	QString type;
	m_pGroupTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	for(int i = 0; i < groups.count(); i++)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(groupsTreeItem);
		item->setText(0, groups[i].Name());
		item->setIcon(0, styleEngine->guessMimeIcon(groups[i].Extensions().split('|') [0], type));
		item->setData(0, Qt::UserRole + 1, GROUP_FILTER_TYPE);
		item->setData(0, Qt::UserRole + 2, groups[i].Name());
	}

	rssTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	rssTreeItem->setIcon(0, rssIcon);
	rssTreeItem->setText(0, tr("RSS_CHANELS"));
	rssTreeItem->setData(0, Qt::UserRole + 1, RSS_FEED);
	searchTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	searchTreeItem->setIcon(0, styleEngine->getIcon("settings_search"));
	searchTreeItem->setText(0, tr("TAB_SEARCH"));
	searchTreeItem->setData(0, Qt::UserRole + 1, SEARCH);
	searchTreeItem->setData(0, Qt::UserRole + 2, "");
	QList<ISerachProvider*>  searchProviders = m_pSearchEngine->GetSearchProviders();
	int nSearchProvidersCount = searchProviders.length();

	for(int i = 0; i < nSearchProvidersCount; i++)
	{
		ISerachProvider* current = searchProviders.at(i);
		QTreeWidgetItem* item = new QTreeWidgetItem(searchTreeItem);
		QString engineName = current->Name();
		item->setText(0, engineName);
		item->setIcon(0, styleEngine->getIcon(engineName));
		item->setData(0, Qt::UserRole + 1, SEARCH);
		item->setData(0, Qt::UserRole + 2, engineName);
	}

	m_pGroupTreeWidget->resizeColumnToContents(0);
	m_pGroupTreeWidget->expandAll();
}


void CuteTorrent::ChnageTorrentFilter()
{
	QTreeWidgetItem* item = m_pGroupTreeWidget->currentItem();
	FilterType filterType = (FilterType) item->data(0, Qt::UserRole + 1).toInt();

	switch(filterType)
	{
		case GROUP_FILTER_TYPE:
		{
			QString gropupFilter = item->data(0, Qt::UserRole + 2).toString();
			torrents->setGroupFilter(gropupFilter);
			switchToTorrentsModel();
			break;
		}

		case SEARCH:
		{
			QString engineName = item->data(0, Qt::UserRole + 2).toString();
			m_pSearchEngine->GetResults()->setFilter(engineName);
			switchToSearchModel();
			break;
		}

		case RSS_FEED:
			break;

		case TORRENT:
		{
			TorrentStorrage::TorrentFilterType torrentFilter = (TorrentStorrage::TorrentFilterType) item->data(0, Qt::UserRole + 2).toInt();
			torrents->setTorrentFilter(torrentFilter);
			switchToTorrentsModel();
			break;
		}

		case NONE:
			break;
	}

	m_pTorrentDisplayModel->updateVisibleTorrents();
}

void CuteTorrent::startBackUpWizard()
{
	BackupWizard* wizard = new BackupWizard(this);
	wizard->show();
}
void CuteTorrent::initMainMenuIcons()
{
	ACTION_MENU_ABAUT_CT->setIcon(styleEngine->getIcon("about"));
	ACTION_MENU_CHECK_UPDATE->setIcon(styleEngine->getIcon("check_update"));
	ACTION_MENU_CREATE->setIcon(styleEngine->getIcon("create_torrent"));
	ACTION_MENU_EXIT->setIcon(styleEngine->getIcon("menu_exit"));
	ACTION_MENU_OPEN_MAGNET->setIcon(styleEngine->getIcon("magnet"));
	ACTION_MENU_OPEN_TORRENT->setIcon(styleEngine->getIcon("add_torrent"));
	ACTION_MENU_SETTINGS->setIcon(styleEngine->getIcon("menu_settings"));
	ACTION_MENU_BACKUP->setIcon(styleEngine->getIcon("menu_backup"));
}

void CuteTorrent::initWindowIcons()
{
	pbMin->setIcon(styleEngine->getIcon("app_min"));
	pbMax->setIcon(styleEngine->getIcon("app_max"));
	pbClose->setIcon(styleEngine->getIcon("app_close"));
	setupGroupTreeWidget();
	initToolbarIcons();
	initStatusBarIcons();
	initMainMenuIcons();
}
void CuteTorrent::setupCustomeWindow()
{
	setupUi(this);
	BaseWindow::setupCustomWindow();
	QRect geo = settings->value("Window", "geometry", QRect(0, 0, 683, 643)).toRect();

	if(geo.height() > 0 && geo.x() < QApplication::desktop()->width() && geo.width() > 0 && geo.y() < QApplication::desktop()->height())
	{
		setGeometry(geo);
	}

	if(settings->value("Window", "maximized", false).toBool())
	{
		showMaximized();
		pbMax->setIcon(styleEngine->getIcon("app_rest"));
	}

	int selectedTab = settings->valueInt("Window", "selected_tab", 0);

	if(selectedTab >= 0 && selectedTab < tabWidget->count())
	{
		tabWidget->setCurrentIndex(selectedTab);
	}

	QList<int> horizontal_sizes = settings->value("Window", "horizontal_sizes", qVariantFromValue(QList<int>() << 130 << 538)).value<QList<int>>();

	if(horizontal_sizes.size() > 0)
	{
		spliiter1->setSizes(horizontal_sizes);
	}

	QList<int> vertical_sizes = settings->value("Window", "vertical_sizes", qVariantFromValue(QList<int>() << 530 << 195)).value<QList<int>>();

	if(vertical_sizes.size() > 0)
	{
		spliiter->setSizes(vertical_sizes);
	}
}

void CuteTorrent::setupKeyMappings()
{
	QList<QAction*> actions;
	actions.append(this->findChildren<QAction*>());
	QMap<QString, QVariant> keyMap = settings->getGroupValues("KeyMap");

	foreach(QAction* action, actions)
	{
		QString objName = action->objectName();

		if(!objName.isEmpty() && objName.startsWith("ACTION_") && !keyMap.contains(objName))
		{
			keyMap.insert(action->objectName(), action->shortcut().toString());
		}
		else
		{
			if(objName.startsWith("ACTION_") && keyMap.contains(objName))
			{
				action->setShortcut(QKeySequence(keyMap[objName].toString()));
			}
		}
	}

	settings->setGroupValues("KeyMap", keyMap);
	qDebug() << keyMap;
}


void CuteTorrent::resizeWindow(QMouseEvent* e)
{
	BaseWindow::resizeWindow(e);
	pieceView->update();
}

QPushButton* CuteTorrent::getMinBtn()
{
	return pbMin;
}

QPushButton* CuteTorrent::getMaxBtn()
{
	return pbMax;
}

QPushButton* CuteTorrent::getCloseBtn()
{
	return pbClose;
}

QWidget* CuteTorrent::getTitleBar()
{
	return titleBar;
}

QWidget* CuteTorrent::centralWidget()
{
	return m_centralWidget;
}

QLabel* CuteTorrent::getTitleLabel()
{
	return LTitle;
}

QLabel* CuteTorrent::getTitleIcon()
{
	return tbMenu;
}

void CuteTorrent::maximizeBtnClicked()
{
	BaseWindow::maximizeBtnClicked();
}

void CuteTorrent::minimizeBtnClicked()
{
	BaseWindow::minimizeBtnClicked();
}

void CuteTorrent::OnGotSerachResults()
{
	//ToDo: implement showing of search results
}

bool CuteTorrent::eventFilter(QObject* obj, QEvent* event)
{
	if(event->type() == QEvent::Resize)
	{
		resizeEvent(static_cast<QResizeEvent*>(event));
		return QObject::eventFilter(obj, event);
	}
	else
	{
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
}

void CuteTorrent::switchToTorrentsModel()
{
	if(m_pTorrentListView->model() != m_pTorrentDisplayModel)
	{
		m_pTorrentListView->setModel(m_pTorrentDisplayModel);
		m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
	}
}

void CuteTorrent::switchToSearchModel()
{
	if(m_pTorrentListView->model() != m_pSearchDisplayModel)
	{
		m_pTorrentListView->setModel(m_pSearchDisplayModel);
		m_pTorrentListView->setItemDelegate(m_pSearchItemDelegate);
	}
}

void CuteTorrent::switchToRssModel()
{
}
