/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

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
#include "SettingsDialog.h"

#include <QFileDialog>
#include <QString>
#include <QDir>
#include "messagebox.h"
#include "TorrentManager.h"
#include <QDebug>
#include <QTranslator>
#include <QIntValidator>
#include "application.h"
#include "Scheduller.h"
#include "webControll/RconWebService.h"
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include "qkeyedit.h"
#include <QScrollArea>
#include <QFormLayout>
#include "StyleEngene.h"
#define PIXELS_TO_ACT 2
#ifdef Q_WS_WIN //file association for windows
#include <windows.h>
#include <tchar.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
                GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}

#endif
SettingsDialog::SettingsDialog(QWidget* parrent,int flags)
{
    setupUi(this);
    previousFocuse=NULL;
    //qDebug()<<"QApplicationSettings::getInstance from SettingsDialog::SettingsDialog";
    settings = QApplicationSettings::getInstance();
    rcon = RconWebService::getInstance();
    tracker = TorrentTracker::getInstance();
    FillDTTab();
    FillFilteringGroups();
    FillTorrentTab();
    FillHDDTab();
    FillWebUITab();
    SetupSchedullerTab();
    FillSearchTab();
    FillKeyMapTab();
    FillNetworkTab();
    setupCustomeWindow();
    setupWindowIcons();
    //OS_SPECIFICK////
    int current=0;
#ifdef Q_WS_WIN
    QSettings assocSettings ("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QString torrentAssociation=assocSettings.value (".torrent/.").toString();
    magnetAssociationCheckBox->setChecked(assocSettings.value ("Magnet/shell/open/command/.").toString().toLower().contains("cutetorrent"));
    asociationCheckBox->setChecked( torrentAssociation == "CuteTorrent.file");
    QSettings bootUpSettings(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\")+(IsWow64() ? "Wow6432Node\\" : "")+"Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString val=bootUpSettings.value("CuteTorrent").toString();

    runOnbootCheckBox->setChecked(val.length()>0);
    startMinimizedCheckBox->setChecked(val.contains("-m"));
#endif

    //OS_SPECIFICK////
    QString curLoc=Application::currentLocale();
    foreach (QString avail, Application::availableLanguages())
    {

        qDebug() << avail << QLocale(avail);
        QLocale language =QLocale(avail);
        localeComboBox->addItem(QLocale::languageToString(language.language()),avail);

        if (avail==curLoc)
            localeComboBox->setCurrentIndex(current);
        current++;

    }
    StyleEngene* style = StyleEngene::getInstance();
    QObject::connect(style,SIGNAL(styleChanged()),this,SLOT(setupWindowIcons()));

    //tabWidget->removeTab(5);
}
void SettingsDialog::setupWindowIcons() {
    StyleEngene* style = StyleEngene::getInstance();
    pbMin->setIcon(style->getIcon("app_min"));
    pbClose->setIcon(style->getIcon("app_close"));

    RunningLabel->setPixmap(style->getIcon("active").pixmap(32,32));

    listWidget->item(0)->setIcon(style->getIcon("settings_torrent"));
    listWidget->item(1)->setIcon(style->getIcon("settings_network"));
    listWidget->item(2)->setIcon(style->getIcon("settings_hdd"));
    listWidget->item(3)->setIcon(style->getIcon("settings_filter"));
    listWidget->item(4)->setIcon(style->getIcon("settings_dt"));
    listWidget->item(5)->setIcon(style->getIcon("settings_scheduler"));
    listWidget->item(6)->setIcon(style->getIcon("settings_web_controll"));
    listWidget->item(7)->setIcon(style->getIcon("settings_search"));
    listWidget->item(8)->setIcon(style->getIcon("settings_hotkeys"));
}

void SettingsDialog::mouseMoveEvent(QMouseEvent *e)
{
    int xMouse = e->pos().x();
    int yMouse = e->pos().y();
    int wWidth = geometry().width();
    int wHeight = geometry().height();

    if (moveWidget)
    {
        inResizeZone = false;
        moveWindow(e);
    }
    else if (allowToResize)
        resizeWindow(e);
    //Cursor part dreta
    else if (xMouse >= wWidth - PIXELS_TO_ACT && allowToResize)
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
    else if (xMouse <= PIXELS_TO_ACT && allowToResize)
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
    else if ((yMouse >= wHeight - PIXELS_TO_ACT) && allowToResize)
    {
        inResizeZone = true;
        setCursor(Qt::SizeVerCursor);

        resizeWindow(e);
    }
    //Cursor part superior
    else if (yMouse <= PIXELS_TO_ACT && allowToResize)
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

void SettingsDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
		QPoint pos = e->pos();
        if (inResizeZone)
        {
            //allowToResize = true;

            if (pos.y() <= PIXELS_TO_ACT)
            {
                if (pos.x() <= PIXELS_TO_ACT)
                    resizeDiagSupEsq = true;
                else if (pos.x() >= geometry().width() - PIXELS_TO_ACT)
                    resizeDiagSupDer = true;
                else
                    resizeVerSup = true;
            }
            else if (pos.x() <= PIXELS_TO_ACT)
                resizeHorEsq = true;
        }
        else if (pos.x() >= PIXELS_TO_ACT&&pos.x() < titleBar->geometry().width()
                 &&pos.y() >= PIXELS_TO_ACT&&pos.y() < titleBar->geometry().height())
        {
            moveWidget = true;
            dragPosition = e->globalPos() - frameGeometry().topLeft();
        }
    }

    e->accept();
}

void SettingsDialog::mouseReleaseEvent(QMouseEvent *e)
{
    moveWidget = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;

    e->accept();
}

void SettingsDialog::mouseDoubleClickEvent(QMouseEvent *e)
{
	QRect geometry = tbMenu->geometry();
	QPoint point = e->pos();
    if (point.x() < geometry.right()&&point.y() < geometry.bottom()
            &&point.x() >=  geometry.x()&&point.y() >= geometry.y()
            &&tbMenu->isVisible())
        close();
    e->accept();
}

void SettingsDialog::paintEvent (QPaintEvent *)
{
    QStyleOption opt;
    opt.init (this);
    QPainter p(this);
    style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void SettingsDialog::moveWindow(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - dragPosition);
        e->accept();
    }
}





