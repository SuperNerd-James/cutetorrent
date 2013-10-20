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
#include <QTextCodec>
#include <QtNetwork/QHostAddress>
#include <QSortFilterProxyModel>
#include <QClipboard>
#include <QCleanlooksStyle>
#include "FileSizeItemDelegate.h"
#include "ProgressItemDelegate.h"
#include "PriorityItemDelegate.h"
#include <QTreeWidgetItem>
#include <QDesktopServices>
CuteTorrent::CuteTorrent(QWidget *parent, Qt::WFlags flags)
    : QWidget(parent)
{
    settings=QApplicationSettings::getInstance();

    Application::setLanguage(settings->valueString("System","Lang","Русский"));
    Application::setLanguageQt("qt_"+settings->valueString("System","Lang","Русский"));
    setupCustomeWindow();
    //ui->setupUi(this);

    //qDebug() << "QApplicationSettings::getInstance from CuteTorrent::CuteTorrent";

    model = new QTorrentDisplayModel(listView,this);
    tManager = TorrentManager::getInstance();
    notyfire = new UpdateNotifier();
    mayShowNotifies = false;



    setAcceptDrops(true);
    setupStatusBar();
    setupTray();
    setupToolBar();
    setupListView();
    setupTabelWidgets();
    setupFileTabel();
    setupGroupTreeWidget();
    setupConnections();

    tracker = TorrentTracker::getInstance();


    rcon = RconWebService::getInstance();
    if (settings->valueBool("WebControl","webui_enabled",false))
    {
        rcon->Start();
        if (settings->valueBool("WebControl","enable_ipfilter",false))
            rcon->parseIpFilter(settings->valueString("WebControl","ipfilter"));
        if (settings->valueBool("WebControl","enable_upnp",false))
        {
            libtorrent::upnp* upnpMapper=tManager->GetUpnp();
            int port=settings->valueInt("WebControl","port",8080);
            upnpMapper->add_mapping(upnp::tcp,port,port);
        }
    }
    if (settings->valueBool("TorrentTracker","enabled",false))
    {
        tracker->start();
    }

    QTextCodec *wantUnicode = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(wantUnicode);

    tManager->initSession();
    torrents = TorrentStorrage::getInstance();
    QTimer::singleShot(10000,this,SLOT(CheckForUpdates()));
    //	QTimer::singleShot(3000,this,SLOT(enableNitifyShow()));
    Scheduller* sch=Scheduller::getInstance();



}
void CuteTorrent::CheckForUpdates()
{
    notyfire->fetch();

}
void CuteTorrent::ShowAbout()
{
    QMessageBox::about(this,tr("ABAUT_TITLE"),tr("ABAUT_TEXT").arg(CT_VERSION));
}
void CuteTorrent::ShowUpdateNitify(const QString& newVersion)
{
#ifdef Q_WS_MAC
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    QBalloonTip::showBalloon("CuteTorrent", tr("CT_NEW_VERSION %1").arg(newVersion),QBalloonTip::UpdateNotyfy,qVariantFromValue(0), icon,
                             5* 1000);
#else
    trayIcon->showMessage("CuteTorrent", tr("CT_NEW_VERSION %1").arg(newVersion),QSystemTrayIcon::Information);
#endif

}

