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
#include <QTextCodec>
#include <QMovie>
#include <QMouseEvent>
#include <QPainter>
#define PIXELS_TO_ACT 2
OpenTorrentDialog::OpenTorrentDialog(QWidget *parent, Qt::WindowFlags flags) : useGroup(false)
{
	setupUi(this);
	setupGroupComboBox();
    setupCustomeWindow();
	mgr=TorrentManager::getInstance();
	validTorrent=true;
	model=NULL;
	QTextCodec *wantUnicode = QTextCodec::codecForName("UTF-8");
	/*QTextCodec::setCodecForTr(wantUnicode);
	QTextCodec::setCodecForLocale(wantUnicode);*/
    QTextCodec::setCodecForLocale(wantUnicode);
	
}
void OpenTorrentDialog::setupGroupComboBox()
{


}

void OpenTorrentDialog::mouseMoveEvent(QMouseEvent *e)
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

void OpenTorrentDialog::mousePressEvent(QMouseEvent *e)
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

void OpenTorrentDialog::mouseReleaseEvent(QMouseEvent *e)
{
    moveWidget = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;

    e->accept();
}

void OpenTorrentDialog::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->pos().x() < tbMenu->geometry().right()&&e->pos().y() < tbMenu->geometry().bottom()
        &&e->pos().x() >=  tbMenu->geometry().x()&&e->pos().y() >= tbMenu->geometry().y()
        &&tbMenu->isVisible())
        close();
    e->accept();
}

void OpenTorrentDialog::paintEvent (QPaintEvent *)
{
    QStyleOption opt;
    opt.init (this);
    QPainter p(this);
    style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void OpenTorrentDialog::moveWindow(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - dragPosition);
        e->accept();
    }
}





void OpenTorrentDialog::minimizeBtnClicked()
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
void OpenTorrentDialog::resizeWindow(QMouseEvent *e)
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
void OpenTorrentDialog::setupCustomeWindow()
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
	if (model!=NULL)
		delete model;
}



void OpenTorrentDialog::SetData(QString filename)
{
	torrentFilename=filename;
	if (filename.startsWith("magnet"))
	{
		QMovie *movie = new QMovie(":/images/loader.gif");
		loaderGifLabel->setMovie(movie);
		movie->start();
		qRegisterMetaType<openmagnet_info>("openmagnet_info");
		MetaDataDownloadWaiter* magnetWaiter = new MetaDataDownloadWaiter(filename);
		if (!QObject::connect(magnetWaiter,SIGNAL(DownloadCompleted(openmagnet_info)),this,SLOT(DownloadMetadataCompleted(openmagnet_info))))
			QMessageBox::critical(this,"ERROR","NOT_CONNECTID");
		magnetWaiter->start(QThread::HighPriority);
		yesButton->setEnabled(false);
	}
	else
	{
		loaderGifLabel->hide();		
		loaderTextLabel->hide();		
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
				i++)
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
			
            torrentFilesTreeView->setModel(model);
            torrentFilesTreeView->setColumnWidth(0,300);
			

			setUpdatesEnabled( true );
			if (!info->base_suffix.isEmpty())
			{
				//qDebug() << "QApplicationSettings::getInstance from OpenTorrentDialog::SetData";
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
                {
                    GroupComboBox->setCurrentIndex(selected);
                }
                else
                {
                    QApplicationSettings* settings=QApplicationSettings::getInstance();
                    QString lastDir=settings->valueString("System","LastSaveTorrentDir");
                    pathEdit->setText(lastDir);
                    QApplicationSettings::FreeInstance();
                }
				//qDebug() << "QApplicationSettings::FreeInstance from OpenTorrentDialog::SetData";
				QApplicationSettings::FreeInstance();
			} 
			delete info;
		}
		else
		{
			validTorrent=false;
		}
	}
}



void OpenTorrentDialog::BrowseButton()
{
	//qDebug() << "QApplicationSettings::getInstance from OpenTorrentDialog::BrowseButton";
	QApplicationSettings* settings=QApplicationSettings::getInstance();
    QString lastDir=pathEdit->text();
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
		useGroup=false;
	}
	//qDebug() << "QApplicationSettings::FreeInstance from OpenTorrentDialog::BrowseButton";
	QApplicationSettings::FreeInstance();
}