void SettingsDialog::minimizeBtnClicked()
{
    if (this->isMinimized())
    {
        setWindowState(windowState() & ~Qt::WindowMinimized);
    }
    else
    {
        setWindowState(windowState() | Qt::WindowMinimized);
    }
}

void SettingsDialog::chooseAction(QAbstractButton *button)
{
    switch(buttonBox->standardButton(button)){
    case QDialogButtonBox::Ok:
        ApplyAndClose();
        break;
    case QDialogButtonBox::Cancel:
        reject();
        break;
    case QDialogButtonBox::Apply:
        ApplySettings();
        break;

    }
}
void SettingsDialog::resizeWindow(QMouseEvent *e)
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
}
void SettingsDialog::setupCustomeWindow()
{
    // setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::FramelessWindowHint);

    //setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
    titleBar->setMouseTracking(true);
    LTitle->setMouseTracking(true);
    tbMenu->setMouseTracking(true);
    pbMin->setMouseTracking(true);
    pbClose->setMouseTracking(true);
    centralWidget->setMouseTracking(true);

    /*centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->setContentsMargins(9,9,9,9);*/

    //addAction(actionClose);

    connect(pbMin, SIGNAL(clicked()), this, SLOT(minimizeBtnClicked()));
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
}


void SettingsDialog::FillHDDTab()
{
    lockFilesCheckBox->setCheckState(settings->valueBool("Torrent","lock_files") ?  Qt::Checked :Qt::Unchecked );
    casheSizeLineEdit->setValue(settings->valueInt("Torrent","cache_size")*16);
    diskIOCasheModeComboBox->setCurrentIndex(settings->valueInt("Torrent","disk_io_write_mode"));
    useDiskReadAheadCheckBox->setCheckState(settings->valueBool("Torrent","use_disk_read_ahead") ?  Qt::Checked :Qt::Unchecked );
    alowReorderedOpsCheckBox->setCheckState(settings->valueBool("Torrent","allow_reordered_disk_operations") ?  Qt::Checked :Qt::Unchecked );
    lowPrioDiskCheckBox->setCheckState(settings->valueBool("Torrent","low_prio_disk") ?  Qt::Checked :Qt::Unchecked );
    useReadCasheCheckBox->setCheckState(settings->valueBool("Torrent","use_read_cache") ?  Qt::Checked :Qt::Unchecked );
}

