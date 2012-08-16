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
#ifndef LTORRENT_H
#define LTORRENT_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QTimer>
#include "ui_CuteTorrent.h"
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
class CuteTorrent : public QMainWindow , private Ui::CuteTorrentClass
{
	Q_OBJECT

public:
	CuteTorrent(QWidget *parent = 0, Qt::WFlags flags = 0);
	void ConnectMessageReceved(QtSingleApplication* a);
	
	~CuteTorrent();
protected:
	
	void changeEvent(QEvent *event);
	void closeEvent(QCloseEvent* ce);
	/*void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);*/
private:
	bool mayShowNotifies;
	QSystemTrayIcon *trayIcon;
	QMenu *trayIconMenu;
	QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
	QLabel* upLabelText;
	QLabel* downLabelText;
	QTorrentDisplayModel* model;
	TorrentManager* mng;
	
	
	void createTrayIcon();
	void createActions();
	void showMessage();
	void setupTray();
	void setupToolBar();
	void setupTimer();
	void setupConnections();
	void setupListView();
	void SetupListViewActions();
	void setupTabelWidgets();
	void setupStatusBar();
	
public slots:
	void HandleNewTorrent(const QString &);
private slots:
	void ShowTorrentError(const QString&);
	void enableNitifyShow();
	void showTorrentCompletedNotyfy(const QString);
	void ShowCreateTorrentDialog();
	void ShowOpenTorrentDialog();
	//To DO: reimplement to torrentChanged signal
	void updateVisibleTorrents();
	void PauseSelected();
	void ResumeSelected();
	void DeleteSelected();
	void UpdateInfoTab();
	void UpdatePeerTab();
	void UpadteTrackerTab();
	void OpenSettingsDialog();
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
	void messageClicked();
	void updateTabWidget(int);
	
};

#endif // LTORRENT_H
