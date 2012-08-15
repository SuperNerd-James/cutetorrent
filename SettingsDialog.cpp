#include "SettingsDialog.h"

#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include "TorrentManager.h"
SettingsDialog::SettingsDialog(QWidget* parrent,int flags) 
{
	setupUi(this);
	settings = QApplicationSettings::getInstance();
	filterGroups = settings->GetFileFilterGroups();
	for (int i=0;i<filterGroups.count();i++)
	{
		GroupsListWidget->addItem(filterGroups.at(i).Name());
	}
	GroupsListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
	portEdit->setText(settings->valueString("Torrent","listen_port"));
	uploadLimitEdit->setText(QString::number(settings->valueInt("Torrent","upload_rate_limit")/1024)+" Kb/s" );
	downloadLimitEdit->setText(QString::number(settings->valueInt("Torrent","download_rate_limit")/1024)+" Kb/s" );
	activeLimitEdit->setText(settings->valueString("Torrent","active_limit"));
	activeSeedLimitEdit->setText(settings->valueString("Torrent","active_seeds"));
	activeDownloadLimitEdit->setText(settings->valueString("Torrent","active_downloads"));
	bool useProxy=settings->valueBool("Torrent","useProxy");
	useProxyCheckBox->setCheckState(useProxy ? Qt::Checked :Qt::Unchecked);
	if (useProxy)
	{
		proxyHostEdit->setText(QString("%1:%2").arg(settings->valueString("Torrent",
				"proxy_hostname").arg(settings->valueString("Torrent","proxy_port"))));
		proxyUsernameEdit->setText(settings->valueString("Torrent","proxy_username"));
		proxyPwdEdit->setText(settings->valueString("Torrent","proxy_password"));
	}
	DTPathEdit->setText(settings->valueString("DT","Executable"));
	int driveNumber=settings->valueInt("DT","Drive");
	driveNumberComboBox->setCurrentIndex(driveNumber < driveNumberComboBox->count() ? driveNumber : 0);
	settings->setValue("DT","DefaultCommand","-mount dt,%1,\"%2\"");
	bool useCustomCommand=settings->valueBool("DT","UseCustomCommand");
	customMountCheckBox->setChecked(useCustomCommand);
	customCommandEdit->setText( (useCustomCommand ? settings->valueString("DT","CustomtCommand") : settings->valueString("DT","DefaultCommand")));
	
	
}
void SettingsDialog::showSelectedGroup(int row)
{
	if (row > filterGroups.count())
	{	
		return;
	}
	if (row <0)
		return;
	newGroupNameEdit->setText(filterGroups.at(row).Name());
	extensionsEdit->setText(filterGroups.at(row).Extensions());
	groupSavePathEdit->setText(filterGroups.at(row).SavePath());
	
}
SettingsDialog::~SettingsDialog() 
{
	
	QApplicationSettings::FreeInstance();
}
void SettingsDialog::customCommandSwitcher()
{
	customMoutGroupBox->setEnabled(customMountCheckBox->checkState()==Qt::Checked);
}
void SettingsDialog::proxySwitcher()
{
	proxyGroupBox->setEnabled(useProxyCheckBox->checkState()==Qt::Checked);
}