void SettingsDialog::FillTorrentTab()
{
    portEdit->setText(settings->valueString("Torrent","listen_port"));
    portEdit->setValidator(new QIntValidator());
    uploadLimitEdit->setValue(settings->valueInt("Torrent","upload_rate_limit")/1024.0);
    downloadLimitEdit->setValue(settings->valueInt("Torrent","download_rate_limit")/1024.0);
    activeLimitEdit->setText(settings->valueString("Torrent","active_limit"));
    activeLimitEdit->setValidator(new QIntValidator());
    activeSeedLimitEdit->setText(settings->valueString("Torrent","active_seeds"));
    activeSeedLimitEdit->setValidator(new QIntValidator());
    activeDownloadLimitEdit->setText(settings->valueString("Torrent","active_downloads"));
    activeDownloadLimitEdit->setValidator(new QIntValidator());
    bool useProxy=settings->valueBool("Torrent","useProxy");
    proxyGroupBox->setChecked(useProxy);
    if (useProxy)
    {
        proxyHostEdit->setText(QString("%1:%2").arg(settings->valueString("Torrent",
                                                                          "proxy_hostname"),settings->valueString("Torrent","proxy_port")));
        proxyUsernameEdit->setText(settings->valueString("Torrent","proxy_username"));
        proxyPwdEdit->setText(settings->valueString("Torrent","proxy_password"));
    }
    trackerGroupBox->setChecked(settings->valueBool("TorrentTracker","enabled",false));
    trackerPortEdit->setText(settings->valueString("TorrentTracker","port","6996"));
    StyleEngene* styleEngine = StyleEngene::getInstance();
    QList<StyleInfo> styleInfos = styleEngine->getAvaliableStyles();
    StyleInfo currentStyle = styleEngine->getCuurentStyle();
    styleComboBox->clear();
    for (int i=0;i<styleInfos.size();i++) {
        styleComboBox->addItem(styleInfos[i].DisplayName,styleInfos[i].InternalName);
        if (styleInfos[i] == currentStyle) {
            styleComboBox->setCurrentIndex(i);
        }
    }
}

