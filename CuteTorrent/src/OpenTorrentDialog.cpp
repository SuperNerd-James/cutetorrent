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
#include <stddef.h>
#include <QDir>
#include <QMap>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QTextCodec>
#include "messagebox.h"
#include "FileTreeModel.h"
#include "OpenTorrentDialog.h"
#include "QApplicationSettings.h"
#include "TorrentManager.h"

OpenTorrentDialog::OpenTorrentDialog(QWidget* parent, Qt::WindowFlags flags) : useGroup(false), BaseWindow(BaseWindow::OnlyCloseButton, BaseWindow::NoResize), model(NULL)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	mgr = TorrentManager::getInstance();
	validTorrent = true;
	QTextCodec* wantUnicode = QTextCodec::codecForName("UTF-8");
	/*QTextCodec::setCodecForTr(wantUnicode);
	QTextCodec::setCodecForLocale(wantUnicode);*/
	QTextCodec::setCodecForLocale(wantUnicode);
}

void OpenTorrentDialog::reject()
{
	mgr->CancelMagnetLink(torrentFilename);
	QDialog::reject();
}
int OpenTorrentDialog::execConditional()
{
	return validTorrent ? exec() : QDialog::Rejected;
}
OpenTorrentDialog::~OpenTorrentDialog()
{
	TorrentManager::freeInstance();

	if (model != NULL)
	{
		delete model;
	}
}



void OpenTorrentDialog::SetData(QString filename)
{
	torrentFilename = filename;

	if(filename.startsWith("magnet"))
	{
		QMovie* movie = new QMovie(":/images/loader.gif");
		loaderGifLabel->setMovie(movie);
		movie->start();
		qRegisterMetaType<openmagnet_info> ("openmagnet_info");
		MetaDataDownloadWaiter* magnetWaiter = new MetaDataDownloadWaiter(filename);
		QObject::connect(magnetWaiter, SIGNAL(DownloadCompleted(openmagnet_info)), this, SLOT(DownloadMetadataCompleted(openmagnet_info)));
		QObject::connect(magnetWaiter, SIGNAL(ErrorOccured(QString)), this, SLOT(OnError(QString)));
		magnetWaiter->start(QThread::HighPriority);
		yesButton->setEnabled(false);
	}
	else
	{
		loaderGifLabel->hide();
		loaderTextLabel->hide();
		opentorrent_info* info = mgr->GetTorrentInfo(torrentFilename);

		if(info != NULL)
		{
			setUpdatesEnabled(false);
			labelNameData->setText(info->name);
			QFontMetrics metrics(labelComentData->font());
			QString elidedText = metrics.elidedText(info->describtion, Qt::ElideRight, labelComentData->width());
			labelComentData->setText(elidedText);
			labelSizeData->setText(StaticHelpers::toKbMbGb(info->size));
			QStringList files;

			for(libtorrent::file_storage::iterator i = info->files.begin();
			        i != info->files.end();
			        ++i)
			{
				files << QString::fromUtf8(info->files.file_path(*i).c_str()) + "|" + StaticHelpers::toKbMbGb(info->files.file_size(*i));
			}

			files.sort();
			model = new FileTreeModel();

			for(int i = 0; i < files.count(); i++)
			{
				QStringList parts = files.at(i).split('|');
				model->addPath(parts.at(0), parts.at(1));
			}

			torrentFilesTreeView->setModel(model);
			torrentFilesTreeView->setColumnWidth(0, 300);
			setUpdatesEnabled(true);

			if(!info->base_suffix.isEmpty())
			{
				QApplicationSettings* instance = QApplicationSettings::getInstance();
				filters = instance->GetFileFilterGroups();
				int selected = -1;

				for(int i = 0; i < filters.count(); i++)
				{
					GroupComboBox->addItem(filters[i].Name());

					if(filters.at(i).Contains(info->base_suffix) && selected < 0)
					{
						selected = i;
						pathEdit->setText(filters.at(i).SavePath());
					}
				}

				if(selected >= 0)
				{
					GroupComboBox->setCurrentIndex(selected);
				}
				else
				{
					QApplicationSettings* settings = QApplicationSettings::getInstance();
					QString lastDir = settings->valueString("System", "LastSaveTorrentDir");
					pathEdit->setText(lastDir);
					GroupComboBox->setCurrentIndex(-1);
					QApplicationSettings::FreeInstance();
				}

				QApplicationSettings::FreeInstance();
			}

			delete info;
		}
		else
		{
			validTorrent = false;
		}
	}
}