void CuteTorrent::setupStatusBar()
{
    upLabel = new QLabel(this);
    upLabelText = new QLabel(this);
    upLabelText->setMinimumWidth(140);
    upLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/upload.ico")).scaled(16,16,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    downLabel = new QLabel(this);
    downLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/download.ico")).scaled(16,16,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
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

}
void CuteTorrent::setupListView()
{
    QTorrentItemDelegat::max_width=width()-QApplication::style( )->pixelMetric( QStyle::PM_MessageBoxIconSize )-35-(listView->verticalScrollBar()->isVisible() ? listView->autoScrollMargin() : 0);
    listView->setItemDelegate(new QTorrentItemDelegat(this));
    listView->setModel(model);
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection );
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
}
void CuteTorrent::setupTabelWidgets()
{
    trackerTableWidget->verticalHeader()->hide();
    trackerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    trackerTableWidget->setColumnWidth(0,420);
    trackerTableWidget->setColumnWidth(2,120);
    trackerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    addTracker = new QAction(QIcon(":/MenuIcons/addTorrent.ico"),tr("ADD_TRACKER"),trackerTableWidget);
    QObject::connect(addTracker,SIGNAL(triggered()),this,SLOT(AddTracker()));
    trackerTableWidget->addAction(addTracker);
    /*
    gridLayout_3->setSpacing(0);
    title = new QLabel(tr("CUTE_TORRENT_TITLE"),this);

    QWidget* buttonPlaceHolder = new QWidget(this);
    toolButtonsUi = new Ui::ToolButtons();
    toolButtonsUi->setupUi(buttonPlaceHolder);

    gridLayout_3->addWidget(title,0,0,1,1);
    gridLayout_3->addWidget(buttonPlaceHolder,0,1,1,1,Qt::AlignRight);
    gridLayout_3->addWidget(mainMenuBar,1,0,1,2);
    gridLayout_3->addWidget(mainToolbar,2,0,1,2);
    gridLayout_3->addWidget(listView,3,0,1,2);
    gridLayout_3->addWidget(tabWidget,4,0,1,2);
    gridLayout_3->addWidget(statusBar(),5,0,1,2);*/
    //    fileTableWidget->setSortingEnabled(true);
    peerTableWidget->verticalHeader()->hide();
    peerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    peerTableWidget->setSortingEnabled(true);
    peerTableWidget->setColumnWidth(2,50);
    peerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    addPeer = new QAction(QIcon(":/MenuIcons/addTorrent.ico"),tr("ADD_PEER"),peerTableWidget);
    QObject::connect(addPeer,SIGNAL(triggered()),this,SLOT(AddPeer()));
    peerTableWidget->addAction(addPeer);


}
void CuteTorrent::setupToolBar()
{

    searchSource = new QComboBox(this);
    searchSources = settings->GetSearchSources();
    for(int i=0;i<searchSources.size();i++)
    {
        searchSource->addItem(searchSources[i].getName());
    }
    searchEdit=new QLineEdit(this);
    QObject::connect(searchEdit,SIGNAL(returnPressed()),this,SLOT(PeformSearch()));
    ul = new QSpinBox(this);

    ul->setSuffix(" Kb\\s");
    ul->setMaximum(12000.0f);
    ul->setSingleStep(10.0);
    ul->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    QObject::connect(ul,SIGNAL(valueChanged(int)),this,SLOT(UpdateUL(int)));
    dl = new QSpinBox(this);
    ul->setSpecialValueText(tr("None"));
    dl->setSpecialValueText(tr("None"));
    dl->setMaximum(12000.0f);
    dl->setSuffix(" Kb\\s");
    dl->setSingleStep(10.0);
    dl->setButtonSymbols(QAbstractSpinBox::PlusMinus);
    QObject::connect(dl,SIGNAL(valueChanged(int)),this,SLOT(UpdateDL(int)));
    uploadLimit = new QLabel(tr("LIMIT_UL"),this);
    uploadLimit->setBuddy(ul);
    downloadLimit = new QLabel(tr("LIMIT_DL"),this);
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

    QObject::connect(listView->selectionModel(), SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
                     model,SLOT(UpdateSelectedIndex(const QItemSelection &)));
    QObject::connect(listView,SIGNAL(customContextMenuRequested(const QPoint &)),model,SLOT(contextualMenu(const QPoint &)));
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                     this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    QObject::connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(UpdateTabWidget(int)));
    QObject::connect(model,SIGNAL(updateTabSender(int)),this,SLOT(UpdateTabWidget(int)));
    QObject::connect(tManager,SIGNAL(TorrentError(const QString&,const QString&)),this,SLOT(ShowTorrentError(const QString&,const QString&)));
    QObject::connect(tManager,SIGNAL(TorrentCompleted(const QString&,const QString&)),
                     this,SLOT(ShowTorrentCompletedNotyfy(const QString,const QString)));
    QObject::connect(tManager,SIGNAL(TorrentInfo(const QString&,const QString&)),
                     this,SLOT(ShowTorrentInfoNotyfy(const QString,const QString)));
    //QObject::connect(mng,SIGNAL(AddTorrentGui(Torrent*)),model,SLOT(AddTorrent(Torrent*)));
    QObject::connect(notyfire,SIGNAL(showUpdateNitify(const QString &)),this,SLOT(ShowUpdateNitify(const QString &)));
    QObject::connect(notyfire,SIGNAL(showNoUpdateNitify(const QString &)),this,SLOT(ShowNoUpdateNitify(const QString &)));
    QObject::connect(fileTableView,SIGNAL(customContextMenuRequested ( const QPoint &)),this,SLOT(FileTabContextMenu(const QPoint &)));
    QObject::connect(tManager,SIGNAL(initCompleted()),model,SLOT(initSessionFinished()));
    QObject::connect(model,SIGNAL(initCompleted()),this,SLOT(EnableNitifyShow()));
    QObject::connect(groupTreeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(ChnageTorrentFilter()));
}
void CuteTorrent::FileTabContextMenu(const QPoint & point)
{

    QModelIndex qmi=fileTableView->indexAt(point);
    if (qmi.isValid())
    {
        Torrent* tor = model->GetSelectedTorrent();
        if (tor!=NULL)
        {
            files_info fileInfos = tor->GetFileDownloadInfo();
            QModelIndex priorityIndex=proxymodel->index(qmi.row(),3);
            int currentPriority=priorityIndex.data().toInt();
            //qDebug() << qmi;
            //qDebug() << qmi.data();
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
            fileTabMenu->exec(fileTableView->mapToGlobal(point));
        }

    }
    else
    {

        fileTableView->selectionModel()->reset();

    }
}
void CuteTorrent::ShowNoUpdateNitify(const QString & ver)
{



}
void CuteTorrent::ShowTorrentError(const QString& name,const QString& error)
{
#ifndef Q_WS_MAC
    QBalloonTip::showBalloon("CuteTorrent", tr("CT_ERROR %1\n%2").arg(name).arg(error), QBalloonTip::Error,qVariantFromValue(0),
                             QSystemTrayIcon::Critical,15000,false);
#else
    trayIcon->showMessage("CuteTorrent", tr("CT_ERROR %1\n%2").arg(name).arg(error),QSystemTrayIcon::Critical);
#endif


}
void CuteTorrent::ShowTorrentCompletedNotyfy(const QString name,QString path)
{
    if (!mayShowNotifies)
        return;
    QBalloonTip::showBalloon("CuteTorrent", tr("CT_DOWNLOAD_COMPLETE %1").arg(name),QBalloonTip::TorrentCompleted,qVariantFromValue(path+name),
                             QSystemTrayIcon::Information,15000,false);

}
void CuteTorrent::UpdateTabWidget(int tab)
{
    //qDebug() << "updateTabWidget(" << tab << ");";


    trayIcon->setToolTip("CuteTorrent "CT_VERSION"\nUpload: "+tManager->GetSessionUploadSpeed()+"\nDownload:"+tManager->GetSessionDownloadSpeed());
    if (this->isMinimized())
        return;
    bool udapteLimits = false;
    if (tab < 0)
    {
        if (tab==-2)
            udapteLimits = true;
        tab=tabWidget->currentIndex();
    }
    //qDebug() << "udapteLimits =" << udapteLimits << ";";


    try
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
        case 3:

            UpdateFileTab();
            break;
        }
        if (udapteLimits)
        {
            Torrent* tor = model->GetSelectedTorrent();
            if (tor!=NULL)
            {
                if (ul->value()!=tor->GetUploadLimit()/1024)
                    ul->setValue(tor->GetUploadLimit()/1024);
                if (dl->value()!=tor->GetDownloadLimit()/1024)
                    dl->setValue(tor->GetDownloadLimit()/1024);
            }
            else
            {
                if (ul->value()!=tManager->GetUploadLimit()/1024)
                    ul->setValue(tManager->GetUploadLimit()/1024);
                if (dl->value()!=tManager->GetDownloadLimit()/1024)
                    dl->setValue(tManager->GetDownloadLimit()/1024);
            }


        }

        upLabelText->setText(QString("%1(%2)").arg(tManager->GetSessionUploaded()).arg(tManager->GetSessionUploadSpeed()));
        downLabelText->setText(QString("%1(%2)").arg(tManager->GetSessionDownloaded()).arg(tManager->GetSessionDownloadSpeed()));
        //	torrentFilterModel->Update();
    }
    catch (std::exception e)
    {
        QMessageBox::warning(this,"Error",QString("CuteTorrent::updateTabWidget()\n")+e.what());
    }

}