void SettingsDialog::FillFilteringGroups()
{

    filterGroups = settings->GetFileFilterGroups();
    for (int i=0;i<filterGroups.count();i++)
    {
        GroupsListWidget->addItem(filterGroups.at(i).Name());
    }
    GroupsListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

void SettingsDialog::FillDTTab()
{
    DTPathEdit->setText(settings->valueString("DT","Executable"));
    int driveNumber=settings->valueInt("DT","Drive");
    driveNumberComboBox->setCurrentIndex(driveNumber < driveNumberComboBox->count() ? driveNumber : 0);
    settings->setValue("DT","DefaultCommand","-mount dt,%1,\"%2\"");
    bool useCustomCommand=settings->valueBool("DT","UseCustomCommand");
    customMoutGroupBox->setChecked(useCustomCommand);
    customCommandEdit->setText( (useCustomCommand ? settings->valueString("DT","CustomtCommand") : settings->valueString("DT","DefaultCommand")));
}

void SettingsDialog::FillWebUITab()
{
    bool enabled  = settings->valueBool("WebControl","webui_enabled",false);
    webUIGroupBox->setChecked(enabled);
    loginLineEdit->setText(settings->valueString("WebControl","webui_login"));
    passwordLineEdit->setText(settings->valueString("WebControl","webui_password"));
    webPortLineEdit->setText(settings->valueString("WebControl","port","8080"));
    upnpCheckBox->setChecked(settings->valueBool("WebControl","enable_upnp",false));
    webUILogginGroupBox->setChecked(settings->valueBool("WebControl","enable_loggin",false));
    logLineEdit->setText(settings->valueString("WebControl","log_name"));
    IPFilterGroupBox->setChecked(settings->valueBool("WebControl","enable_ipfilter",false));
    ipFilterTextEdit->setText(settings->valueString("WebControl","ipfilter"));

    bool isRunning=rcon->isRunning();

    RunningLabel->setEnabled(isRunning && enabled);
    startRconButton->setEnabled(!isRunning && enabled);
    stopRconButton->setEnabled(isRunning && enabled);


}

void SettingsDialog::showSelectedGroup(int row)
{
    if (row > filterGroups.count())
    {
        return;
    }
    if (row <0)
        return;
	GroupForFileFiltering currentGroup = filterGroups.at(row);
    newGroupNameEdit->setText(currentGroup.Name());
    extensionsEdit->setText(currentGroup.Extensions());
    groupSavePathEdit->setText(currentGroup.SavePath());

}
SettingsDialog::~SettingsDialog() 
{
    RconWebService::freeInstance();
    TorrentTracker::freeInstance();
    //qDebug()<<"QApplicationSettings::FreeInstance from SettingsDialog::~SettingsDialog";
    QApplicationSettings::FreeInstance();
}
void SettingsDialog::ApplySettings()
{
    settings->setValue("Torrent","listen_port",					qVariantFromValue(portEdit->text().toInt()));
    settings->setValue("Torrent","active_limit",				qVariantFromValue(activeLimitEdit->text().toInt()));
    settings->setValue("Torrent","active_downloads",			qVariantFromValue(activeDownloadLimitEdit->text().toInt()));
    settings->setValue("Torrent","active_seeds",				qVariantFromValue(activeSeedLimitEdit->text().toInt()));


    settings->setValue("Torrent","upload_rate_limit",			qVariantFromValue(uploadLimitEdit->value()*1024));
    settings->setValue("Torrent","download_rate_limit",			qVariantFromValue(downloadLimitEdit->value()*1024));
    settings->setValue("Torrent","useProxy",					qVariantFromValue(proxyGroupBox->isChecked()));
    if (proxyGroupBox->isChecked())
    {
        QStringList iport= proxyHostEdit->text().split(':');
        if (iport.count()==2)
        {
            settings->setValue("Torrent","proxy_hostname",		qVariantFromValue(iport.at(0)));
            settings->setValue("Torrent","proxy_password",		qVariantFromValue(proxyPwdEdit->text()));
            settings->setValue("Torrent","proxy_port",			qVariantFromValue(iport.at(1)));
            settings->setValue("Torrent","proxy_type",			qVariantFromValue(proxyTypeComboBox->currentIndex()));
            settings->setValue("Torrent","proxy_username",		qVariantFromValue(proxyUsernameEdit->text()));
        }

    }

    settings->setValue("Torrent","lock_files",					lockFilesCheckBox->isChecked());
    settings->setValue("Torrent","cache_size",					casheSizeLineEdit->value() /16 );
    settings->setValue("Torrent","disk_io_read_mode",			diskIOCasheModeComboBox->currentIndex());
    settings->setValue("Torrent","disk_io_write_mode",			diskIOCasheModeComboBox->currentIndex());
    settings->setValue("Torrent","low_prio_disk",				lowPrioDiskCheckBox->isChecked() );
    settings->setValue("Torrent","allow_reordered_disk_operations",alowReorderedOpsCheckBox->isChecked() );
    settings->setValue("Torrent","use_read_cache",				useReadCasheCheckBox->isChecked() );

    settings->setValue("Torrent","in_enc_policy",               inEncPolicyComboBox->currentIndex());
    settings->setValue("Torrent","out_enc_policy",              outEncPolicyComboBox->currentIndex());
    settings->setValue("Torrent","allowed_enc_level",           encLevelComboBox->currentIndex());
    settings->setValue("Torrent","prefer_rc4",                  preferFullEncCheckBox->isChecked());

    ApplySettingsToSession();

    settings->setValue("DT","Executable",						DTPathEdit->text());
    settings->setValue("DT","Drive",							driveNumberComboBox->currentIndex());
    settings->setValue("DT","DefaultCommand",					"-mount dt,%1,\"%2\"");
    settings->setValue("DT","UseCustomCommand",					(customMoutGroupBox->isChecked()));
    settings->setValue("DT","CustomtCommand",					customCommandEdit->text());

    settings->setValue("WebControl","webui_enabled",			webUIGroupBox->isChecked());
    settings->setValue("WebControl","requireAuth",              !loginLineEdit->text().isEmpty());
    settings->setValue("WebControl","webui_login",				loginLineEdit->text());
    settings->setValue("WebControl","webui_password",			passwordLineEdit->text());
    settings->setValue("WebControl","port",                     webPortLineEdit->text());
    settings->setValue("WebControl","enable_upnp",				upnpCheckBox->isChecked());
    settings->setValue("WebControl","enable_loggin",			webUILogginGroupBox->isChecked());
    settings->setValue("WebControl","log_name",					logLineEdit->text());
    settings->setValue("WebControl","enable_ipfilter",			IPFilterGroupBox->isChecked());
    settings->setValue("WebControl","ipfilter",					ipFilterTextEdit->toPlainText());

    //
    settings->SaveFilterGropups(filterGroups);
    settings->setSearchSources(searchSources);
    settings->setValue("TorrentTracker","enabled",              trackerGroupBox->isChecked());
    settings->setValue("TorrentTracker","port",                 trackerPortEdit->text());


#ifdef Q_WS_WIN //file association for windows
    QSettings asocSettings ("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    QString base_dir=QDir::toNativeSeparators(settings->valueString("System","BaseDir"))+"CuteTorrent.exe";
    if (base_dir.isEmpty())
    {
        base_dir=QDir::toNativeSeparators(QApplication::applicationFilePath());
    }
    if (asociationCheckBox->checkState()==Qt::Checked)
    {


        asocSettings.setValue (".torrent/.", "CuteTorrent.file");
        asocSettings.setValue ("CuteTorrent.file/.", tr("Torrent file"));
        asocSettings.setValue (".torrent/OpenWithProgids/CuteTorrent.file","");
        asocSettings.setValue ("CuteTorrent.file/shell/open/command/.",
                               "\"" + QDir::toNativeSeparators (base_dir) + "\"" + " \"%1\"");
        asocSettings.setValue ("CuteTorrent.file/DefaultIcon/.",
                               QDir::toNativeSeparators (QApplication::applicationFilePath())+ ",1" );

    }
    else
    {
        asocSettings.remove(".torrent/OpenWithProgids/CuteTorrent.file");
        asocSettings.remove(".torrent/.");
        asocSettings.remove("CuteTorrent.file/.");
        asocSettings.remove("CuteTorrent.file/shell/open/command/.");
        asocSettings.remove("CuteTorrent.file/DefaultIcon/.");
    }
    if (magnetAssociationCheckBox->isChecked())
    {
        asocSettings.setValue ("Magnet/.", "Magnet URI");
        asocSettings.setValue ("Magnet/Content Type", "application/x-magnet");
        asocSettings.setValue ("Magnet/URL Protocol", "");
        asocSettings.setValue ("Magnet/shell/open/command/.",
                               "\"" + QDir::toNativeSeparators (base_dir) + "\"" + " \"%1\"");
    }
    else
    {
        asocSettings.remove ("Magnet/.");
        asocSettings.remove ("Magnet/Content Type");
        asocSettings.remove ("Magnet/URL Protocol");
        asocSettings.remove ("Magnet/shell/open/command/.");
    }
    QSettings bootUpSettings(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\")+(IsWow64() ? "Wow6432Node\\" : "")+"Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (runOnbootCheckBox->checkState()==Qt::Checked)
    {
        bootUpSettings.setValue("CuteTorrent","\""+base_dir+"\""+ (startMinimizedCheckBox->isChecked() ? " -m" : ""));
    }
    else
        bootUpSettings.remove("CuteTorrent");
#endif
    if (settings->valueBool("WebControl","webui_enabled",false))
    {
        rcon->Start();
        if (settings->valueBool("WebControl","enable_ipfilter",false))
            rcon->parseIpFilter(settings->valueString("WebControl","ipfilter"));
    }
    else
    {
        if (rcon->isRunning())
        {
            rcon->Stop();
        }
    }
    //qDebug() << "TorrentTracker enabled" << settings->valueBool("TorrentTracker","enabled",false);
    if (settings->valueBool("TorrentTracker","enabled",false))
    {
        tracker->start();
    }
    else
    {
        if (tracker->isRunning())
            tracker->stop();
    }
    int curLocaleIndex=localeComboBox->currentIndex();
    QString choosenLanguage = localeComboBox->itemData(curLocaleIndex).toString();
    if (Application::currentLocale() != choosenLanguage) {
        Application::setLanguage(choosenLanguage);
        Application::setLanguageQt(choosenLanguage);
        settings->setValue("System","Lang",choosenLanguage);
        calendarWidget->setLocale(QLocale(choosenLanguage));
        buttonBox->setLocale(QLocale(choosenLanguage));
    }

    QList<QKeyEdit*> keyEdits = keyMapContainer->findChildren<QKeyEdit*>();
    qDebug()  << keyEdits;
    QMap<QString,QVariant> keyMap;
    foreach(QKeyEdit* keyEdit,keyEdits) {
        keyMap.insert(keyEdit->objectName(),keyEdit->text());
    }
    settings->setGroupValues("KeyMap",keyMap);
    FillKeyMapTab();
    int styleIndex = styleComboBox->currentIndex();
    QString currentStyle = styleComboBox->itemData(styleIndex).toString();
    settings->setValue("System","Style",currentStyle);
    StyleEngene* sEngine = StyleEngene::getInstance();
    if (sEngine->getCuurentStyle().InternalName.compare(currentStyle) != 0) {
        sEngine->setStyle(currentStyle);
    }

}
void SettingsDialog::ApplySettingsToSession()
{

    TorrentManager* manager = TorrentManager::getInstance();

    libtorrent::session_settings current= manager->readSettings();
    current.active_limit		= activeLimitEdit->text().toInt();
    current.active_downloads	= activeDownloadLimitEdit->text().toInt();
    current.active_seeds		= activeSeedLimitEdit->text().toInt();

    current.cache_size			= casheSizeLineEdit->value() /16 ;
    current.use_read_cache		= useReadCasheCheckBox->isChecked();
    current.lock_files			= lockFilesCheckBox->isChecked();
    current.disk_io_write_mode  = diskIOCasheModeComboBox->currentIndex();
    current.disk_io_read_mode   = current.disk_io_write_mode;
    current.low_prio_disk		= lowPrioDiskCheckBox->isChecked();

    current.allow_reordered_disk_operations = alowReorderedOpsCheckBox->isChecked();

    current.upload_rate_limit	= uploadLimitEdit->value()*1024;

    current.download_rate_limit = downloadLimitEdit->value()*1024;

    manager->updateSettings(current);

    pe_settings enc_settings = manager->readEncSettings();

    enc_settings.in_enc_policy = (pe_settings::enc_policy)inEncPolicyComboBox->currentIndex();
    enc_settings.out_enc_policy = (pe_settings::enc_policy)outEncPolicyComboBox->currentIndex();
    enc_settings.allowed_enc_level = (pe_settings::enc_level)(encLevelComboBox->currentIndex()+1);
    enc_settings.prefer_rc4 = preferFullEncCheckBox->isChecked();

    qDebug() << "saving in_enc_policy" << (int)enc_settings.in_enc_policy;
    qDebug() << "saving out_enc_policy" << (int)enc_settings.out_enc_policy;
    manager->updateEncSettings(enc_settings);
    manager->freeInstance();

}
void SettingsDialog::addGroup()
{

    int foundRow=-1;
    QString name=newGroupNameEdit->text();
    if (name.isEmpty())
    {
        MyMessageBox::warning(this,tr("STR_SETTINGS"),
                              tr("ERROR_GROUP_NAME_NOT_SET"));
        return;
    }
    QString extensions=extensionsEdit->toPlainText();
    if (extensions.isEmpty())
    {
        MyMessageBox::warning(this,tr("STR_SETTINGS"),
                              tr("ERROR_NO_EXTENSIONS"));
        return;
    }
    QString dir=groupSavePathEdit->text();
    if (dir.isEmpty())
    {
        MyMessageBox::warning(this,tr("STR_SETTINGS"),
                              tr("ERROR_NO_PATH"));
        return;
    }
    if (!QDir(dir).exists())
    {
        MyMessageBox::warning(this,tr("STR_SETTINGS"),
                              tr("ERROR_PATH_NOT_EXISTS"));
        return;
    }

    for (int i=0;i<filterGroups.count();i++)
    {

        if (filterGroups.at(i).Name()==name)
        {
            foundRow=i;
            break;
        }
    }
    if (foundRow >= 0)
    {
        if (QMessageBox::No==MyMessageBox::warning(this,tr("STR_SETTINGS"),
                                                   tr("SHURE_IN_CHANGING_GROUP %1").arg(name),
                                                   QMessageBox::No | QMessageBox::Yes))
            return;
    }
    else
    {
        GroupsListWidget->addItem(name);
    }
    GroupForFileFiltering newfilterGroup = GroupForFileFiltering(name,extensions,dir);
    foundRow > 0 ? filterGroups.replace(foundRow,newfilterGroup) : filterGroups.append(newfilterGroup);

}
void SettingsDialog::removeGroup()
{
    if (GroupsListWidget->selectedItems().length() > 0)
    {
        QListWidgetItem* index=GroupsListWidget->selectedItems().first();
        if (index!=NULL)
        {
            QString name=index->text();
            int foundRow=-1;
            for (int i=0;i<filterGroups.count();i++)
            {

                if (filterGroups.at(i).Name()==name)
                {
                    foundRow=i;
                    break;
                }
            }
            if (foundRow >= 0)
            {
                newGroupNameEdit->setText("");
                extensionsEdit->setText("");
                groupSavePathEdit->setText("");
                filterGroups.removeAt(foundRow);
                delete index;
            }
            else
            {
                MyMessageBox::warning(this,"Error",QString(tr("Unable to find %1")).arg(name));
            }
        }
    }
}
void SettingsDialog::browseSavepathGroup()
{
    QString lastDir=settings->valueString("System","LastSaveTorrentDir");

    QString savaPathForCurrentGroup = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                                        lastDir,
                                                                        QFileDialog::ShowDirsOnly
                                                                        | QFileDialog::DontResolveSymlinks);
    savaPathForCurrentGroup.append("\\");
    groupSavePathEdit->setText(QDir::toNativeSeparators(savaPathForCurrentGroup));
}
void SettingsDialog::browseDTPath()
{
    QString lastDir=settings->valueString("System","LastSaveTorrentDir");
    QString DTPath = QFileDialog::getOpenFileName(this,
                                                  tr("WHERE_DT"), lastDir , tr("DaemonTools Lite (DTLite.exe);;DaemonTools PRO (DTAgent.exe);;Any File (*.*)"));

    DTPathEdit->setText(QDir::toNativeSeparators(DTPath));
}

void SettingsDialog::ApplyAndClose()
{
    ApplySettings();
    close();
}

void SettingsDialog::SetDate(QDate date)
{
    //	QPair<QDateTime,QDateTime> interval=calendarWidget->getSelectedInterval();

    beginDateTimeEdit->setDate(date);

}

void SettingsDialog::DeleteTask()
{
    int index=tasksComboBox->currentIndex();
    SchedulerTask currentTask=tasksComboBox->itemData(index).value<SchedulerTask>();
    tasksComboBox->removeItem(index);
    tasks.removeAt (index);
    settings->SaveSchedullerQueue(tasks);
}

void SettingsDialog::AddTask()
{

    SchedulerTask::TaskType type=SchedulerTask::UNKNOWN;
    int limit=-1;
    QString name=taskNameLineEdit->text();
    QVariant limitVal;
    if (pauseAllRadioButton->isChecked())
    {
        type=SchedulerTask::PAUSE_ALL;
    }
    else if(startAllRadioButton->isChecked())
    {
        type=SchedulerTask::START_ALL;
    }
    else if(limitDlRadioButton->isChecked())
    {
        type=SchedulerTask::LIMIT_DOWNLOAD;
        limit=dlLimitEdit->text().toInt();
    }
    else if(limitUlRadioButton->isChecked())
    {
        type=SchedulerTask::LIMIT_UPLOAD;
        limit=ulLimitEdit->text().toInt();
    }
    if (limit >0)
    {
        limitVal=qVariantFromValue(limit);
    }
    if (SchedulerTask::UNKNOWN==type)
    {
        MyMessageBox::warning(this,tr("ERROR_SRT"),tr("SCHEDULLER_UNKNOWN_TYPE"));
        return;
    }
    if (name.length()==0)
    {
        MyMessageBox::warning(this,tr("ERROR_SRT"),tr("SCHEDULLER_NO_NAME"));
        return;
    }
    SchedulerTask newTask(name,type,limitVal,beginDateTimeEdit->dateTime());
    tasks.push_back(newTask);
    settings->SaveSchedullerQueue(tasks);
    tasksComboBox->addItem(newTask.name(),qVariantFromValue(newTask));
    emit tasksChanged();

}

void SettingsDialog::SetupSchedullerTab()
{
    QObject::connect(calendarWidget,SIGNAL(clicked(QDate)),this,SLOT(SetDate(QDate)));
    beginDateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(120));
    QObject::connect(tasksComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateSchedullerTab(int)));
    tasks = settings->GetSchedullerQueue();
    for (int i=0;i<tasks.count();i++)
    {
        tasksComboBox->addItem(tasks.at(i).name(),qVariantFromValue(tasks.at(i)));
    }
    Scheduller* scheduller=Scheduller::getInstance();
    QObject::connect(this,SIGNAL(tasksChanged()),scheduller,SLOT(UpdateTasks()));
    Scheduller::freeInstance();
}

