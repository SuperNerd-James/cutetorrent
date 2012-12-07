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
#include "CreateTorrentDilaog.h"
#include <QDebug>
CreateTorrentDialog::CreateTorrentDialog(QWidget *parent, Qt::WFlags flags)
{
	setupUi(this);
	
	mgr= TorrentManager::getInstance();
	
}
quint64 CreateTorrentDialog::listFolder ( QString path ) {
	QDir currentFolder( path );

	quint64 totalsize = 0;

	currentFolder.setFilter( QDir::Dirs | QDir::Files | QDir::NoSymLinks );
	currentFolder.setSorting( QDir::Name );

	QFileInfoList folderitems( currentFolder.entryInfoList() );

	foreach ( QFileInfo i, folderitems ) {
		QString iname( i.fileName() );
		if ( iname == "." || iname == ".." || iname.isEmpty() )
			continue;

		if ( i.isDir() )
			totalsize += listFolder( path+"/"+iname );
		else
			totalsize += i.size();
	}
	return totalsize;
}
CreateTorrentDialog::~CreateTorrentDialog()
{
	TorrentManager::freeInstance();
}

quint64 CreateTorrentDialog::getPiceSize()
{
	
	switch (piceSizeComboBox->currentIndex())
	{
		case 0 :
		{	QString file=pathEdit->text();
			QFileInfo fi(file);
			quint64 fileSize=0;
			quint64 dirSize=0;
			int pieceSize=0;
			int needToSet=0;

			if(fi.isFile()){
				fileSize=fi.size();
				pieceSize=fileSize/1000;
			}
			if(fi.isDir()){
				//qDebug()<<"this is dir";
				dirSize=listFolder(file);
				pieceSize=dirSize/1000;
			}
			if(pieceSize<(32*1024)){
				needToSet=32;
			}else if(pieceSize<(64*1024)){
				needToSet=64;
			}else if(pieceSize<(128*1024)){
				needToSet=128;
			}else if(pieceSize<(256*1024)){
				needToSet=256;
			}else if(pieceSize<(512*1024)){
				needToSet=512;
			}else if(pieceSize<(1024*1024)){
				needToSet=1024;
			}else if(pieceSize<(2*1024*1024)){
				needToSet=2048;
			}else if (pieceSize<4*1024*1024){
				needToSet=4096;
			}else if (pieceSize< 8*1024*1024){
				needToSet=8*1024;
			}else 
				needToSet=16*1024;
			return needToSet;	
		}
		case 1 :
			return 16*1024;
		case 2:
			return 8*1024;
		case 3:
			return 4*1024;
		case 4:
			return 2*1024;
		case 5:
			return 1024;
		case 6:
			return 512;
		case 7:
			return 256;
		case 8:
			return 128;
		case 9:
			return 64;
		default:
			return 4*1024;
	}
	
}
void CreateTorrentDialog::BrowseDir()
{
	path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit(tr("DIALOG_OPEN_FOLDER").toAscii().data()),
                                             "",
                                             QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);
	pathEdit->setText(path);
}
void CreateTorrentDialog::BrowseFile()
{
	path =  QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit(tr("DIALOG_OPEN_FILE").toAscii().data()), "", QString::fromLocal8Bit("Any File (*.*)"));
	pathEdit->setText(path);
}
void CreateTorrentDialog::BeginCreate()
{
	createButton->setEnabled(false);
	
	creator  = new torrentCreatorThread(this);
	
	
	
	
	
	QObject::connect(creator,SIGNAL(updateProgress(int)),this,SLOT(UpdateProgressBar(int)));

	
	QObject::connect(creator,SIGNAL(ShowCreationSucces(QString)),this,SLOT(ShowCreationSucces(QString)));

	
	QObject::connect(creator,SIGNAL(ShowCreationFailture(QString)),this,SLOT(ShowCreationFailture(QString)));

	
	QObject::connect(this,SIGNAL(AbortCreation()),creator,SLOT(Abort()));
/*	info->describtion	= discribtionEdit->text().toStdString();
	info->filter		= filterEdit->text().toStdString();
	
	
	
	info->startSeed		= (startSeedCheckBox->checkState() == Qt::Checked);
	
	info->saveFileOrder	= (saveOrderCheckBox->checkState() == Qt::Checked);
	
	info->_private		= (privateCheckBox->checkState() == Qt::Checked);*/
	
	QStringList trackers=trackerEdit->toPlainText().split('\n');
	
	QStringList webseeds=webSeedEdit->toPlainText().split('\n');
	
	if (path.length()==0)
	{	QMessageBox::information(this, QString::fromLocal8Bit(tr("ERROR_STR").toAscii().data()),
		QString::fromLocal8Bit(tr("ERROR_NO_FILE_OR_FOLDER_NAME").toAscii().data()));
		delete creator;
		createButton->setEnabled(true);
		return;
	}
	

	
	for (int i=0;i<trackers.size();i++)
	{
		if (trackers.at(i).isEmpty())
			trackers.removeAt(i);
	}
	if (trackers.count()==0)
	{	if (QMessageBox::No == QMessageBox::information(this, QString::fromLocal8Bit(tr("ERROR_STR").toAscii().data()),
		QString::fromLocal8Bit(tr("ERROR_NO_TRACKERS").toAscii().data()),
		QMessageBox::Yes | QMessageBox::No))
		{
			delete creator;
			createButton->setEnabled(true);
			return;
		}
	}
	
	for (int i=0;i<webseeds.size();i++)
	{
		if (webseeds.at(i).isEmpty())
			webseeds.removeAt(i);
	}
	
	
	
	QString save_path = QFileDialog::getSaveFileName(this,
		QString::fromLocal8Bit(tr("CREATE_TORRENT_DIALOG").toAscii().data()),"",
		QString::fromLocal8Bit(tr("Торрент файлы (*.torrent)").toAscii().data()));
	
	creator->create(pathEdit->text(),save_path,filterEdit->text(),trackers,webseeds,discribtionEdit->text(),(privateCheckBox->checkState() == Qt::Checked),getPiceSize()*1024);
	
	
}
void CreateTorrentDialog::Cancel()
{
	emit AbortCreation();
	close();
}

