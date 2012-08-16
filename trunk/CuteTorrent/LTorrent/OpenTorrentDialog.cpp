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
#include "OpenTorrentDialog.h"
#include "QApplicationSettings.h"
#include <QDir>
OpenTorrentDialog::OpenTorrentDialog(QWidget *parent, Qt::WFlags flags)
{
	setupUi(this);
	mgr=TorrentManager::getInstance();
	validTorrent=true;
}
int OpenTorrentDialog::execConditional()
{
	return validTorrent ? exec() : 1;
}
OpenTorrentDialog::~OpenTorrentDialog()
{
	TorrentManager::freeInstance();
}



void OpenTorrentDialog::SetData(QString filename)
{
	
	torrentFilename=filename;
	opentorrent_info* info=mgr->GetTorrentInfo(torrentFilename);
	if (info!=NULL)
	{
		setUpdatesEnabled( false );
		labelNameData->setText(info->name);
		labelComentData->setText(info->describtion);
		labelSizeData->setText(StaticHelpers::toKbMbGb(info->size));
		for (libtorrent::file_storage::iterator i = info->files.begin(); 
			i != info->files.end();
			 ++i)
		 {
				QTreeWidgetItem* item= new QTreeWidgetItem(treeWidget);
				item->setText(0,QString::fromUtf8(info->files.file_path(*i).c_str()));
				item->setText(1,helper->toKbMbGb(info->files.file_size(*i)));
		 }
		treeWidget->sortItems(0,Qt::AscendingOrder);
		setUpdatesEnabled( true );
		if (!info->base_suffix.isEmpty())
		{
			QApplicationSettings* instance= QApplicationSettings::getInstance();
			QList<GroupForFileFiltering> filters=instance->GetFileFilterGroups();
			for (int i=0;i<filters.count();i++)
			{
				if (filters.at(i).Contains(info->base_suffix))
				{
					pathEdit->setText(filters.at(i).SavePath());
					break;
				}
			}
			QApplicationSettings::FreeInstance();
		} 
		delete info;
	}
	else
		validTorrent=false;
	
}


void OpenTorrentDialog::BrowseButton()
{
	QApplicationSettings* settings=QApplicationSettings::getInstance();
	QString lastDir=settings->valueString("System","LastSaveTorrentDir");
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                             lastDir,
                                             QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);
	if (!dir.isEmpty())
	{
		
		dir.append(QDir::separator());
		dir=QDir::toNativeSeparators(dir);
		settings->setValue("System","LastSaveTorrentDir",dir);
		pathEdit->setText(dir);
	}
	QApplicationSettings::FreeInstance();
}



void OpenTorrentDialog::AccepTorrent()
{
	QFile file(torrentFilename);
	file.copy(combine_path("CT_DATA",file.fileName().toAscii().data()).c_str());
	mgr->AddTorrent(torrentFilename,pathEdit->displayText());
	close();
}
