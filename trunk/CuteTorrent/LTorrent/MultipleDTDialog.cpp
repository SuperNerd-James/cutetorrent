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

#include "MultipleDTDialog.h"
#include "QApplicationSettings.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QProcess>
#include <QDebug>
#include <QAbstractItemView>
#include "StaticHelpers.h"
#include "DT_mounter.h"
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include "StyleEngene.h"
MultipleDTDialog::MultipleDTDialog(QStringList& _files,QWidget* parrent/* =0 */,int flags/* =0 */)
{
	setupUi(this);
    setupCustomWindow();
    setupWindowIcons();
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(MountSelectedFILE()));
	files.append(_files);
	
	
	
	
	//qDebug() << "files set";
	QStandardItemModel *model = new QStandardItemModel(files.count(),1);
	listView->setModel(model);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	QString temp; 
    QIcon iso = StyleEngene::getInstance()->guessMimeIcon("iso",temp);
	for (int i=0;i<files.count();i++)
	{
		QStandardItem * item = new QStandardItem(iso,files.at(i));
		model->setItem(i,item);
	}
     StyleEngene* style = StyleEngene::getInstance();
     QObject::connect(style,SIGNAL(styleChanged()),this,SLOT(setupWindowIcons()));
}

void MultipleDTDialog::setupWindowIcons()
{
    StyleEngene* style = StyleEngene::getInstance();
    pbMin->setIcon(style->getIcon("app_min"));
    pbClose->setIcon(style->getIcon("app_close"));

}
void MultipleDTDialog::MountSelectedFILE()
{
	int selectedRow=listView->selectionModel()->selectedIndexes().first().row();
	Mount(files.at(selectedRow));
}


void MultipleDTDialog::Mount( QString filename )
{
	DT_mounter::mountImage(filename);
}

void MultipleDTDialog::changeEvent( QEvent *event )
{
	if (event->type()==QEvent::LanguageChange)
	{
		retranslateUi(this);
	}
}

void MultipleDTDialog::setupCustomWindow()
{
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

//    addAction(actionClose);

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


void MultipleDTDialog::mouseMoveEvent(QMouseEvent *e)
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

void MultipleDTDialog::mousePressEvent(QMouseEvent *e)
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

void MultipleDTDialog::mouseReleaseEvent(QMouseEvent *e)
{
    moveWidget = false;
    allowToResize = false;
    resizeVerSup = false;
    resizeHorEsq = false;
    resizeDiagSupEsq = false;
    resizeDiagSupDer = false;

    e->accept();
}

void MultipleDTDialog::mouseDoubleClickEvent(QMouseEvent *e)
{
	QRect geometry = tbMenu->geometry();
	QPoint point = e->pos();
    if (point.x() < geometry.right()&&point.y() < geometry.bottom()
        &&point.x() >=  geometry.x()&&point.y() >= geometry.y()
        &&tbMenu->isVisible())
        close();
    e->accept();
}

void MultipleDTDialog::paintEvent (QPaintEvent *)
{
    QStyleOption opt;
    opt.init (this);
    QPainter p(this);
    style()->drawPrimitive (QStyle::PE_Widget, &opt, &p, this);
}

void MultipleDTDialog::moveWindow(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - dragPosition);
        e->accept();
    }
}





void MultipleDTDialog::minimizeBtnClicked()
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
void MultipleDTDialog::resizeWindow(QMouseEvent *e)
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
