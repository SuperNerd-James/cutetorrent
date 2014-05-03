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
#include <QPainter>
#include "messagebox.h"
#include "StyleEngene.h"
CreateTorrentDialog::CreateTorrentDialog(QWidget *parent, Qt::WindowFlags flags) : QDialog(parent,flags)
{
	setupUi(this);
	
	setupCustomWindow();
    setupWindowIcons();
    settings = QApplicationSettings::getInstance();
	creator  = new torrentCreatorThread(this);
	//qDebug() << "new torrentCreatorThread";
	mgr = TorrentManager::getInstance();
    StyleEngene* style = StyleEngene::getInstance();
    QObject::connect(style,SIGNAL(styleChanged()),this,SLOT(setupWindowIcons()));
}

void CreateTorrentDialog::setupWindowIcons()
{
    StyleEngene* style = StyleEngene::getInstance();
    pbMin->setIcon(style->getIcon("app_min"));
    pbClose->setIcon(style->getIcon("app_close"));

}
void CreateTorrentDialog::setupCustomWindow()
{
	setAttribute(Qt::WA_DeleteOnClose);
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

	addAction(actionClose);

	connect(pbMin, SIGNAL(clicked()), this, SLOT(minimizeBtnClicked()));
	connect(pbClose, SIGNAL(clicked()), this, SLOT(close()));

	//Per poder rebre les dades del ratol� sense haver de clicar cap bot�
	m_titleMode = FullTitle;
	moveWidget = false;
	inResizeZone = false;
	allowToResize = false;
	resizeVerSup = false;
	resizeHorEsq = false;
	resizeDiagSupEsq = false;
	resizeDiagSupDer = false;
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


void CreateTorrentDialog::mouseMoveEvent(QMouseEvent *e)
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

void CreateTorrentDialog::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (inResizeZone)
		{
            //allowToResize = true;

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
		}
	}

	e->accept();
}

void CreateTorrentDialog::mouseReleaseEvent(QMouseEvent *e)
{
	moveWidget = false;
	allowToResize = false;
	resizeVerSup = false;
	resizeHorEsq = false;
	resizeDiagSupEsq = false;
	resizeDiagSupDer = false;

	e->accept();
}

void CreateTorrentDialog::mouseDoubleClickEvent(QMouseEvent *e)
{
	QPoint pos = e->pos();
	QRect geometry = tbMenu->geometry();
	if (pos.x() < geometry.right()&&pos.y() < geometry.bottom()
		&&pos.x() >=  geometry.x()&&pos.y() >= geometry.y()
		&&tbMenu->isVisible())
		close();
	e->accept();
}

void CreateTorrentDialog::paintEvent (QPaintEvent *)
{
	QStyleOption opt;
	opt.init (this);
	QPainter p(this);
	style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void CreateTorrentDialog::moveWindow(QMouseEvent *e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		move(e->globalPos() - dragPosition);
		e->accept();
	}
}