void OpenTorrentDialog::BrowseButton()
{
	QApplicationSettings* settings = QApplicationSettings::getInstance();
	QString lastDir = pathEdit->text();
	QString dir = QFileDialog::getExistingDirectory(this, tr("DIALOF_OPEN_DIR"),
	              lastDir,
	              QFileDialog::ShowDirsOnly
	              | QFileDialog::DontResolveSymlinks);

	if(!dir.isEmpty())
	{
		dir.append(QDir::separator());
		dir = QDir::toNativeSeparators(dir);
		settings->setValue("System", "LastSaveTorrentDir", dir);
		GroupComboBox->setCurrentIndex(-1);
		pathEdit->setText(dir);
		useGroup = false;
	}

	QApplicationSettings::FreeInstance();
}



void OpenTorrentDialog::AccepTorrent()
{
	if(validTorrent)
	{
		QMap<QString, qint8> filePriorities = model->getFilePiorites();
		error_code ec;
		int groupIndex = GroupComboBox->currentIndex();
		QString group = groupIndex >= 0 ? filters[groupIndex].Name() : "";

		if(!torrentFilename.startsWith("magnet"))
		{
			mgr->AddTorrent(torrentFilename, pathEdit->displayText(), labelNameData->text(), ec, filePriorities, group, setSequntialCheckBox->isChecked());
		}
		else
		{
			mgr->AddMagnet(_info.handle, pathEdit->displayText(), group, filePriorities);
		}

		if(ec)
		{
			MyMessageBox::critical(this, "Adding torrent Error", QString::fromStdString(ec.message()));
			return;
		}
	}

	accept();
}

void OpenTorrentDialog::ChangeGroup()
{
	if(GroupComboBox->currentIndex() >= 0 && GroupComboBox->currentIndex() < filters.length())
	{
		pathEdit->setText(filters[GroupComboBox->currentIndex()].SavePath());
		useGroup = true;
	}
}

void OpenTorrentDialog::DownloadMetadataCompleted(openmagnet_info info)
{
	loaderGifLabel->hide();
	loaderTextLabel->hide();
	yesButton->setEnabled(true);
	_info = info;
	setUpdatesEnabled(false);
	labelNameData->setText(info.name);
	QFontMetrics metrics(labelComentData->font());
	QString elidedText = metrics.elidedText(info.describtion, Qt::ElideRight, labelComentData->width());
	labelComentData->setText(elidedText);
	labelSizeData->setText(StaticHelpers::toKbMbGb(info.size));
	QStringList files;

	for(libtorrent::file_storage::iterator i = info.files.begin();
	        i != info.files.end();
	        ++i)
	{
		files << QString::fromUtf8(info.files.file_path(*i).c_str()) + "|" + StaticHelpers::toKbMbGb(info.files.file_size(*i));
	}

	files.sort();
	model = new FileTreeModel();

	for(int i = 0; i < files.count(); i++)
	{
		QStringList parts = files.at(i).split('|');

		if(parts.count() >= 1)
		{
			model->addPath(parts.at(0), parts.at(1));
		}
	}

	torrentFilesTreeView->setModel(model);
	torrentFilesTreeView->setColumnWidth(0, 300);
	setUpdatesEnabled(true);

	if(!info.base_suffix.isEmpty())
	{
		try
		{
			QApplicationSettings* instance = QApplicationSettings::getInstance();
			filters = instance->GetFileFilterGroups();
			int selected = -1;

			for(int i = 0; i < filters.count(); i++)
			{
				GroupComboBox->addItem(filters[i].Name());

				if(filters.at(i).Contains(info.base_suffix) && selected < 0)
				{
					selected = i;
					pathEdit->setText(filters.at(i).SavePath());
				}
			}

			if(selected >= 0)
			{
				GroupComboBox->setCurrentIndex(selected);
			}
		}
		catch(std::exception ex)
		{
			qDebug() << ex.what();
		}

		QApplicationSettings::FreeInstance();
	}
}

void OpenTorrentDialog::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);
	}
}

QPushButton* OpenTorrentDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* OpenTorrentDialog::getTitleBar()
{
	return titleBar;
}

QLabel* OpenTorrentDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* OpenTorrentDialog::getTitleIcon()
{
	return tbMenu;
}

QWidget* OpenTorrentDialog::centralWidget()
{
	return m_centralWidget;
}

void OpenTorrentDialog::OnError(QString error_msg)
{
	MyMessageBox::critical(this, "Error", error_msg);
	accept();
}