void SettingsDialog::FillNetworkTab() {
    //QMessageBox::information(this,"SettingsDialog::FillNetworkTab","Filling FillNetworkTab");

    inEncPolicyComboBox->setCurrentIndex(settings->valueInt("Torrent","in_enc_policy"));
    outEncPolicyComboBox->setCurrentIndex(settings->valueInt("Torrent","out_enc_policy"));
    encLevelComboBox->setCurrentIndex(settings->valueInt("Torrent","allowed_enc_level"));
    preferFullEncCheckBox->setChecked(settings->valueBool("Torrent","prefer_rc4"));

}

void SettingsDialog::FillKeyMapTab()
{
    QMap<QString, QVariant> keyMappings=settings->getGroupValues("KeyMap");
    qDeleteAll(keyMapContainer->findChildren<QGroupBox*>());
    QLayout* origLayout = keyMapContainer->layout();
    QGridLayout* layout =origLayout ? (QGridLayout*)origLayout :  new QGridLayout(keyMapContainer);
    QMap<QString,QMap<QString,QString>> grouppedKeyMap;
    int index = 0;
    for (QMap<QString, QVariant>::iterator i=keyMappings.begin();
         i!=keyMappings.end();++i, index++)
    {
        QString groupName  = i.key().split('_')[1];
        grouppedKeyMap[groupName][i.key()] = i.value().toString();
    }
    int size = keyMappings.size();
    int rightColumns =  size/ 2;
    if (size & 1)
        rightColumns++;
    int leftColumns =  size/ 2;
    int rightIndex=0,leftIndex=0;
    qDebug() << grouppedKeyMap;
    QStringList keys = grouppedKeyMap.keys();
    for(int i = 0; i<keys.length();i++){
        for(int j = 0; j<keys.length();j++){
            if (keys[i] != keys[j]) {
                if (grouppedKeyMap[keys[i]].size() > grouppedKeyMap[keys[j]].size()) {
                    qSwap(keys[i],keys[j]);
                }
            }
        }
    }

    foreach(QString groupName,keys) {

        QMap<QString,QString> keyMap = grouppedKeyMap[groupName];
        qDebug() << groupName <<  keyMap.size();
        QGroupBox* _groupBox = new  QGroupBox(keyMapContainer);
        _groupBox->setTitle(trUtf8(groupName.toUpper().toUtf8()));
        QFormLayout* groupLayout = new QFormLayout(_groupBox);
        for (QMap<QString, QString>::iterator i=keyMap.begin();
             i!=keyMap.end();++i, ++index)
        {
            QKeyEdit* keyEdit=new QKeyEdit(keyMapContainer);
            keyEdit->setText(i.value());
            keyEdit->show();
            keyEdit->setObjectName(i.key());
            groupLayout->addRow(trUtf8(i.key().toUtf8()),keyEdit);
        }
        _groupBox->setLayout(groupLayout);
        int keMapSize = keyMap.size();
        if (rightColumns - keMapSize >= 0) {
            rightColumns -= keMapSize;
            layout->addWidget(_groupBox,rightIndex,0,keMapSize,1);
            rightIndex+=keMapSize;
        } else {
            leftColumns  -= keMapSize;
            layout->addWidget(_groupBox,leftIndex,1,keMapSize,1);
            leftIndex += keMapSize;

        }

    }
}