void CreateTorrentDialog::minimizeBtnClicked()
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
void CreateTorrentDialog::resizeWindow(QMouseEvent *e)
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
CreateTorrentDialog::~CreateTorrentDialog()
{
    QApplicationSettings::FreeInstance();
	creator->deleteLater();
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
			quint64 pieceSize=0;
			quint64 needToSet=0;

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
			//qDebug() << needToSet;
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
	path = QFileDialog::getExistingDirectory(this, tr("DIALOG_OPEN_FOLDER"),
                                             settings->valueString("System","LastSaveTorrentDir"),
                                             QFileDialog::ShowDirsOnly
                                             | QFileDialog::DontResolveSymlinks);
	pathEdit->setText(path);
}
void CreateTorrentDialog::BrowseFile()
{
	path =  QFileDialog::getOpenFileName(this,
        tr("DIALOG_OPEN_FILE"),settings->valueString("System","LastSaveTorrentDir") , tr("Any File (*.*)"));
	pathEdit->setText(path);
}
void CreateTorrentDialog::BeginCreate()
{
    path = pathEdit->text();
	if ((QFileInfo(path).isDir() && listFolder(path)==0) || (!QFileInfo(path).isDir() && QFileInfo(path).size()==0))
	{
        MyMessageBox::warning(this, tr("ERROR_STR"),
			tr("ERROR_EMPTY_DIR"));
		return;
	}
	createButton->setEnabled(false);
	
	
	QStringList trackers=trackerEdit->toPlainText().split('\n');
	QStringList webseeds=webSeedEdit->toPlainText().split('\n');
	
	if (path.length()==0)
    {	MyMessageBox::information(this, tr("ERROR_STR"),
		tr("ERROR_NO_FILE_OR_FOLDER_NAME"));
		//delete creator;
		createButton->setEnabled(true);
		return;
	}
	

	
	for (QStringList::iterator i=trackers.begin();i!=trackers.end();++i)
	{
		if ((*i).isEmpty())
			trackers.removeOne(*i);
	}
	if (trackers.count()==0)
    {	if (QMessageBox::No == MyMessageBox::information(this, tr("ERROR_STR"),
		tr("ERROR_NO_TRACKERS"),
		QMessageBox::Yes | QMessageBox::No))
		{
			//delete creator;
			createButton->setEnabled(true);
			return;
		}
	}
	//qDebug() << "before removing web seeds";
	for (QStringList::iterator i=webseeds.begin();i!=webseeds.end();++i)
	{
		if ((*i).isEmpty())
			webseeds.removeOne(*i);
	}
	
	QFileInfo info(path);
	//qDebug() << "after removing web seeds";
	QString save_path = QFileDialog::getSaveFileName(this,
		tr("CREATE_TORRENT_DIALOG"),info.isDir() ? path+QDir::separator()+info.fileName(): path+".torrent",
		tr("������� ����� (*.torrent)"));
	if (!save_path.isEmpty())
	{
		
		QObject::connect(creator,SIGNAL(updateProgress(int)),this,SLOT(UpdateProgressBar(int)));
		QObject::connect(creator,SIGNAL(ShowCreationSucces(QString)),this,SLOT(ShowCreationSucces(QString)));
		QObject::connect(creator,SIGNAL(ShowCreationFailture(QString)),this,SLOT(ShowCreationFailture(QString)));
		QObject::connect(this,SIGNAL(AbortCreation()),creator,SLOT(terminate()));
		//qDebug() << "connects";
		
		creator->create(pathEdit->text(),save_path,filterEdit->text(),trackers,webseeds,discribtionEdit->text(),privateCheckBox->isChecked(),getPiceSize()*1024);
	}
	else
		createButton->setEnabled(true);
	
	
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
        MyMessageBox::information(this,tr("CREATE_TORRENT_DIALOG"),
			tr("CREATE_TORRENT_SUCCES_SAVED %1").arg(filename));
	}
	progressBar->setValue(0);
    createButton->setEnabled(true);
	//delete creator;
	//creator = NULL;
}
void CreateTorrentDialog::ShowCreationFailture(QString msg)
{
    MyMessageBox::critical(this, tr("CREATE_TORRENT_DIALOG"),
		tr("CREATE_TORRENT_FILE_ERROR\n %1").arg(msg));
	progressBar->setValue(0);
	createButton->setEnabled(true);
// 	delete creator;
// 	creator = NULL;
}
void CreateTorrentDialog::UpdateProgressBar(int val)
{
	if (val<=progressBar->maximum())
		progressBar->setValue(val);
}


////torrentCreatorThread\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

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
		std::string full_path = libtorrent::complete(input_path.toStdString());
		FileFilter::filterString = filter.toStdString();
		add_files(fs, full_path, FileFilter::file_filter);
		if(abort) return;
		create_torrent t(fs, piece_size);


		// Add url seeds
		QString seed;
		foreach(seed, url_seeds){
			t.add_url_seed(seed.toStdString());
		}
		for(int i=0; i<trackers.size(); ++i){
			t.add_tracker(trackers.at(i).toStdString());
		}
		if(abort) return;
		set_piece_hashes(t, parent_path(full_path)
			, boost::bind<void>(&sendProgressUpdateSignal, _1, t.num_pieces(), this));
		if(abort) return;
		t.set_creator(creator_str);
		t.set_comment((const char*)comment.toLocal8Bit());
		t.set_priv(is_private);
		if(abort) return;
        qDebug() << save_path.toLatin1().data();
		// create the torrent and print it to out
    #ifdef _MSC_VER
        wchar_t *wsave_path = new wchar_t[save_path.length()+1];
        int len = save_path.toWCharArray(wsave_path);
        wsave_path[len] = '\0';
        std::ofstream outfile(wsave_path, std::ios_base::out|std::ios_base::binary);
        delete[] wsave_path;
    #else
        std::ofstream outfile(save_path.toLocal8Bit().constData(), std::ios_base::out|std::ios_base::binary);
    #endif
        bencode(std::ostream_iterator<char>(outfile), t.generate());

		emit updateProgress(100);
		emit ShowCreationSucces(save_path);
	}
	catch (std::exception& e){
		emit ShowCreationFailture(QString::fromUtf8(e.what()));
	}
	//qDebug() << "torrentCreatorThread finished";
}