class MyTableWidgetItem : public QTableWidgetItem {
public:
    MyTableWidgetItem(QString text) : QTableWidgetItem(text)
    {

    }
    bool operator <(const QTableWidgetItem &other) const
    {
        QHostAddress thisadr(text());
        if (!thisadr.isNull())
        {
            //qDebug() << "Ip Addresses" << text() << other.text();
            QHostAddress otheradr(other.text());
            return thisadr.toIPv4Address() < otheradr.toIPv4Address();
        }
        else
            if (text().contains(" tb",Qt::CaseInsensitive) || text().contains(" gb",Qt::CaseInsensitive) ||text().contains(" kb",Qt::CaseInsensitive) || text().contains(" mb",Qt::CaseInsensitive) || text().contains(" b",Qt::CaseInsensitive))
            {
                QStringList parts1 = text().split(' ');
                bool ok;
                double speed1=parts1[0].toDouble(&ok);
                if (ok)
                {
                    QStringList parts2=other.text().split(' ');
                    double speed2=parts2[0].toDouble();

                    switch(parts1[1][0].toLower().toAscii())
                    {
                    case 'k':
                        speed1*=1024.0;
                        break;
                    case 'm':
                        speed1*=1024*1024.0;
                        break;
                    case 'g':
                        speed1*=1024*1024*1024.0;
                        break;
                    case 't':
                        speed1*=1024*1024*1024*1024.0;
                        break;
                    case 'b':
                        break;

                    }
                    //qDebug() << parts1 << speed1;
                    switch(parts2[1][0].toLower().toAscii())
                    {
                    case 'k':
                        speed2*=1024;
                        break;
                    case 'm':
                        speed2*=1024*1024;
                        break;
                    case 'g':
                        speed2*=1024*1024*1024;
                        break;
                    case 't':
                        speed2*=1024*1024*1024*1024;
                        break;
                    case 'b':
                        break;

                    }
                    //qDebug() << "Speed or size" << text() << other.text() << speed1 << speed2;
                    return speed1 < speed2;
                }

            }else
                if (text().endsWith('%'))
                {
                    //qDebug() << "Percentage" << text() << other.text() ;
                    QString perc1=text().remove('%'),perc2=other.text().remove('%');
                    return perc1.toDouble() < perc2.toDouble();
                }
        return QTableWidgetItem::operator <(other);
    }
};
void CuteTorrent::UpdateFileTab()
{
    Torrent* tor = model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        files_info infos = tor->GetFileDownloadInfo();
        fileViewModel->setDataSource(infos);
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
        if(!isVisible() && isMinimized())
        {
            setUpdatesEnabled(false);
        }
        else
        {
            setUpdatesEnabled(true);
        }

    }
    if (event->type()==QEvent::LanguageChange)
    {
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
        copyContext->setText(tr("ACTION_COPY"));

        downLabel->setToolTip(tr("STATUS_DWONLOAD"));
        downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
        upLabel->setToolTip(tr("STATUS_UPLOAD"));
        upLabelText->setToolTip(tr("STATUS_UPLOAD"));

        uploadLimit->setText(tr("LIMIT_UL"));
        downloadLimit->setText(tr("LIMIT_DL"));

        addTracker->setText(tr("ADD_TRACKER"));
        addPeer->setText(tr("ADD_PEER"));

        __qtreewidgetitem->setText(0,tr("TORRENTS_ACTIVITY"));
        __qtreewidgetitem1->setText(0,tr("DOWNLOADING_FLTR"));
        __qtreewidgetitem2->setText(0,tr("SEEDING_FLTR"));
        __qtreewidgetitem3->setText(0,tr("COMPLETED_FLTR"));
        __qtreewidgetitem4->setText(0,tr("ACTIVE_FLTR"));
        __qtreewidgetitem5->setText(0,tr("NOT_ACTIVE_FLTR"));
        __qtreewidgetitem6->setText(0,tr("TORRENT_GROUPS"));
        ul->setSpecialValueText(tr("None"));
        dl->setSpecialValueText(tr("None"));
        fileViewModel->retranslateUI();
        model->retranslate();

    }
    QWidget::changeEvent(event);


}
void CuteTorrent::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::DoubleClick:
        if (isVisible())
            hide();
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
    minimizeAction = new QAction(QIcon(":/images/app_min.ico"),tr("ACTION_HIDE"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(QIcon(":/images/app_max.ico"),tr("ACTION_MAXIMIZE_FULLSCREEN"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(QIcon(":/images/app_max.ico"),tr("ACTION_MAXIMIZE"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    quitAction = new QAction(QIcon(":/images/app_close.ico"),tr("ACTION_EXIT"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    copyContext = new QAction(QIcon(":/MenuIcons/copy-to-clipboard.ico"),tr("ACTION_COPY"),describtionLabel);
    connect(copyContext, SIGNAL(triggered()), this, SLOT(CopyDiscribtion()));
    describtionLabel->addAction(copyContext);

}
void CuteTorrent::ConnectMessageReceved(Application* a)
{
    QObject::connect(a,SIGNAL(messageReceived ( const QString & )), this, SLOT(HandleNewTorrent(const QString &)));
    QObject::connect(a,SIGNAL(OpenTorrent(QString)),this,SLOT(HandleNewTorrent(QString)));
}
void CuteTorrent::HandleNewTorrent(const QString & path)
{
    showNormal();
    activateWindow();
    OpenTorrentDialog dlg(this);
    dlg.SetData(path);
    dlg.execConditional();

}
void CuteTorrent::ShowCreateTorrentDialog(void)
{
    CreateTorrentDialog* dlg = new CreateTorrentDialog();

    dlg->exec();
}
void CuteTorrent::ShowOpenTorrentDialog()
{

    QString lastDir=settings->valueString("System","LastOpenTorrentDir","");
    QString filename =  QFileDialog::getOpenFileName(this,tr("OPEN_TOORENT_DIALOG"),
                                                     lastDir , tr("TORRENT_FILES (*.torrent);;Any File (*.*)"));
    if (!filename.isEmpty())
    {
        OpenTorrentDialog* dlg=new OpenTorrentDialog(this);
        settings->setValue("System","LastOpenTorrentDir",filename);
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

    Torrent* tor=model->GetSelectedTorrent();
    if (tor!=NULL)
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
        describtionLabel->setText(tor->GetDiscribtion());
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

    Torrent* tor=model->GetSelectedTorrent();

    if (tor!=NULL)
    {


        QVector<peer_info> peerInfos=QVector<peer_info>::fromStdVector(tor->GetPeerInfo());
        peerTableWidget->setRowCount(peerInfos.size());
        for(int i=0;i<peerInfos.size();i++)
        {
            peerTableWidget->setItem(i,0,new MyTableWidgetItem(QString::fromStdString(peerInfos[i].ip.address().to_string())));
            peerTableWidget->setItem(i,1,new MyTableWidgetItem(QString::fromUtf8(peerInfos[i].client.c_str())));
            peerTableWidget->setItem(i,2,new MyTableWidgetItem(QString::number(peerInfos[i].progress_ppm/10000.f,'f',1) + "%"));
            peerTableWidget->setItem(i,3,new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].down_speed)+"/s"));
            peerTableWidget->setItem(i,4,new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].up_speed)+"/s"));
            peerTableWidget->setItem(i,5,new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_download)));
            peerTableWidget->setItem(i,6,new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_upload)));
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
    Torrent* tor=model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        std::vector<announce_entry> trackers=tor->GetTrackerInfo();

        trackerTableWidget->setRowCount(trackers.size());

        for (size_type i=0;i<trackers.size();i++)
        {
            trackerTableWidget->setItem(i,0,new QTableWidgetItem(trackers[i].url.c_str()));
            trackerTableWidget->setItem(i,1,new QTableWidgetItem(trackers[i].message.length() >0 ? QString::fromUtf8(trackers[i].message.c_str()) : "Ok" ));
            trackerTableWidget->setItem(i,2,new QTableWidgetItem(StaticHelpers::toTimeString(trackers[i].next_announce_in())));

        }
        //trackerTableWidget->resizeColumnsToContents();


    }
    else
    {
        trackerTableWidget->setRowCount(0);
    }
}
void CuteTorrent::PauseSelected()
{
    model->ActionOnSelectedItem(QTorrentDisplayModel::pause);

}
void CuteTorrent::ResumeSelected()
{
    model->ActionOnSelectedItem(QTorrentDisplayModel::resume);

}
void CuteTorrent::DeleteSelected()
{
    model->ActionOnSelectedItem(QTorrentDisplayModel::remove);

}
void CuteTorrent::Retranslate()
{
    retranslateUi(this);
}
void CuteTorrent::OpenSettingsDialog()
{
    SettingsDialog* dlg = new SettingsDialog(this);
    QObject::connect(dlg,SIGNAL(needRetranslate()),this,SLOT(Retranslate()));
    dlg->exec();
    delete dlg;

    searchSources = settings->GetSearchSources();
    searchSource->clear();
    for(int i=0;i<searchSources.size();i++)
    {
        searchSource->addItem(searchSources[i].getName());
    }
    setupGroupTreeWidget();
    UpdateTabWidget(-2);
}
void CuteTorrent::closeEvent(QCloseEvent* ce)
{
    ce->ignore();
    hide();
    QBalloonTip::showBalloon("CuteTorrent", tr("CT_HIDE_MSG"),QBalloonTip::Info,qVariantFromValue(0),
                             QSystemTrayIcon::Information,5000,false);
    setUpdatesEnabled(false);
    return;


}

