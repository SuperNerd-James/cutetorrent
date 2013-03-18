#include "DT_mounter.h"
#include <QMessageBox>
#include "QApplicationSettings.h"
#include <QProcess>
#include "QTorrentDisplayModel.h"
void DT_mounter::mountImage( QString path )
{
	QApplicationSettings* settings=QApplicationSettings::getInstance();
	QString exe = settings->valueString("DT","Executable");
	if (exe.isEmpty())
	{
		QApplicationSettings::FreeInstance();
		QMessageBox::warning(NULL,"DT Mounter",QTorrentDisplayModel::tr("DT_PATH_NOT_SET"));
		return;
	}
	bool useCustomCmd = settings->valueBool("DT","UseCustomCommand");
	int driveNum = settings->valueInt("DT","Drive");
	QString command = useCustomCmd ?  settings->valueString("DT","CustomtCommand"): settings->valueString("DT","DefaultCommand"); 
	QProcess *dt = new QProcess;
	QStringList args;

	dt->setNativeArguments(command.arg(QString::number(driveNum)).arg(path));
	dt->start(exe,args);
	QApplicationSettings::FreeInstance();
	if (!dt->waitForStarted(5000))
	{
		QMessageBox::warning(NULL,"DT Mounter",QTorrentDisplayModel::tr("LAUNCH_ERROR")+exe);
		return;
	}

	dt->waitForFinished();
	delete dt;
}