void SettingsDialog::trackerAuthSwitcher()
{
	authGroupBox->setEnabled(useAuthCheckBox->checkState()==Qt::Checked);
}
void SettingsDialog::saveSettings()
{
	settings->setValue("Torrent","listen_port",qVariantFromValue(portEdit->text().toInt()));
	settings->setValue("Torrent","active_limit",qVariantFromValue(activeLimitEdit->text().toInt()));
	settings->setValue("Torrent","active_downloads",qVariantFromValue(activeDownloadLimitEdit->text().toInt()));
	settings->setValue("Torrent","active_seeds",qVariantFromValue(activeSeedLimitEdit->text().toInt()));


	settings->setValue("Torrent","upload_rate_limit",qVariantFromValue(uploadLimitEdit->text().split(' ').at(0).toInt()*1024));
	settings->setValue("Torrent","download_rate_limit",qVariantFromValue(downloadLimitEdit->text().split(' ').at(0).toInt()*1024));
	settings->setValue("Torrent","useProxy",qVariantFromValue(useProxyCheckBox->checkState()==Qt::Checked));
	if (useProxyCheckBox->checkState()==Qt::Checked)
	{
		QStringList iport= proxyHostEdit->text().split(':');
		if (iport.count()==2)
		{
			settings->setValue("Torrent","proxy_hostname",qVariantFromValue(iport.at(0)));
			settings->setValue("Torrent","proxy_password",qVariantFromValue(proxyPwdEdit->text()));
			settings->setValue("Torrent","proxy_port",qVariantFromValue(iport[1]));
			settings->setValue("Torrent","proxy_type",qVariantFromValue(proxyTypeComboBox->currentIndex()));
			settings->setValue("Torrent","proxy_username",qVariantFromValue(proxyUsernameEdit->text()));
		}
		
	}
	
	ApplySettingsToSession();
	settings->setValue("DT","Executable",DTPathEdit->text());
	settings->setValue("DT","Drive",driveNumberComboBox->currentIndex());
	settings->setValue("DT","DefaultCommand","-mount dt,%1,\"%2\"");
	settings->setValue("DT","UseCustomCommand",(customMountCheckBox->checkState()==Qt::Checked));
	settings->setValue("DT","CustomtCommand",customCommandEdit->text());
	settings->SaveFilterGropups(filterGroups);
	
	close();
}
void SettingsDialog::ApplySettingsToSession()
{
	
	TorrentManager* manager = TorrentManager::getInstance();
	
	libtorrent::session_settings current= manager->readSettings();
	current.active_limit		= activeLimitEdit->text().toInt();
	current.active_downloads	= activeDownloadLimitEdit->text().toInt();
	current.active_seeds		= activeSeedLimitEdit->text().toInt();

	
	current.upload_rate_limit	= uploadLimitEdit->text().split(' ').at(0).toInt()*1024;
	
	current.download_rate_limit = downloadLimitEdit->text().split(' ').at(0).toInt()*1024;
	
	manager->updateSettings(current);
	
	manager->freeInstance();

}
void SettingsDialog::addGroup()
{
	
	int foundRow=-1;
	QString name=newGroupNameEdit->text();
	if (name.isEmpty())
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("Настройки"),
			QString::fromLocal8Bit("Вы не можете создать группу с пустым именем"));
		return;
	}
	QString extensions=extensionsEdit->toPlainText();
	if (extensions.isEmpty())
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("Настройки"),
			QString::fromLocal8Bit("Вы не можете создать группу которая не содержит ни одного расширения файла"));
		return;
	}
	QString dir=groupSavePathEdit->text();
	if (dir.isEmpty())
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("Настройки"),
			QString::fromLocal8Bit("Вы должны указать путь куда сохранть файлы для данной группы"));
		return;
	}
	if (!QDir(dir).exists())
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("Настройки"),
			QString::fromLocal8Bit("Указанный путь сохранения не существует!"));
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
		if (QMessageBox::No==QMessageBox::warning(this,QString::fromLocal8Bit("Настройки"),
			QString::fromLocal8Bit("Вы действительно хотите изменить группу %1").arg(name),
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
	QListWidgetItem* index=GroupsListWidget->selectedItems().first();
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
		QMessageBox::warning(this,"Error",QString(tr("Unable to find %1")).arg(name));
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
		QString::fromUtf8(tr("Указать папку с DaemonTools").toAscii().data()), lastDir , QString::fromUtf8("DaemonTools Lite (DTLite.exe);;Any File (*.*)"));
	
	DTPathEdit->setText(QDir::toNativeSeparators(DTPath));
}

void SettingsDialog::showCurrentTrackerSettings(){}
void SettingsDialog::addTracker(){}

void SettingsDialog::showHint(){}