void CuteTorrent::dragEnterEvent(QDragEnterEvent *event)
{
    /*foreach (const QString &mime, event->mimeData()->formats())
    {
       //qDebug("mimeData: %s", mime.toLocal8Bit().data());
    }*/
    if (event->mimeData()->hasFormat(QString("text/plain")) || event->mimeData()->hasFormat(QString("text/uri-list")))
    {
        event->acceptProposedAction();
    }
}

void CuteTorrent::dropEvent(QDropEvent *event)
{
    QStringList files;
    if (event->mimeData()->hasUrls())
    {
        const QList<QUrl> urls = event->mimeData()->urls();
        foreach (const QUrl &url, urls)
        {
            if (!url.isEmpty())
            {
                if (url.scheme().compare("file", Qt::CaseInsensitive) == 0)
                    files << url.toLocalFile();
                else
                    files << url.toString();
            }
        }
    }
    else
    {
        files = event->mimeData()->text().split(QString::fromUtf8("\n"));
    }
    foreach(QString file,files)
    {
        if (file.startsWith("magnet:", Qt::CaseInsensitive) || file.endsWith(".torrent",Qt::CaseInsensitive))
        {
            OpenTorrentDialog* dlg2 = new OpenTorrentDialog();
            dlg2->SetData(file);
            dlg2->exec();
            delete dlg2;
            event->acceptProposedAction();
        }
        else
        {
            //qDebug() << file;
        }
    }


}
CuteTorrent::~CuteTorrent()
{
    settings->setValue("Window","geometry", geometry());
    settings->setValue("Window","maximized", isMaximized());
    settings->setValue("Window","selected_tab", tabWidget->currentIndex());
    //qDebug() << "CuteTorrent::~CuteTorrent";
    TorrentTracker::freeInstance();
    RconWebService::freeInstance();
    trayIcon->hide();
    TorrentManager::freeInstance();

    delete model;
    Scheduller::freeInstance();
    //qDebug() << "QApplicationSettings::FreeInstance() from CuteTorrent::~CuteTorren";
    QApplicationSettings::FreeInstance();
    delete notyfire;
    delete trayIcon;
}

