/********************************************************************************
** Form generated from reading UI file 'CustomWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMWINDOW_H
#define UI_CUSTOMWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomWindow
{
public:
    QAction* ACTION_MENU_OPEN_TORRENT;
    QAction* ACTION_MENU_OPEN_MAGNET;
    QAction* ACTION_MENU_CREATE;
    QAction* ACTION_MENU_EXIT;
    QAction* ACTION_MENU_SETTINGS;
    QAction* ACTION_TOOLBAR_PAUSE;
    QAction* ACTION_TOOLBAR_START;
    QAction* ACTION_TOOLBAR_REMOVE;
    QAction* ACTION_MENU_ABAUT_CT;
    QAction* ACTION_MENU_CHECK_UPDATE;
    QAction* ACTION_TOOLBAR_SETTINGS;
    QAction* ACTION_MENU_BACKUP;
    QVBoxLayout* verticalLayout;
    QWidget* titleBar;
    QHBoxLayout* horizontalLayout;
    QLabel* tbMenu;
    QLabel* LTitle;
    QPushButton* pbMin;
    QPushButton* pbMax;
    QPushButton* pbClose;
    QWidget* m_centralWidget;
    QGridLayout* gridLayout_3;
    QToolBar* mainToolbar;
    QStatusBar* mystatusBar;
    QMenuBar* mainMenuBar;
    QMenu* menu;
    QMenu* menu_2;
    QMenu* menu_CuteTorrent;
    QMenu* menuMENU_TOOLS;
    QGridLayout* gridLayout_6;
    QSplitter* spliiter;
    QSplitter* spliiter1;
    QTreeWidget* m_pGroupTreeWidget;
    QListView* m_pTorrentListView;
    QTabWidget* tabWidget;
    QWidget* tab;
    QGridLayout* gridLayout_4;
    QLabel* label_11;
    QLabel* pathLabel;
    QLabel* label_8;
    QWidget* widget;
    QLabel* label_10;
    QLabel* uploadSpeedLabel;
    QLabel* uploadedBytesLabel;
    QLabel* label_6;
    QLabel* seedCoutLabel;
    QLabel* activetimeLabel;
    QLabel* label_4;
    QLabel* describtionLabel;
    QLabel* totalSizeLabel;
    QLabel* label_7;
    QLabel* timeleftLabel;
    QLabel* label_2;
    QLabel* label;
    QLabel* downloadedBytesLabel;
    QLabel* label_3;
    QLabel* peerCoutLabel;
    QLabel* label_5;
    QLabel* downloadSpeedLabel;
    QLabel* label_9;
    QWidget* tab_3;
    QGridLayout* gridLayout_2;
    QTableWidget* peerTableWidget;
    QWidget* tab_2;
    QGridLayout* gridLayout;
    QTableWidget* trackerTableWidget;
    QWidget* tab_4;
    QGridLayout* gridLayout_5;
    QTreeView* fileTableView;

    void setupUi(QWidget* CustomWindow)
    {
        if(CustomWindow->objectName().isEmpty())
        {
            CustomWindow->setObjectName(QString::fromUtf8("CustomWindow"));
        }

        CustomWindow->setWindowModality(Qt::ApplicationModal);
        CustomWindow->resize(532, 620);
        CustomWindow->setStyleSheet(QString::fromUtf8(""));
        ACTION_MENU_OPEN_TORRENT = new QAction(CustomWindow);
        ACTION_MENU_OPEN_TORRENT->setObjectName(QString::fromUtf8("ACTION_MENU_OPEN_TORRENT"));
        ACTION_MENU_OPEN_TORRENT->setShortcutContext(Qt::WindowShortcut);
        ACTION_MENU_OPEN_TORRENT->setSoftKeyRole(QAction::NoSoftKey);
        ACTION_MENU_OPEN_MAGNET = new QAction(CustomWindow);
        ACTION_MENU_OPEN_MAGNET->setObjectName(QString::fromUtf8("ACTION_MENU_OPEN_MAGNET"));
        ACTION_MENU_CREATE = new QAction(CustomWindow);
        ACTION_MENU_CREATE->setObjectName(QString::fromUtf8("ACTION_MENU_CREATE"));
        ACTION_MENU_EXIT = new QAction(CustomWindow);
        ACTION_MENU_EXIT->setObjectName(QString::fromUtf8("ACTION_MENU_EXIT"));
        ACTION_MENU_EXIT->setMenuRole(QAction::QuitRole);
        ACTION_MENU_SETTINGS = new QAction(CustomWindow);
        ACTION_MENU_SETTINGS->setObjectName(QString::fromUtf8("ACTION_MENU_SETTINGS"));
        ACTION_MENU_SETTINGS->setMenuRole(QAction::PreferencesRole);
        ACTION_TOOLBAR_PAUSE = new QAction(CustomWindow);
        ACTION_TOOLBAR_PAUSE->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_PAUSE"));
        ACTION_TOOLBAR_START = new QAction(CustomWindow);
        ACTION_TOOLBAR_START->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_START"));
        ACTION_TOOLBAR_REMOVE = new QAction(CustomWindow);
        ACTION_TOOLBAR_REMOVE->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_REMOVE"));
        ACTION_MENU_ABAUT_CT = new QAction(CustomWindow);
        ACTION_MENU_ABAUT_CT->setObjectName(QString::fromUtf8("ACTION_MENU_ABAUT_CT"));
        ACTION_MENU_ABAUT_CT->setMenuRole(QAction::AboutRole);
        ACTION_MENU_CHECK_UPDATE = new QAction(CustomWindow);
        ACTION_MENU_CHECK_UPDATE->setObjectName(QString::fromUtf8("ACTION_MENU_CHECK_UPDATE"));
        ACTION_TOOLBAR_SETTINGS = new QAction(CustomWindow);
        ACTION_TOOLBAR_SETTINGS->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_SETTINGS"));
        ACTION_MENU_BACKUP = new QAction(CustomWindow);
        ACTION_MENU_BACKUP->setObjectName(QString::fromUtf8("ACTION_MENU_BACKUP"));
        verticalLayout = new QVBoxLayout(CustomWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 2);
        titleBar = new QWidget(CustomWindow);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setMinimumSize(QSize(0, 23));
        titleBar->setMaximumSize(QSize(16777215, 23));
        titleBar->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(titleBar);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 0, 1, 0);
        tbMenu = new QLabel(titleBar);
        tbMenu->setObjectName(QString::fromUtf8("tbMenu"));
        tbMenu->setMaximumSize(QSize(15, 15));
        tbMenu->setPixmap(QPixmap(QString::fromUtf8(":/icons/app.ico")));
        tbMenu->setScaledContents(true);
        horizontalLayout->addWidget(tbMenu);
        LTitle = new QLabel(titleBar);
        LTitle->setObjectName(QString::fromUtf8("LTitle"));
        LTitle->setMinimumSize(QSize(100, 22));
        LTitle->setMaximumSize(QSize(16777215, 22));
        LTitle->setStyleSheet(QString::fromUtf8(""));
        LTitle->setTextFormat(Qt::AutoText);
        horizontalLayout->addWidget(LTitle);
        pbMin = new QPushButton(titleBar);
        pbMin->setObjectName(QString::fromUtf8("pbMin"));
        pbMin->setMinimumSize(QSize(22, 22));
        pbMin->setMaximumSize(QSize(22, 22));
        pbMin->setIconSize(QSize(22, 22));
        pbMin->setFlat(true);
        horizontalLayout->addWidget(pbMin);
        pbMax = new QPushButton(titleBar);
        pbMax->setObjectName(QString::fromUtf8("pbMax"));
        pbMax->setMinimumSize(QSize(22, 22));
        pbMax->setMaximumSize(QSize(22, 22));
        pbMax->setIconSize(QSize(22, 22));
        pbMax->setFlat(true);
        horizontalLayout->addWidget(pbMax);
        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));
        pbClose->setMinimumSize(QSize(22, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);
        horizontalLayout->addWidget(pbClose);
        verticalLayout->addWidget(titleBar);
        m_centralWidget = new QWidget(CustomWindow);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        m_centralWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(m_centralWidget);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 1, 6, 6);
        mainToolbar = new QToolBar(m_centralWidget);
        mainToolbar->setObjectName(QString::fromUtf8("mainToolbar"));
        mainToolbar->setEnabled(true);
        mainToolbar->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setKerning(true);
        mainToolbar->setFont(font);
        mainToolbar->setContextMenuPolicy(Qt::NoContextMenu);
        mainToolbar->setStyleSheet(QString::fromUtf8("QToolBar {\n"
                                   "border-bottom: none;\n"
                                   "border-top: none;\n"
                                   "spacing:3px;\n"
                                   "padding-right:2px;\n"
                                   "}\n"
                                   ""));
        mainToolbar->setMovable(false);
        mainToolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
        gridLayout_3->addWidget(mainToolbar, 2, 0, 1, 1);
        mystatusBar = new QStatusBar(m_centralWidget);
        mystatusBar->setObjectName(QString::fromUtf8("mystatusBar"));
        mystatusBar->setMaximumSize(QSize(16777215, 19));
        mystatusBar->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3->addWidget(mystatusBar, 4, 0, 1, 1);
        mainMenuBar = new QMenuBar(m_centralWidget);
        mainMenuBar->setObjectName(QString::fromUtf8("mainMenuBar"));
        mainMenuBar->setEnabled(true);
        mainMenuBar->setMinimumSize(QSize(0, 0));
        mainMenuBar->setMaximumSize(QSize(16777215, 16777215));
        mainMenuBar->setStyleSheet(QString::fromUtf8(""));
        mainMenuBar->setDefaultUp(false);
        mainMenuBar->setNativeMenuBar(false);
        menu = new QMenu(mainMenuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setStyleSheet(QString::fromUtf8(""));
        menu_2 = new QMenu(mainMenuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_CuteTorrent = new QMenu(mainMenuBar);
        menu_CuteTorrent->setObjectName(QString::fromUtf8("menu_CuteTorrent"));
        menuMENU_TOOLS = new QMenu(mainMenuBar);
        menuMENU_TOOLS->setObjectName(QString::fromUtf8("menuMENU_TOOLS"));
        gridLayout_3->addWidget(mainMenuBar, 1, 0, 1, 1);
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(0);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, -1, 0, -1);
        spliiter = new QSplitter(m_centralWidget);
        spliiter->setObjectName(QString::fromUtf8("spliiter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spliiter->sizePolicy().hasHeightForWidth());
        spliiter->setSizePolicy(sizePolicy);
        spliiter->setMinimumSize(QSize(0, 330));
        spliiter->setStyleSheet(QString::fromUtf8(""));
        spliiter->setOrientation(Qt::Vertical);
        spliiter->setOpaqueResize(true);
        spliiter1 = new QSplitter(spliiter);
        spliiter1->setObjectName(QString::fromUtf8("spliiter1"));
        sizePolicy.setHeightForWidth(spliiter1->sizePolicy().hasHeightForWidth());
        spliiter1->setSizePolicy(sizePolicy);
        spliiter1->setMinimumSize(QSize(0, 330));
        spliiter1->setStyleSheet(QString::fromUtf8(""));
        spliiter1->setOrientation(Qt::Horizontal);
        spliiter1->setOpaqueResize(true);
        m_pGroupTreeWidget = new QTreeWidget(spliiter1);
        QTreeWidgetItem* __qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        m_pGroupTreeWidget->setHeaderItem(__qtreewidgetitem);
        m_pGroupTreeWidget->setObjectName(QString::fromUtf8("m_pGroupTreeWidget"));
        m_pGroupTreeWidget->setHeaderHidden(true);
        spliiter1->addWidget(m_pGroupTreeWidget);
        m_pTorrentListView = new QListView(spliiter1);
        m_pTorrentListView->setObjectName(QString::fromUtf8("m_pTorrentListView"));
        spliiter1->addWidget(m_pTorrentListView);
        spliiter->addWidget(spliiter1);
        tabWidget = new QTabWidget(spliiter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setStyleSheet(QString::fromUtf8(""));
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setElideMode(Qt::ElideMiddle);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_4 = new QGridLayout(tab);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        gridLayout_4->addWidget(label_11, 7, 3, 1, 1);
        pathLabel = new QLabel(tab);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));
        gridLayout_4->addWidget(pathLabel, 6, 4, 1, 1);
        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        gridLayout_4->addWidget(label_8, 2, 3, 1, 1);
        widget = new QWidget(tab);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(0, 20));
        widget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_4->addWidget(widget, 0, 0, 1, 5);
        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        gridLayout_4->addWidget(label_10, 7, 0, 1, 1);
        uploadSpeedLabel = new QLabel(tab);
        uploadSpeedLabel->setObjectName(QString::fromUtf8("uploadSpeedLabel"));
        gridLayout_4->addWidget(uploadSpeedLabel, 3, 4, 1, 1);
        uploadedBytesLabel = new QLabel(tab);
        uploadedBytesLabel->setObjectName(QString::fromUtf8("uploadedBytesLabel"));
        gridLayout_4->addWidget(uploadedBytesLabel, 2, 4, 1, 1);
        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        gridLayout_4->addWidget(label_6, 4, 0, 1, 1);
        seedCoutLabel = new QLabel(tab);
        seedCoutLabel->setObjectName(QString::fromUtf8("seedCoutLabel"));
        gridLayout_4->addWidget(seedCoutLabel, 7, 1, 1, 2);
        activetimeLabel = new QLabel(tab);
        activetimeLabel->setObjectName(QString::fromUtf8("activetimeLabel"));
        gridLayout_4->addWidget(activetimeLabel, 1, 1, 1, 2);
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        gridLayout_4->addWidget(label_4, 6, 3, 1, 1);
        describtionLabel = new QLabel(tab);
        describtionLabel->setObjectName(QString::fromUtf8("describtionLabel"));
        describtionLabel->setMaximumSize(QSize(16777215, 16));
        describtionLabel->setContextMenuPolicy(Qt::ActionsContextMenu);
        describtionLabel->setWordWrap(true);
        gridLayout_4->addWidget(describtionLabel, 4, 1, 1, 4);
        totalSizeLabel = new QLabel(tab);
        totalSizeLabel->setObjectName(QString::fromUtf8("totalSizeLabel"));
        gridLayout_4->addWidget(totalSizeLabel, 6, 1, 1, 2);
        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        gridLayout_4->addWidget(label_7, 1, 3, 1, 1);
        timeleftLabel = new QLabel(tab);
        timeleftLabel->setObjectName(QString::fromUtf8("timeleftLabel"));
        gridLayout_4->addWidget(timeleftLabel, 1, 4, 1, 1);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        gridLayout_4->addWidget(label_2, 2, 0, 1, 1);
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        gridLayout_4->addWidget(label, 1, 0, 1, 1);
        downloadedBytesLabel = new QLabel(tab);
        downloadedBytesLabel->setObjectName(QString::fromUtf8("downloadedBytesLabel"));
        gridLayout_4->addWidget(downloadedBytesLabel, 2, 1, 1, 2);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        gridLayout_4->addWidget(label_3, 3, 0, 1, 1);
        peerCoutLabel = new QLabel(tab);
        peerCoutLabel->setObjectName(QString::fromUtf8("peerCoutLabel"));
        gridLayout_4->addWidget(peerCoutLabel, 7, 4, 1, 1);
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        gridLayout_4->addWidget(label_5, 6, 0, 1, 1);
        downloadSpeedLabel = new QLabel(tab);
        downloadSpeedLabel->setObjectName(QString::fromUtf8("downloadSpeedLabel"));
        gridLayout_4->addWidget(downloadSpeedLabel, 3, 1, 1, 2);
        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        gridLayout_4->addWidget(label_9, 3, 3, 1, 1);
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_2 = new QGridLayout(tab_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        peerTableWidget = new QTableWidget(tab_3);

        if(peerTableWidget->columnCount() < 7)
        {
            peerTableWidget->setColumnCount(7);
        }

        QTableWidgetItem* __qtablewidgetitem = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem* __qtablewidgetitem1 = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem* __qtablewidgetitem2 = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem* __qtablewidgetitem3 = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem* __qtablewidgetitem4 = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem* __qtablewidgetitem5 = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem* __qtablewidgetitem6 = new QTableWidgetItem();
        peerTableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        peerTableWidget->setObjectName(QString::fromUtf8("peerTableWidget"));
        peerTableWidget->setMinimumSize(QSize(0, 10));
        peerTableWidget->setMaximumSize(QSize(16777215, 16777215));
        peerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        peerTableWidget->setGridStyle(Qt::NoPen);
        peerTableWidget->setSortingEnabled(true);
        peerTableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        peerTableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        peerTableWidget->horizontalHeader()->setStretchLastSection(true);
        gridLayout_2->addWidget(peerTableWidget, 0, 0, 1, 1);
        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        trackerTableWidget = new QTableWidget(tab_2);

        if(trackerTableWidget->columnCount() < 3)
        {
            trackerTableWidget->setColumnCount(3);
        }

        QTableWidgetItem* __qtablewidgetitem7 = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem7);
        QTableWidgetItem* __qtablewidgetitem8 = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem8);
        QTableWidgetItem* __qtablewidgetitem9 = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem9);
        trackerTableWidget->setObjectName(QString::fromUtf8("trackerTableWidget"));
        trackerTableWidget->setMaximumSize(QSize(16777215, 16777215));
        trackerTableWidget->setLayoutDirection(Qt::LeftToRight);
        trackerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        trackerTableWidget->setGridStyle(Qt::NoPen);
        trackerTableWidget->setSortingEnabled(true);
        trackerTableWidget->horizontalHeader()->setCascadingSectionResizes(true);
        trackerTableWidget->horizontalHeader()->setDefaultSectionSize(120);
        trackerTableWidget->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        trackerTableWidget->horizontalHeader()->setStretchLastSection(true);
        gridLayout->addWidget(trackerTableWidget, 0, 0, 1, 1);
        tabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_5 = new QGridLayout(tab_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        fileTableView = new QTreeView(tab_4);
        fileTableView->setObjectName(QString::fromUtf8("fileTableView"));
        fileTableView->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_5->addWidget(fileTableView, 0, 0, 1, 1);
        tabWidget->addTab(tab_4, QString());
        spliiter->addWidget(tabWidget);
        gridLayout_6->addWidget(spliiter, 0, 0, 1, 2);
        gridLayout_3->addLayout(gridLayout_6, 3, 0, 1, 1);
        verticalLayout->addWidget(m_centralWidget);
        mainToolbar->addAction(ACTION_TOOLBAR_START);
        mainToolbar->addAction(ACTION_TOOLBAR_PAUSE);
        mainToolbar->addAction(ACTION_TOOLBAR_REMOVE);
        mainToolbar->addAction(ACTION_TOOLBAR_SETTINGS);
        mainMenuBar->addAction(menu->menuAction());
        mainMenuBar->addAction(menu_2->menuAction());
        mainMenuBar->addAction(menuMENU_TOOLS->menuAction());
        mainMenuBar->addAction(menu_CuteTorrent->menuAction());
        menu->addAction(ACTION_MENU_OPEN_TORRENT);
        menu->addAction(ACTION_MENU_OPEN_MAGNET);
        menu->addSeparator();
        menu->addAction(ACTION_MENU_CREATE);
        menu->addSeparator();
        menu->addAction(ACTION_MENU_EXIT);
        menu_2->addAction(ACTION_MENU_SETTINGS);
        menu_CuteTorrent->addAction(ACTION_MENU_ABAUT_CT);
        menu_CuteTorrent->addAction(ACTION_MENU_CHECK_UPDATE);
        menuMENU_TOOLS->addAction(ACTION_MENU_BACKUP);
        retranslateUi(CustomWindow);
        QObject::connect(ACTION_MENU_CREATE, SIGNAL(triggered()), CustomWindow, SLOT(ShowCreateTorrentDialog()));
        QObject::connect(ACTION_MENU_OPEN_TORRENT, SIGNAL(triggered()), CustomWindow, SLOT(ShowOpenTorrentDialog()));
        QObject::connect(ACTION_TOOLBAR_START, SIGNAL(triggered()), CustomWindow, SLOT(ResumeSelected()));
        QObject::connect(ACTION_TOOLBAR_PAUSE, SIGNAL(triggered()), CustomWindow, SLOT(PauseSelected()));
        QObject::connect(ACTION_TOOLBAR_REMOVE, SIGNAL(triggered()), CustomWindow, SLOT(DeleteSelected()));
        QObject::connect(ACTION_MENU_SETTINGS, SIGNAL(triggered()), CustomWindow, SLOT(OpenSettingsDialog()));
        QObject::connect(ACTION_MENU_CHECK_UPDATE, SIGNAL(triggered()), CustomWindow, SLOT(CheckForUpdates()));
        QObject::connect(ACTION_MENU_ABAUT_CT, SIGNAL(triggered()), CustomWindow, SLOT(ShowAbout()));
        QObject::connect(ACTION_MENU_OPEN_MAGNET, SIGNAL(triggered()), CustomWindow, SLOT(ProcessMagnet()));
        QObject::connect(ACTION_TOOLBAR_SETTINGS, SIGNAL(triggered()), CustomWindow, SLOT(OpenSettingsDialog()));
        QObject::connect(ACTION_MENU_BACKUP, SIGNAL(triggered()), CustomWindow, SLOT(startBackUpWizard()));
        tabWidget->setCurrentIndex(0);
        QMetaObject::connectSlotsByName(CustomWindow);
    } // setupUi

    void retranslateUi(QWidget* CustomWindow)
    {
        CustomWindow->setWindowTitle(QApplication::translate("CustomWindow", "CuteTorrent", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_TORRENT->setText(QApplication::translate("CustomWindow", "MENU_OPEN_TORRENT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_TORRENT->setShortcut(QApplication::translate("CustomWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_MAGNET->setText(QApplication::translate("CustomWindow", "MENU_OPEN_MAGNET", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_MAGNET->setShortcut(QApplication::translate("CustomWindow", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CREATE->setText(QApplication::translate("CustomWindow", "MENU_CREATE_TORRENT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CREATE->setShortcut(QApplication::translate("CustomWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_EXIT->setText(QApplication::translate("CustomWindow", "MENU_EXIT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_EXIT->setShortcut(QApplication::translate("CustomWindow", "Alt+X", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_SETTINGS->setText(QApplication::translate("CustomWindow", "MENU_CONFIGURATION", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_SETTINGS->setShortcut(QApplication::translate("CustomWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_PAUSE->setText(QApplication::translate("CustomWindow", "STR_PAUSE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_PAUSE->setIconText(QApplication::translate("CustomWindow", "HINT_PAUSE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_START->setText(QApplication::translate("CustomWindow", "STR_START", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_START->setIconText(QApplication::translate("CustomWindow", "HINT_RESUME", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_REMOVE->setText(QApplication::translate("CustomWindow", "STR_REMOVE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_REMOVE->setIconText(QApplication::translate("CustomWindow", "HINT_REMOVE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ACTION_TOOLBAR_REMOVE->setToolTip(QApplication::translate("CustomWindow", "STR_REMOVE", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ACTION_MENU_ABAUT_CT->setText(QApplication::translate("CustomWindow", "MENU_ABAUT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_ABAUT_CT->setShortcut(QApplication::translate("CustomWindow", "F1", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CHECK_UPDATE->setText(QApplication::translate("CustomWindow", "MENU_CHECK_UPDATES", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CHECK_UPDATE->setShortcut(QApplication::translate("CustomWindow", "Ctrl+U", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_SETTINGS->setText(QApplication::translate("CustomWindow", "STR_SETTINGS", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_BACKUP->setText(QApplication::translate("CustomWindow", "TOOLS_BACKUP", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("CustomWindow", "CuteTorrent", 0, QApplication::UnicodeUTF8));
        mainToolbar->setWindowTitle(QApplication::translate("CustomWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("CustomWindow", "MENU_FILE", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("CustomWindow", "MENU_SETTINGS", 0, QApplication::UnicodeUTF8));
        menu_CuteTorrent->setTitle(QApplication::translate("CustomWindow", "MENU_HELP", 0, QApplication::UnicodeUTF8));
        menuMENU_TOOLS->setTitle(QApplication::translate("CustomWindow", "MENU_TOOLS", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CustomWindow", "INFO_PEER_COUNT", 0, QApplication::UnicodeUTF8));
        pathLabel->setText(QString());
        label_8->setText(QApplication::translate("CustomWindow", "INFO_UPLOADED", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("CustomWindow", "INFO_SEED_COUNT", 0, QApplication::UnicodeUTF8));
        uploadSpeedLabel->setText(QString());
        uploadedBytesLabel->setText(QString());
        label_6->setText(QApplication::translate("CustomWindow", "INFO_DESCRIBTION", 0, QApplication::UnicodeUTF8));
        seedCoutLabel->setText(QString());
        activetimeLabel->setText(QString());
        label_4->setText(QApplication::translate("CustomWindow", "INFO_PATH", 0, QApplication::UnicodeUTF8));
        describtionLabel->setText(QString());
        totalSizeLabel->setText(QString());
        label_7->setText(QApplication::translate("CustomWindow", "INFO_REMAININF_TIME", 0, QApplication::UnicodeUTF8));
        timeleftLabel->setText(QString());
        label_2->setText(QApplication::translate("CustomWindow", "INFO_DOWNLOADED", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CustomWindow", "INFO_ACTIVE_TIME", 0, QApplication::UnicodeUTF8));
        downloadedBytesLabel->setText(QString());
        label_3->setText(QApplication::translate("CustomWindow", "INFO_DOWNLOAD_SPEED", 0, QApplication::UnicodeUTF8));
        peerCoutLabel->setText(QString());
        label_5->setText(QApplication::translate("CustomWindow", "INFO_TOTAL_SIZE", 0, QApplication::UnicodeUTF8));
        downloadSpeedLabel->setText(QString());
        label_9->setText(QApplication::translate("CustomWindow", "INFO_UPLOAD_SPEED", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("CustomWindow", "TAB_INFO", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem = peerTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CustomWindow", "PEER_IP", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem1 = peerTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CustomWindow", "PEER_CLIENT_NAME", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem2 = peerTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CustomWindow", "PEER_%", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem3 = peerTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CustomWindow", "PEER_DOWNLOAD_SPEED", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem4 = peerTableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("CustomWindow", "PEER_UPLOAD_SPEED", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem5 = peerTableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("CustomWindow", "PEER_DOWNLOADED", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem6 = peerTableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("CustomWindow", "PEER_UPLOADED", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("CustomWindow", "TAB_PEERS", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem7 = trackerTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem7->setText(QApplication::translate("CustomWindow", "TRACKER_URL", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem8 = trackerTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem8->setText(QApplication::translate("CustomWindow", "TRACKER_STATUS", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem* ___qtablewidgetitem9 = trackerTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem9->setText(QApplication::translate("CustomWindow", "TRACKER_NEXT_ANNOUNCE", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("CustomWindow", "TAB_TRACKERS", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("CustomWindow", "TAB_FILES", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui
{
class CustomWindow: public Ui_CustomWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMWINDOW_H