void CreateTorrentDialog::ShowCreationSucces(QString filename)
{
	if (!filename.isNull())
	{
		QMessageBox::information(this,QString::fromLocal8Bit(tr("CREATE_TORRENT_DIALOG").toAscii().data()),
			QString::fromLocal8Bit(tr("CREATE_TORRENT_SUCCES_SAVED %1").arg(filename).toAscii().data()));
	}
	progressBar->setValue(0);
	createButton->setEnabled(true);
	delete creator;
	creator = NULL;
}
void CreateTorrentDialog::ShowCreationFailture(QString msg)
{
	QMessageBox::information(this, QString::fromLocal8Bit(tr("CREATE_TORRENT_DIALOG").toAscii().data()),
		QString::fromLocal8Bit(tr("CREATE_TORRENT_FILE_ERROR\n %1").arg(msg).toAscii().data()));
	progressBar->setValue(0);
	createButton->setEnabled(true);
	delete creator;
	creator = NULL;
}
void CreateTorrentDialog::UpdateProgressBar(int val)
{
	if (val<=progressBar->maximum())
		progressBar->setValue(val);
}


//////////////////////////////////torrentCreatorThread\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/torrent_info.hpp"
#include "libtorrent/file.hpp"
#include "libtorrent/storage.hpp"
#include "libtorrent/hasher.hpp"
#include "libtorrent/create_torrent.hpp"
#include "libtorrent/file.hpp"

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

void torrentCreatorThread::create(QString _input_path, QString _save_path,QString _filter, QStringList _trackers, QStringList _url_seeds, QString _comment, bool _is_private, qint64 _piece_size) {
	input_path = _input_path;
	filter=_filter;
	save_path = _save_path;
	trackers = _trackers;
	url_seeds = _url_seeds;
	comment = _comment;
	is_private = _is_private;
	piece_size = _piece_size;
	abort = false;
	//qDebug() << "starting torrentCreatorThread";
	start();
}

void sendProgressUpdateSignal(int i, int num, torrentCreatorThread *parent){
	parent->sendProgressSignal((int)(i*100./(float)num));
}

void torrentCreatorThread::sendProgressSignal(int progress) {
	emit updateProgress(progress);
}
class FileFilter{

public:
	static std::string filterString;
	static bool file_filter(std::string const& f)
	{
		if (filename(f)[0] == '.') return false;
		if (!filterString.empty() && filename(f).find(filterString)!=std::string::npos) return false;
		return true;
	}

};

std::string FileFilter::filterString="";
void torrentCreatorThread::run() {
	emit updateProgress(0);
	char const* creator_str = "CuteTorrent"CT_VERSION;
	try {

		file_storage fs;
		file_pool fp;
		std::string full_path = libtorrent::complete(input_path.toUtf8().data());
		FileFilter::filterString = filter.toStdString();
		add_files(fs, full_path, FileFilter::file_filter);
		if(abort) return;
		create_torrent t(fs, piece_size);


		// Add url seeds
		QString seed;
		foreach(seed, url_seeds){
			t.add_url_seed(seed.toLocal8Bit().data());
		}
		for(int i=0; i<trackers.size(); ++i){
			t.add_tracker(trackers.at(i).toLocal8Bit().data());
		}
		if(abort) return;
		set_piece_hashes(t, parent_path(full_path)
			, boost::bind<void>(&sendProgressUpdateSignal, _1, t.num_pieces(), this));
		if(abort) return;
		t.set_creator(creator_str);
		t.set_comment((const char*)comment.toLocal8Bit());
		t.set_priv(is_private);
		if(abort) return;
		// create the torrent and print it to out
		std::ofstream out(complete((const char*)save_path.toLocal8Bit()).c_str(), std::ios_base::binary);
		bencode(std::ostream_iterator<char>(out), t.generate());

		emit updateProgress(100);
		emit ShowCreationSucces(save_path);
	}
	catch (std::exception& e){
		emit ShowCreationFailture(QString::fromUtf8(e.what()));
	}
	//qDebug() << "torrentCreatorThread finished";
}