void CuteTorrent::setupFileTabel()
{
    fileViewModel = new FileViewModel(this);
    proxymodel = new QSortFilterProxyModel(this);
    proxymodel->setSourceModel(fileViewModel);
    fileTableView->setModel(proxymodel);
    fileTableView->setShowGrid(false);
    fileTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    fileTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    fileTableView->setItemDelegateForColumn(1,new FileSizeItemDelegate(this));
    fileTableView->setItemDelegateForColumn(2,new ProgressItemDelegate(this));
    fileTableView->setItemDelegateForColumn(3,new PriorityItemDelegate(this));
    fileTableView->setSortingEnabled(true);
    fileTableView->setColumnWidth(0,480);
    fileTableView->setColumnWidth(1,65);
    fileTableView->setColumnWidth(2,65);
    fileTableView->setColumnWidth(3,70);
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
    QObject::connect(lowPriority, SIGNAL(triggered()), this, SLOT(SetLowForCurrentFile()));
    priority->addAction(lowPriority);
    mediumPriority = new QAction(tr("FILETAB_PRIORITY_MEDIUM"), fileTabMenu);
    mediumPriority->setCheckable(true);
    QObject::connect(mediumPriority, SIGNAL(triggered()), this, SLOT(SetMediumForCurrentFile()));
    priority->addAction(mediumPriority);
    highPriority = new QAction(tr("FILETAB_PRIORITY_HIGH"), fileTabMenu);
    highPriority->setCheckable(true);
    QObject::connect(highPriority, SIGNAL(triggered()), this, SLOT(SetHighForCurrentFile()));
    priority->addAction(highPriority);
    fileTabMenu->addMenu(priority);
    fileTabMenu->addSeparator();
    dontDownload = new QAction(tr("FILETAB_PRIORITY_ZERO"), fileTabMenu);
    dontDownload->setCheckable(true);
    QObject::connect(dontDownload, SIGNAL(triggered()), this, SLOT(SetNotDownloadForCurrentFile()));
    fileTabMenu->addAction(dontDownload);

}

void CuteTorrent::OpenFileSelected()
{
    Torrent* tor= model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        files_info info = tor->GetFileDownloadInfo();
        int file_num=fileTableView->selectionModel()->currentIndex().row();
        QDesktopServices desctopService;

        QString path=QString::fromStdString(combine_path(tor->GetSavePath().toStdString(),info.storrage.file_path(file_num)));

        desctopService.openUrl(QUrl("file:///"+path));
    }
}

