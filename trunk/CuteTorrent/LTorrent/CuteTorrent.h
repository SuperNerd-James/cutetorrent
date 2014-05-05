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
#include "messagebox.h"
#include <QFileDialog>
#include <QSpinBox>
#include <QMenuBar>
#include <QMenu>
#include <QString>
#include <QTimer>
#include "application.h"
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
#include <QDesktopWidget>
#include <QMutex>
#include <QtNetwork/QHostAddress>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include "tracker/torrentracker.h"
#include "webControll/RconWebService.h"
#include "FileViewModel.h"
#include "TorrentStorrage.h"
#include "searchitem.h"
#include "peicedisplaywidget.h"
#include "StyleEngene.h"
#include "ui_CustomWindow.h"
#define PIXELS_TO_ACT 2
Q_DECLARE_METATYPE(QHostAddress)
class CuteTorrent : public QWidget , private Ui::CustomWindow
{
    Q_OBJECT

public:
    enum TitleMode { CleanTitle = 0, OnlyCloseButton, MenuOff, MaxMinOff, FullScreenMode, MaximizeModeOff, MinimizeModeOff, FullTitle };
    CuteTorrent(QWidget *parent = 0);



    void ConnectMessageReceved(Application* a);

    ~CuteTorrent();
protected:

    void changeEvent(QEvent *event);
    void resizeEvent ( QResizeEvent * event );
    void closeEvent(QCloseEvent* ce);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent ( QKeyEvent * event );

private:
    /**
         * @brief centralLayout Main layout where the centralwidget will be set.
         */
    QHBoxLayout *centralLayout;
    /**
         * @brief dragPosition Increment of the position movement.
         */
    QPoint dragPosition;
    /**
         * @brief m_titleMode Flags that defines the current titlebar mode.
         */
    TitleMode m_titleMode;
    /**
         * @brief moveWidget Specifies if the window is in move action.
         */
    bool moveWidget;
    /**
         * @brief inResizeZone Specifies if the mouse is in resize zone.
         */
    bool inResizeZone;
    /**
         * @brief allowToResize Specifies if the mouse is allowed to resize.
         */
    bool allowToResize;
    /**
         * @brief resizeVerSup Specifies if the resize is in the top of the window.
         */
    bool resizeVerSup;
    /**
         * @brief resizeHorEsq Specifies if the resize is in the left of the window.
         */
    bool resizeHorEsq;
    /**
         * @brief resizeDiagSupEsq Specifies if the resize is in the top left of the window.
         */
    bool resizeDiagSupEsq;
    /**
         * @brief resizeDiagSupDer Specifies if the resize is in the top right of the window.
         */
    bool resizeDiagSupDer;

    /**
         * @brief mouseMoveEvent Overloaded member that moves of resizes depending of the
         * configuration sotred at mousePressEvent().
         * @param e The mouse event.
         */
    void mouseMoveEvent(QMouseEvent *e);
    /**
         * @brief mousePressEvent Overloaded member that stores that changes the cursor and
         * configures the side that is changing.
         * @param e The mouse event.
         */
    void mousePressEvent(QMouseEvent *e);
    /**
         * @brief mouseReleaseEvent Overloaded member that removes the configuration set in mousePressEvent().
         * @param e The mouse event.
         */
    void mouseReleaseEvent(QMouseEvent *e);
    /**
         * @brief mouseDoubleClickEvent Overloadad member that maximize/restore the window if is
         * doubleclicked and the position of the mouse is not the top left of the window (menu zone).
         * @param e The mouse event.
         */
    void mouseDoubleClickEvent(QMouseEvent *e);
    /**
         * @brief paintEvent Overloaded method that allows to customize the styles of the window.
         */
    void paintEvent (QPaintEvent *);
    /**
         * @brief resizeWindow Method that calculates the resize and new position of the window an
         * does this actions.
         * @param e The mouse event to calculate the new position and size.
         */
    void resizeWindow(QMouseEvent *e);

    void moveWindow(QMouseEvent *e);
    void initToolbarIcons();
    void initStatusBarIcons();
    void initMainMenuIcons();
    StyleEngene* styleEngine;
    QList<SearchItem> searchSources;
    QComboBox* searchSource;
    TorrentStorrage* torrents;
    PeiceDisplayWidget* pieceView;
    QSortFilterProxyModel* proxymodel;
    FileViewModel* fileViewModel;
    TorrentTracker* tracker;
    QApplicationSettings* settings;
    RconWebService* rcon;
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
    TorrentManager* tManager;
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
    void fillPieceDisplay(QSize);
    void setFilePriority(int);
    void setupCustomeWindow();
    void setupKeyMappings();
public slots:
    void HandleNewTorrent(const QString &);
    /**
         * @brief maximizeBtnClicked Maximizes or restores the window depending on the last status.
         */

    void maximizeBtnClicked();
    /**
         * @brief minimizeBtnClicked Minimizes or restores the window depending on the last status.
         */
    void minimizeBtnClicked();

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
    void ShowTorrentCompletedNotyfy(const QString&,const QString&);
    void ShowTorrentInfoNotyfy(const QString&,const QString&);
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
    void initWindowIcons();
    void AddTracker();
    void AddPeer();
    void ChnageTorrentFilter();
    void startBackUpWizard();


};

#endif // LTORRENT_H