void SettingsDialog::FillSearchTab()
{
    searchSources = settings->GetSearchSources();
    for(int i=0;i<searchSources.size();i++)
    {
        sourcesComboBox->addItem(searchSources[i].getName());
    }
}

void SettingsDialog::UpdateSchedullerTab( int index )
{
    SchedulerTask currentTask=tasksComboBox->itemData(index).value<SchedulerTask>();
    taskNameLineEdit->setText(currentTask.name());
    beginDateTimeEdit->setDateTime(currentTask.startTime());
    switch(currentTask.type())
    {
    case SchedulerTask::START_ALL :
        startAllRadioButton->setChecked(true);
        break;
    case SchedulerTask::PAUSE_ALL:
        pauseAllRadioButton->setChecked(true);
        break;
    case SchedulerTask::LIMIT_UPLOAD:
        limitUlRadioButton->setChecked(true);
        ulLimitEdit->setText(QString::number(currentTask.limit()));
        break;
    case SchedulerTask::LIMIT_DOWNLOAD:
        limitDlRadioButton->setChecked(true);
        dlLimitEdit->setText(QString::number(currentTask.limit()));
        break;
    }
}

void SettingsDialog::StartRcon()
{
    RconWebService* svc = RconWebService::getInstance();
    svc->Start();
    bool isRunning=svc->isRunning();
    RunningLabel->setEnabled(isRunning);
    startRconButton->setEnabled(!isRunning);
    stopRconButton->setEnabled(isRunning);
    RconWebService::freeInstance();
}

