#ifndef LTORRENT_H
#define LTORRENT_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QTimer>
#include "ui_ltorrent.h"
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
class LTorrent : public QMainWindow , private Ui::LTorrentClass
{
	Q_OBJECT

public:
	LTorrent(QWidget *parent = 0, Qt::WFlags flags = 0);
	void ConnectMessageReceved(QtSingleApplication* a);
	
	~LTorrent();
protected:
	void changeEvent(QEvent *event);
	void closeEvent(QCloseEvent* ce);
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