void CuteTorrent::OpenDirSelected()
{
    Torrent* tor= model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        int file_num=fileTableView->selectionModel()->currentIndex().row();
        QModelIndex file_index = proxymodel->index(file_num,0);
        QString path = QFileInfo(QDir::toNativeSeparators(tor->GetSavePath()+file_index.data().toString())).absoluteFilePath();

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

void CuteTorrent::SetLowForCurrentFile()
{
    setFilePriority(2);
}

void CuteTorrent::SetMediumForCurrentFile()
{
    setFilePriority(5);
}

void CuteTorrent::SetHighForCurrentFile()
{
    setFilePriority(7);
}

void CuteTorrent::SetNotDownloadForCurrentFile()
{
    setFilePriority(0);
}

void CuteTorrent::ProcessMagnet()
{
    bool ok;
    QString magnetLink=QInputDialog::getText(this,tr("MAGNET_LINK_DLG"),tr("MAGNET_LINK:"),QLineEdit::Normal,"",&ok);
    if (ok && !magnetLink.isEmpty())
    {
        OpenTorrentDialog* dlg2 = new OpenTorrentDialog();
        dlg2->SetData(magnetLink);
        dlg2->exec();
        delete dlg2;
    }

}

void CuteTorrent::PeformSearch()
{

    QString searchText=searchEdit->text();
    int soureIndex = searchSource->currentIndex();
    if (soureIndex >= 0 )
    {
        SearchItem item = searchSources[soureIndex];
        if (item.getPattern().startsWith("client:/search?q=",Qt::CaseInsensitive))
        {
            torrents->setSearchFilter(searchText);
        }
        else
        {
            QDesktopServices desctopService;
            desctopService.openUrl(QUrl(item.getPattern().arg(QString(QUrl::toPercentEncoding(searchText)))));
        }
    }

}

void CuteTorrent::resizeEvent( QResizeEvent * event )
{
   // QTorrentItemDelegat::max_width=event->size().width()-QApplication::style( )->pixelMetric( QStyle::PM_MessageBoxIconSize )-35-(listView->verticalScrollBar()->isVisible() ? listView->autoScrollMargin() : 0)-groupTreeWidget->width();
    fillPieceDisplay(event->size());
}

void CuteTorrent::ShowTorrentInfoNotyfy( const QString name,const QString info)
{
#ifndef Q_WS_MAC
    QBalloonTip::showBalloon("CuteTorrent", QString("%1\n%2").arg(name).arg(info),QBalloonTip::Info,qVariantFromValue(0),
                             QSystemTrayIcon::Information,15000,false);
#else
    trayIcon->showMessage("CuteTorrent", QString("%1\n%2").arg(name).arg(info),QSystemTrayIcon::Information);
#endif
}

void CuteTorrent::keyPressEvent( QKeyEvent * event )
{
    if (focusWidget()!=listView)
    {
        QWidget::keyPressEvent(event);
        return;
    }
    switch(event->key())
    {
    case Qt::Key_Delete:
    {
        if(event->modifiers()==Qt::ShiftModifier)
        {


            model->ActionOnSelectedItem(QTorrentDisplayModel::remove_all);
        }
        else
        {
            model->ActionOnSelectedItem(QTorrentDisplayModel::remove);
        }
        event->accept();
        break;
    }
    case  Qt::Key_Enter:
    case  Qt::Key_Return:
    {
        model->OpenDirSelected();
        event->accept();
        break;
    }
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void CuteTorrent::fillPieceDisplay(QSize size)
{
    Torrent* tor=model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        int piece_count=tor->GetPieceCount();
        QVector<int> avaliablePieces = tor->GetDownloadedPieces();
        QVector<int> dwonloadingPieces = tor->GetDownloadingPieces();
        /*QGraphicsScene *scene = new QGraphicsScene(this);
        qreal pieceItemWidth  = (piceDwonloadedView->width())*1.0/piece_count;
        qreal pieceItemHeight = piceDwonloadedView->height()*0.8;
        for (int i=0;i<avaliablePieces.count();i++)
        {
            scene->addRect(avaliablePieces[i]*pieceItemWidth,0,pieceItemWidth,pieceItemHeight,QPen(Qt::darkBlue),QBrush(Qt::darkBlue));
        }
        for (int i=0;i<dwonloadingPieces.count();i++)
        {
            scene->addRect(dwonloadingPieces[i]*pieceItemWidth,0,pieceItemWidth,pieceItemHeight,QPen(Qt::green),QBrush(Qt::green));
        }
        piceDwonloadedView->scene()->deleteLater();
        piceDwonloadedView->setScene(scene);
        piceDwonloadedView->show();*/
        pieceView->resize(size);
        pieceView->setDowloadedParts(avaliablePieces);
        pieceView->setDowloadingParts(dwonloadingPieces);
        pieceView->setPiceCount(piece_count);
        pieceView->update();
    }

}

void CuteTorrent::CopyDiscribtion()
{
    QClipboard *clipboard = QApplication::clipboard();
    Torrent* tor=model->GetSelectedTorrent();
    if (tor!=NULL)
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
    //qDebug()<< "UpdateUL" << kbps*1024;
    Torrent* tor=model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        tor->SetUlLimit(kbps*1024);
    }
    else
    {
        //settings = QApplicationSettings::getInstance();
        settings->setValue("Torrent","upload_rate_limit",kbps*1024);
        //QApplicationSettings::FreeInstance();
        tManager->SetUlLimit(kbps*1024);
    }
}

void CuteTorrent::UpdateDL(int kbps)
{
    //qDebug()<< "UpdateDL" << kbps*1024;
    Torrent* tor=model->GetSelectedTorrent();

    if (tor!=NULL)
    {
        tor->SetDlLimit(kbps*1024);
    }
    else
    {
        //	QApplicationSettings* settings = QApplicationSettings::getInstance();
        settings->setValue("Torrent","download_rate_limit",kbps*1024);
        //	QApplicationSettings::FreeInstance();
        tManager->SetDlLimit(kbps*1024);
    }
}

void CuteTorrent::AddPeer()
{
    Torrent* torrent=model->GetSelectedTorrent();
    if (torrent!=NULL)
    {
        bool ok;
        QString peerStr = QInputDialog::getText(this,tr("ADD_PEER_DLG"),tr("PEER:"),QLineEdit::Normal,"",&ok);
        if (ok && !peerStr.isEmpty())
        {
            QStringList parts = peerStr.split(':');
            if (parts.count()==2)
            {
                QHostAddress adr(parts[0]);
                if(!adr.isNull())
                {
                    torrent->AddPeer(adr,parts[1].toUInt());
                    return;
                }

            }
            QMessageBox::critical(this,tr("PEER_ERR"),tr("INVALID_IP_STRING"));
        }
    }
}

void CuteTorrent::AddTracker()
{
    Torrent* torrent=model->GetSelectedTorrent();
    if (torrent!=NULL)
    {
        bool ok;
        QString trackerUrl = QInputDialog::getText(this,tr("ADD_TRACKER_DLG"),tr("TRACKER:"),QLineEdit::Normal,"",&ok);
        if (ok && !trackerUrl.isEmpty())
        {
            torrent->AddTracker(trackerUrl);
        }
    }
}

void CuteTorrent::StopSelected()
{
    model->ActionOnSelectedItem(QTorrentDisplayModel::stop);
}

void CuteTorrent::setFilePriority( int priority)
{
    Torrent* tor= model->GetSelectedTorrent();
    if (tor!=NULL)
    {
        //qDebug() << " CuteTorrent::setHighForCurrentFile";
        lowPriority->setChecked(false);
        mediumPriority->setChecked(false);
        dontDownload->setChecked(false);
        int file_num=fileTableView->selectionModel()->currentIndex().row();
        int file_index = proxymodel->index(file_num,0).data(Qt::UserRole).toInt();
        tor->SetFilePriority(file_index,priority);
        files_info info = tor->GetFileDownloadInfo();
        fileViewModel->setDataSource(info);

    }
}

void CuteTorrent::setupGroupTreeWidget()
{

    groupTreeWidget->clear();
    QIcon icon12;
    icon12.addFile(QString::fromUtf8(":/MenuIcons/super-seed.ico"), QSize(), QIcon::Normal, QIcon::Off);
    QIcon icon13;
    icon13.addFile(QString::fromUtf8(":/icons/download.ico"), QSize(), QIcon::Normal, QIcon::Off);
    QIcon icon14;
    icon14.addFile(QString::fromUtf8(":/icons/upload.ico"), QSize(), QIcon::Normal, QIcon::Off);
    QIcon icon15;
    icon15.addFile(QString::fromUtf8(":/icons/completed.ico"), QSize(), QIcon::Normal, QIcon::Off);
    QIcon icon16;
    icon16.addFile(QString::fromUtf8(":/icons/active.ico"), QSize(), QIcon::Normal, QIcon::Off);
    QIcon icon18(icon16.pixmap(QSize(16,16), QIcon::Disabled, QIcon::On));

    QIcon icon17;
    icon17.addFile(QString::fromUtf8(":/icons/groups.ico"), QSize(), QIcon::Normal, QIcon::Off);
    __qtreewidgetitem = new QTreeWidgetItem(groupTreeWidget);
    __qtreewidgetitem->setIcon(0, icon12);
    __qtreewidgetitem->setText(0,tr("TORRENTS_ACTIVITY"));
    __qtreewidgetitem->setText(1,"NONE");
    __qtreewidgetitem1 = new QTreeWidgetItem(__qtreewidgetitem);
    __qtreewidgetitem1->setIcon(0, icon13);
    __qtreewidgetitem1->setText(0,tr("DOWNLOADING_FLTR"));
    __qtreewidgetitem1->setText(1,"DWONLOADING");
    __qtreewidgetitem2 = new QTreeWidgetItem(__qtreewidgetitem);
    __qtreewidgetitem2->setIcon(0, icon14);
    __qtreewidgetitem2->setText(0,tr("SEEDING_FLTR"));
    __qtreewidgetitem2->setText(1,"SEEDING");
    __qtreewidgetitem3 = new QTreeWidgetItem(__qtreewidgetitem);
    __qtreewidgetitem3->setIcon(0, icon15);
    __qtreewidgetitem3->setText(0,tr("COMPLETED_FLTR"));
    __qtreewidgetitem3->setText(1,"COMPLETED");
    __qtreewidgetitem4 = new QTreeWidgetItem(__qtreewidgetitem);
    __qtreewidgetitem4->setIcon(0, icon16);
    __qtreewidgetitem4->setText(0,tr("ACTIVE_FLTR"));
    __qtreewidgetitem4->setText(1,"ACTIVE");
    __qtreewidgetitem5 = new QTreeWidgetItem(__qtreewidgetitem);
    __qtreewidgetitem5->setIcon(0, icon18);
    __qtreewidgetitem5->setText(0,tr("NOT_ACTIVE_FLTR"));
    __qtreewidgetitem5->setText(1,"NOT_ACTIVE");

    __qtreewidgetitem6 = new QTreeWidgetItem(groupTreeWidget);
    __qtreewidgetitem6->setText(0,tr("TORRENT_GROUPS"));
    __qtreewidgetitem6->setText(1,"NONE");
    __qtreewidgetitem6->setIcon(0, icon17);

    QList<GroupForFileFiltering> groups = settings->GetFileFilterGroups();
    QString type;
    groupTreeWidget->setSelectionMode(QAbstractItemView::SingleSelection );
    for (int i=0;i<groups.count();i++)
    {
        QTreeWidgetItem* item=new QTreeWidgetItem(__qtreewidgetitem6);
        item->setText(0,groups[i].Name());

        item->setIcon(0,StaticHelpers::guessMimeIcon(groups[i].Extensions().split('|')[0],type));
        item->setText(1,groups[i].Name());

    }
    groupTreeWidget->resizeColumnToContents(0);
    groupTreeWidget->expandAll();
}


void CuteTorrent::ChnageTorrentFilter()
{
    QTreeWidgetItem* item=groupTreeWidget->currentItem();
    QString filter = item->text(1);
    FilterType newFilter = StaticHelpers::StrToFilter(filter);
    if (newFilter==NONE && filter!="NONE")
    {
        torrents->setGroupFilter(filter);
    }
    else
    {
        torrents->setFilter(newFilter);
    }
    model->updateVisibleTorrents();
}

void CuteTorrent::setupCustomeWindow()
{
    setupUi(this);
    //	tbMenu->setIcon(QIcon(":/icons/app.ico"));

    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
    titleBar->setMouseTracking(true);
    LTitle->setMouseTracking(true);
    tbMenu->setMouseTracking(true);
    pbMin->setMouseTracking(true);
    pbMax->setMouseTracking(true);
    pbClose->setMouseTracking(true);
    centralWidget->setMouseTracking(true);
    listView->setMouseTracking(true);
    groupTreeWidget->setMouseTracking(true);
    tabWidget->setMouseTracking(true);
    /*centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(9,9,9,9);*/

    addAction(actionClose);

    connect(pbMin, SIGNAL(clicked()), this, SLOT(minimizeBtnClicked()));
    connect(pbMax, SIGNAL(clicked()), this, SLOT(maximizeBtnClicked()));
    connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));

    //Per poder rebre les dades del ratolн sense haver de clicar cap botу
    m_titleMode = FullTitle;
    moveWidget = false;
    inResizeZone = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;



    QRect geo = settings->value("Window","geometry").toRect();

    if (geo.height() > 0 && geo.x() < QApplication::desktop()->width() && geo.width() > 0 && geo.y() < QApplication::desktop()->height())
        setGeometry(geo);

    if (settings->value("Window","maximized").toBool())
    {
        showMaximized();
        pbMax->setIcon(QIcon(":/images/app_rest.ico"));
    }
    int selectedTab = settings->valueInt("Window","selected_tab");
    if (selectedTab>=0 && selectedTab < tabWidget->count())
    {
        tabWidget->setCurrentIndex(selectedTab);
    }
}