void SettingsDialog::StopRcon()
{
    RconWebService* svc = RconWebService::getInstance();
    svc->Stop();
    bool isRunning=svc->isRunning();
    RunningLabel->setEnabled(isRunning);
    startRconButton->setEnabled(!isRunning);
    stopRconButton->setEnabled(isRunning);
    RconWebService::freeInstance();
}

void SettingsDialog::changeEvent( QEvent *event )
{
    if (event->type()==QEvent::LanguageChange)
    {
        retranslateUi(this);
        FillDTTab();
        FillFilteringGroups();
        FillTorrentTab();
        FillHDDTab();
        FillWebUITab();
        SetupSchedullerTab();
        FillSearchTab();
        FillKeyMapTab();
        FillNetworkTab();

    }
}

void SettingsDialog::addSearchitem()
{
    int foundIndex=-1;
    QString name =  searchItemNameLineEdit->text();
    QString pattern = searchItemPatternLineEdit->text();
    if (name.length()==0)
    {
        MyMessageBox::warning(this,tr("ERROR_SRT"),tr("SEARCH_ITEM_NO_NAME"));
        return;
    }
    if (pattern.length()==0)
    {
        MyMessageBox::warning(this,tr("ERROR_SRT"),tr("SEARCH_ITEM_NO_PATTERN"));
        return;
    }
    for(int i=0;i<searchSources.size();i++)
    {
        if (searchSources[i].getName()==name)
        {
            foundIndex = i;
            break;
        }
    }
    if (foundIndex > 0)
    {
        if (QMessageBox::No==MyMessageBox::warning(this,tr("STR_SETTINGS"),
                                                   tr("SHURE_IN_CHANGING_SEARCH_ITEM %1").arg(name),
                                                   QMessageBox::No | QMessageBox::Yes))
            return;
        searchSources[foundIndex].setPattern(pattern);
    }

    else
    {

        SearchItem item;
        item.setName(name);
        item.setPattern(pattern);
        searchSources.append(item);
        sourcesComboBox->addItem(name);
    }

}

void SettingsDialog::removeSearchItem()
{
    int index = sourcesComboBox->currentIndex();
    if (index>=0)
    {
        sourcesComboBox->removeItem(index);
        searchSources.removeAt(index);
    }
}

void SettingsDialog::searchItemChanged(int index)
{
    if (index>=0)
    {
        searchItemNameLineEdit->setText(searchSources[index].getName());
        searchItemPatternLineEdit->setText(searchSources[index].getPattern());
    }
    else
    {
        searchItemNameLineEdit->setText("");
        searchItemPatternLineEdit->setText("");
    }
}


void SettingsDialog::OpenWebUI()
{
    RconWebService* svc = RconWebService::getInstance();
    if (svc->isRunning())
    {
        QDesktopServices desctopService;
        desctopService.openUrl(QUrl("http://localhost:"+webPortLineEdit->text()+"/"));
    }
    RconWebService::freeInstance();
}
