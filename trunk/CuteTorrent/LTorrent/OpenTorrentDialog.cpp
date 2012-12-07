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
#include "OpenTorrentDialog.h"
#include "QApplicationSettings.h"
#include <QDir>
#include <QMap>
OpenTorrentDialog::OpenTorrentDialog(QWidget *parent, Qt::WFlags flags)
{
	setupUi(this);
	setupGroupComboBox();
	mgr=TorrentManager::getInstance();
	validTorrent=true;
}
void OpenTorrentDialog::setupGroupComboBox()
{

	
}
int OpenTorrentDialog::execConditional()
{
	return validTorrent ? exec() : 1;
}
OpenTorrentDialog::~OpenTorrentDialog()
{
	TorrentManager::freeInstance();
	delete model;
}



void OpenTorrentDialog::SetData(QString filename)
{
	if (filename.startsWith("magnet"))
	{


	}
	else
	{
		torrentFilename=filename;
		opentorrent_info* info=mgr->GetTorrentInfo(torrentFilename);
		if (info!=NULL)
		{
			setUpdatesEnabled( false );
			labelNameData->setText(info->name);
			labelComentData->setText(info->describtion);
			labelSizeData->setText(StaticHelpers::toKbMbGb(info->size));
			QStringList files;
			for (libtorrent::file_storage::iterator i = info->files.begin(); 
				i != info->files.end();
				++i)
			{
				files << QString::fromUtf8(info->files.file_path(*i).c_str())+"|"+StaticHelpers::toKbMbGb(info->files.file_size(*i));
			}
			files.sort();
			
			model = new FileTreeModel();
			for (int i=0;i<files.count();i++)
			{
				 
				QStringList parts= files.at(i).split('|');
				model->addPath(parts.at(0),parts.at(1));
					
			 }
			
			treeView->setModel(model);
			treeView->setColumnWidth(0,300);
			

			setUpdatesEnabled( true );
			if (!info->base_suffix.isEmpty())
			{
				QApplicationSettings* instance= QApplicationSettings::getInstance();
				filters=instance->GetFileFilterGroups();
				int selected=-1;
				for (int i=0;i<filters.count();i++)
				{
					GroupComboBox->addItem(filters[i].Name());
					
					if (filters.at(i).Contains(info->base_suffix) && selected<0)
					{
						selected=i;
						pathEdit->setText(filters.at(i).SavePath());
					}
				}
				if (selected>=0)
				GroupComboBox->setCurrentIndex(selected);
				QApplicationSettings::FreeInstance();
			} 
			delete info;
		}
		else
			validTorrent=false;
	}
}


void OpenTorrentDialog::BrowseButton()
{
	QApplicationSettings* settings=QApplicationSettings::getInstance();
	QString lastDir=settings->valueString("System","LastSaveTorrentDir");
	QString dir = QFileDialog::getExistingDirectory(this, tr("DIALOF_OPEN_DIR"),
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
	QMap<QString,int> filePriorities=model->getFilePiorites();
	mgr->AddTorrent(torrentFilename,pathEdit->displayText(),filePriorities);
	close();
}

void OpenTorrentDialog::ChangeGroup()
{
	if (GroupComboBox->currentIndex()>=0 && GroupComboBox->currentIndex()<filters.length())
		pathEdit->setText(filters[GroupComboBox->currentIndex()].SavePath());
}