void CuteTorrent::mouseMoveEvent(QMouseEvent *e)
{
    int xMouse = e->pos().x();
    int yMouse = e->pos().y();
    int wWidth = geometry().width();
    int wHeight = geometry().height();

    if (moveWidget)
    {
        inResizeZone = false;
        if (isMaximized())
        {
            maximizeBtnClicked();

        }
        moveWindow(e);
    }
    else if (allowToResize)
        resizeWindow(e);
    //Cursor part dreta
    else if (xMouse >= wWidth - PIXELS_TO_ACT || allowToResize)
    {
        inResizeZone = true;

        if (yMouse >= wHeight - PIXELS_TO_ACT)
            setCursor(Qt::SizeFDiagCursor);
        else if (yMouse <= PIXELS_TO_ACT)
            setCursor(Qt::SizeBDiagCursor);
        else
            setCursor(Qt::SizeHorCursor);

        resizeWindow(e);
    }
    //Cursor part esquerra
    else if (xMouse <= PIXELS_TO_ACT||allowToResize)
    {
        inResizeZone = true;

        if (yMouse >= wHeight - PIXELS_TO_ACT)
            setCursor(Qt::SizeBDiagCursor);
        else if (yMouse <= PIXELS_TO_ACT)
            setCursor(Qt::SizeFDiagCursor);
        else
            setCursor(Qt::SizeHorCursor);

        resizeWindow(e);
    }
    //Cursor part inferior
    else if ((yMouse >= wHeight - PIXELS_TO_ACT)||allowToResize)
    {
        inResizeZone = true;
        setCursor(Qt::SizeVerCursor);

        resizeWindow(e);
    }
    //Cursor part superior
    else if (yMouse <= PIXELS_TO_ACT||allowToResize)
    {
        inResizeZone = true;
        setCursor(Qt::SizeVerCursor);

        resizeWindow(e);
    }
    else
    {
        inResizeZone = false;
        setCursor(Qt::ArrowCursor);
    }

    e->accept();
}

