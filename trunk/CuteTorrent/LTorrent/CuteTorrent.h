/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
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
#ifndef LTORRENT_H
#define LTORRENT_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QSpinBox>
#include <QMenuBar>
#include <QMenu>
#include <QString>
#include <QTimer>
#include "ui_CuteTorrent.h"
#include "ui_mainWindowButtons.h"
#include "SettingsDialog.h"
#include "TorrentManager.h"
#include "CreateTorrentDilaog.h"
#include "OpenTorrentDialog.h"
#include "QTorrentDisplayModel.h"
#include "QTorrentItemDelegat.h"
#include <qtsingleapplication.h>
#include <QSystemTrayIcon>
#include <QEvent>
#include <QCloseEvent>
#include "QTorrentListView.h"
#include "UpdateNotyfier.h"
#include <QMutex>
#include <QtNetwork/QHostAddress>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include "tracker/torrentracker.h"
#include "RconWebService.h"
#include "FileViewModel.h"
#include "TorrentStorrage.h"
Q_DECLARE_METATYPE(QHostAddress)
class CuteTorrent : public QMainWindow , private Ui::CuteTorrentClass
{
	Q_OBJECT

public:
	CuteTorrent(QWidget *parent = 0, Qt::WFlags flags = 0);

	

	void ConnectMessageReceved(QtSingleApplication* a);
	
    ~CuteTorrent();
protected:
	
	void changeEvent(QEvent *event);
	void resizeEvent ( QResizeEvent * event );
	void closeEvent(QCloseEvent* ce);
	void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
	void keyPressEvent ( QKeyEvent * event );
private:
	QComboBox* searchSource;
	TorrentStorrage* torrents;
	QSortFilterProxyModel* proxymodel;
    FileViewModel* fileViewModel;
	Ui::ToolButtons* toolButtonsUi;
    TorrentTracker* tracker;
	QApplicationSettings* settings;
	RconWebService* rcon;
	int  m_nMouseClick_X_Coordinate;
	int  m_nMouseClick_Y_Coordinate;
	

	UpdateNotifier* notyfire;
	bool mayShowNotifies;
	QSystemTrayIcon *trayIcon;
	QSpinBox* ul,*dl;
	QMenu *trayIconMenu;
	QAction* minimizeAction;
    QAction* maximizeAction;
    QAction* restoreAction;
    QAction* quitAction;
	QAction* copyContext;
	QAction* addPeer,* addTracker;
	QLabel *upLabelText, *upLabel,*title;
	QLabel *uploadLimit, *downloadLimit;
	QLabel* downLabelText, *downLabel;
	QTorrentDisplayModel* model;
	TorrentManager* mng;
	QTimer *timer;
	QMenu* fileTabMenu;
	QAction* openFile;
	QAction* openDir;
	QMenu* priority;
	QAction* lowPriority;
	QAction* mediumPriority;
	QAction* highPriority;
	QAction* dontDownload;
	QLineEdit* searchEdit;
	QTreeWidgetItem *__qtreewidgetitem;
	QTreeWidgetItem *__qtreewidgetitem1;
	QTreeWidgetItem *__qtreewidgetitem2;
	QTreeWidgetItem *__qtreewidgetitem3;
	QTreeWidgetItem *__qtreewidgetitem4;
	QTreeWidgetItem *__qtreewidgetitem5;
	QTreeWidgetItem *__qtreewidgetitem6;
	void createTrayIcon();
	void createActions();
	void setupTray();
	void setupToolBar();
	void setupConnections();
	void setupListView();
	void setupTabelWidgets();
	void setupStatusBar();
	void setupFileTabel();
	void setupGroupTreeWidget();
	void setupFileTabelContextMenu();
	void fillPieceDisplay();
	void setFilePriority(int);
public slots:
	void HandleNewTorrent(const QString &);
private slots:
	void UpdateUL(int);
	void UpdateDL(int);
	void OpenFileSelected();
	void OpenDirSelected();
	void SetLowForCurrentFile();
	void SetMediumForCurrentFile();
	void SetHighForCurrentFile();
	void SetNotDownloadForCurrentFile();
	void FileTabContextMenu(const QPoint &);
	void ShowAbout();
	void CheckForUpdates();
	void ShowUpdateNitify(const QString&);
	void ShowNoUpdateNitify(const QString&);
	void Retranslate();
	void ShowTorrentError(const QString&,const QString&);
	void EnableNitifyShow();
	void ShowTorrentCompletedNotyfy(const QString,const QString);
	void ShowTorrentInfoNotyfy(const QString,const QString);
	void ShowCreateTorrentDialog();
	void ShowOpenTorrentDialog();
	void PauseSelected();
	void StopSelected();
	void ResumeSelected();
	void DeleteSelected();
	void UpdateInfoTab();
	void UpdatePeerTab();
	void UpdateFileTab();
	void UpadteTrackerTab();
	void OpenSettingsDialog();
	void IconActivated(QSystemTrayIcon::ActivationReason reason);
	void UpdateTabWidget(int);
	void ProcessMagnet();
	void PeformSearch();
	void CopyDiscribtion();
	void ClearPieceDisplay();
	void UpdateItemWidth(int,int);
	void AddTracker();
	void AddPeer();
	void ChnageTorrentFilter();


};

#endif // LTORRENT_H
