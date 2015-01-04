#include "DT_mounter.h"
#include "messagebox.h"
#include "QApplicationSettings.h"
#include <QProcess>
#include "QTorrentDisplayModel.h"
void DT_mounter::mountImage(QString path)
{
#ifdef Q_WS_WIN //file association for windows
    //qDebug() << "QApplicationSettings::getInstance from DT_mounter::mountImage";
    QApplicationSettings* settings = QApplicationSettings::getInstance();
    QString exe = settings->valueString("DT", "Executable");

    if(exe.isEmpty())
    {
        //qDebug() << "QApplicationSettings::FreeInstance from DT_mounter::mountImage";
        QApplicationSettings::FreeInstance();
        MyMessageBox::warning(NULL, "DT Mounter", QTorrentDisplayModel::tr("DT_PATH_NOT_SET"));
        return;
    }

    bool useCustomCmd = settings->valueBool("DT", "UseCustomCommand");
    int driveNum = settings->valueInt("DT", "Drive");
    QString command = useCustomCmd ?  settings->valueString("DT", "CustomtCommand") : settings->valueString("DT", "DefaultCommand");
    QProcess* dt = new QProcess;
    QStringList args;
    dt->setNativeArguments(command.arg(QString::number(driveNum)).arg(path));
    dt->start(exe, args);
    //qDebug() << "QApplicationSettings::FreeInstance from DT_mounter::mountImage";
    QApplicationSettings::FreeInstance();

    if(!dt->waitForStarted(5000))
    {
        MyMessageBox::warning(NULL, "DT Mounter", QTorrentDisplayModel::tr("LAUNCH_ERROR") + exe);
        return;
    }

    dt->waitForFinished();
    delete dt;
#endif
}