void OpenTorrentDialog::AccepTorrent()
{
	if (validTorrent)
	{
		QFile file(torrentFilename);
		QMap<QString,int> filePriorities=model->getFilePiorites();
		error_code ec;
		QString group=useGroup ? filters[GroupComboBox->currentIndex()].Name(): "" ;
		if (!torrentFilename.startsWith("magnet"))
		{
            mgr->AddTorrent(torrentFilename,pathEdit->displayText(),ec,filePriorities,group,setSequntialCheckBox->isChecked());
		}
		else
		{
			mgr->AddMagnet(_info.handle,pathEdit->displayText(),group,filePriorities);
		}
		if (ec)
		{
			QMessageBox::critical(this,"Adding torrent Error",QString::fromStdString(ec.message()));
			return;
		}
	}
	accept ();
}

void OpenTorrentDialog::ChangeGroup()
{
	if (GroupComboBox->currentIndex()>=0 && GroupComboBox->currentIndex()<filters.length())
	{
		pathEdit->setText(filters[GroupComboBox->currentIndex()].SavePath());
		useGroup=true;
	}
}

void OpenTorrentDialog::DownloadMetadataCompleted(openmagnet_info info)
{
	//qDebug() << "OpenTorrentDialog::DownloadMetadataCompleted";
	loaderGifLabel->hide();		
	loaderTextLabel->hide();
	yesButton->setEnabled(true);
	_info=info;
	setUpdatesEnabled( false );
	//qDebug() << "accesing info.name";
	labelNameData->setText(info.name);
	//qDebug() << "accesing info.describtion";
	labelComentData->setText(info.describtion);
	//qDebug() << "accesing info.size";
	labelSizeData->setText(StaticHelpers::toKbMbGb(info.size));
	QStringList files;
	//qDebug() << "building file lsit";
	for (libtorrent::file_storage::iterator i = info.files.begin(); 
		i != info.files.end();
		i++)
	{
		files << QString::fromUtf8(info.files.file_path(*i).c_str())+"|"+StaticHelpers::toKbMbGb(info.files.file_size(*i));
	}
	files.sort();
	//qDebug() << "Creating file tree model";
	model = new FileTreeModel();
	for (int i=0;i<files.count();i++)
	{
		//qDebug() << "Adding to file tree model "+files.at(i);
		QStringList parts= files.at(i).split('|');
		if (parts.count() >= 1)
		{
			model->addPath(parts.at(0),parts.at(1));
		}

	}
	//qDebug() << "Setting file tree model";
    torrentFilesTreeView->setModel(model);
    torrentFilesTreeView->setColumnWidth(0,300);


	setUpdatesEnabled( true );
	//qDebug() << "base_suffix stuf started";
	if (!info.base_suffix.isEmpty())
	{
		try
		{
			//qDebug() << "QApplicationSettings::getInstance from  OpenTorrentDialog::DownloadMetadataCompleted";
			QApplicationSettings* instance= QApplicationSettings::getInstance();
			
			
			filters=instance->GetFileFilterGroups();
			
			int selected=-1;
			for (int i=0;i<filters.count();i++)
			{
				GroupComboBox->addItem(filters[i].Name());

				if (filters.at(i).Contains(info.base_suffix) && selected<0)
				{
					selected=i;
					pathEdit->setText(filters.at(i).SavePath());
				}
			}
			if (selected>=0)
				GroupComboBox->setCurrentIndex(selected);
			
		
		}
		catch (std::exception ex)
		{
			//qDebug() << ex.what();
		}
		//qDebug() << "QApplicationSettings::FreeInstance from  OpenTorrentDialog::DownloadMetadataCompleted";
		QApplicationSettings::FreeInstance();
		
	} 
}

void OpenTorrentDialog::changeEvent( QEvent *event )
{
	if (event->type()==QEvent::LanguageChange)
	{
		retranslateUi(this);
	}
}