void CuteTorrent::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (inResizeZone)
        {
            allowToResize = true;

            if (e->pos().y() <= PIXELS_TO_ACT)
            {
                if (e->pos().x() <= PIXELS_TO_ACT)
                    resizeDiagSupEsq = true;
                else if (e->pos().x() >= geometry().width() - PIXELS_TO_ACT)
                    resizeDiagSupDer = true;
                else
                    resizeVerSup = true;
            }
            else if (e->pos().x() <= PIXELS_TO_ACT)
                resizeHorEsq = true;
        }
        else if (e->pos().x() >= PIXELS_TO_ACT&&e->pos().x() < titleBar->geometry().width()
                 &&e->pos().y() >= PIXELS_TO_ACT&&e->pos().y() < titleBar->geometry().height())
        {
            moveWidget = true;
            dragPosition = e->globalPos() - frameGeometry().topLeft();
    /*        if (isMaximized())
            {
                QRect desktopRect = QApplication::desktop()->availableGeometry();

                dragPosition.setX((desktopRect.width()-normalGeometry().width())/2);
            }*/
        }
    }

    e->accept();
}

void CuteTorrent::mouseReleaseEvent(QMouseEvent *e)
{
    moveWidget = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;

    e->accept();
}

void CuteTorrent::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->pos().x() < tbMenu->geometry().right()&&e->pos().y() < tbMenu->geometry().bottom()
            &&e->pos().x() >=  tbMenu->geometry().x()&&e->pos().y() >= tbMenu->geometry().y()
            &&tbMenu->isVisible())
        close();
    else if (e->pos().x() < titleBar->geometry().width()
             &&e->pos().y() < titleBar->geometry().height()
             &&m_titleMode != FullScreenMode)
        maximizeBtnClicked();
    e->accept();
}

void CuteTorrent::paintEvent (QPaintEvent *)
{
    QStyleOption opt;
    opt.init (this);
    QPainter p(this);
    style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void CuteTorrent::moveWindow(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - dragPosition);
        e->accept();
    }
}



void CuteTorrent::maximizeBtnClicked()
{
    if (isFullScreen()||isMaximized())
    {
        pbMax->setIcon(QIcon(":/images/app_max.ico"));
        showNormal();
    }
    else
    {
        pbMax->setIcon(QIcon(":/images/app_rest.ico"));
        showMaximized();
    }
}

void CuteTorrent::minimizeBtnClicked()
{
    if (isMinimized())
    {
        setWindowState(windowState() & ~Qt::WindowMinimized);
    }
    else
    {
        setWindowState(windowState() | Qt::WindowMinimized);
    }
}
void CuteTorrent::resizeWindow(QMouseEvent *e)
{
    if (allowToResize)
    {
        int xMouse = e->pos().x();
        int yMouse = e->pos().y();
        int wWidth = geometry().width();
        int wHeight = geometry().height();

        if (cursor().shape() == Qt::SizeVerCursor)
        {
            if (resizeVerSup)
            {
                int newY = geometry().y() + yMouse;
                int newHeight = wHeight - yMouse;

                if (newHeight > minimumSizeHint().height())
                {
                    resize(wWidth, newHeight);
                    move(geometry().x(), newY);
                }
            }
            else
                resize(wWidth, yMouse+1);
        }
        else if (cursor().shape() == Qt::SizeHorCursor)
        {
            if (resizeHorEsq)
            {
                int newX = geometry().x() + xMouse;
                int newWidth = wWidth - xMouse;

                if (newWidth > minimumSizeHint().width())
                {
                    resize(newWidth, wHeight);
                    move(newX, geometry().y());
                }
            }
            else
                resize(xMouse, wHeight);
        }
        else if (cursor().shape() == Qt::SizeBDiagCursor)
        {
            int newX = 0;
            int newWidth = 0;
            int newY = 0;
            int newHeight = 0;

            if (resizeDiagSupDer)
            {
                newX = geometry().x();
                newWidth = xMouse;
                newY = geometry().y() + yMouse;
                newHeight = wHeight - yMouse;
            }
            else
            {
                newX = geometry().x() + xMouse;
                newWidth = wWidth - xMouse;
                newY = geometry().y();
                newHeight = yMouse;
            }

            if (newWidth >= minimumSizeHint().width()&&newHeight >= minimumSizeHint().height())
            {
                resize(newWidth, newHeight);
                move(newX, newY);
            }
            else if (newWidth >= minimumSizeHint().width())
            {
                resize(newWidth, wHeight);
                move(newX, geometry().y());
            }
            else if (newHeight >= minimumSizeHint().height())
            {
                resize(wWidth, newHeight);
                move(geometry().x(), newY);
            }
        }
        else if (cursor().shape() == Qt::SizeFDiagCursor)
        {
            if (resizeDiagSupEsq)
            {
                int newX = geometry().x() + xMouse;
                int newWidth = wWidth - xMouse;
                int newY = geometry().y() + yMouse;
                int newHeight = wHeight - yMouse;

                if (newWidth >= minimumSizeHint().width() && newHeight >= minimumSizeHint().height())
                {
                    resize(newWidth, newHeight);
                    move(newX, newY);
                }
                else if (newWidth >= minimumSizeHint().width())
                {
                    resize(newWidth, wHeight);
                    move(newX, geometry().y());
                }
                else if (newHeight >= minimumSizeHint().height())
                {
                    resize(wWidth, newHeight);
                    move(geometry().x(), newY);
                }
            }
            else
                resize(xMouse+1, yMouse+1);
        }

        e->accept();
    }
    pieceView->update();
}
