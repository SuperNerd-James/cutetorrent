#include "peicedisplaywidget.h"
#include <QPainter>
#include <QDebug>
PeiceDisplayWidget::PeiceDisplayWidget(QWidget *parent)
	: QWidget(parent)
{
	this->setMinimumHeight(20);
	piceCount=0;
	setStyleSheet("border-color: rgb(216, 216, 216);");
}

PeiceDisplayWidget::~PeiceDisplayWidget()
{

}

void PeiceDisplayWidget::paintEvent( QPaintEvent * paintEvent)
{
	
    QPainter painter(this);
    if (img.isNull())
    {
        painter.fillRect(0,0,width()-1,height() - 1,Qt::white);
    }
    else
    {
        painter.drawImage(0,0,img);
    }
    painter.setPen(QColor(0x90,0x90,0x90));
	painter.drawRect(0, 0, width() - 1, height() - 1);

}

void PeiceDisplayWidget::UpdateImage()
{
    QImage img2(width()-1,height()-1,QImage::Format_RGB32);
    QPainter painter(&img2);
	painter.fillRect(0, 0, width() - 1, height() - 1, Qt::white);
    if (piceCount!=0)
    {
        double pieceItemWidth = width()*1.0/piceCount;
        double pieceItemHeight = height();
        int i=0;
        for (i=0;i<dowloadedParts.size();i++)
         {
            QRectF currentRect(dowloadedParts[i].first*pieceItemWidth,0,(dowloadedParts[i].second-dowloadedParts[i].first)*pieceItemWidth,pieceItemHeight);
            painter.fillRect(currentRect,QColor(0x32,0x91,0xd4));
        }
        for (i=0;i<dowloadingParts.size();i++)
        {
            QRectF currentRect(dowloadingParts[i].first*pieceItemWidth,0,(dowloadingParts[i].second-dowloadingParts[i].first)*pieceItemWidth,pieceItemHeight);
            painter.fillRect(currentRect,QColor(82,211,49));
        }
    }
    img=img2;


}

void PeiceDisplayWidget::setPiceCount( int _pieceCount)
{
	piceCount=_pieceCount;
    UpdateImage();
}

void PeiceDisplayWidget::setDowloadingParts( QVector<int> parts )
{
	dowloadingParts.clear();
	int previusPicNumber=-1;
	foreach(int piceNumber,parts)
	{
		//qDebug() << piceNumber << previusPicNumber;
        if(piceNumber-previusPicNumber==1)
        {
            dowloadingParts.last().second=piceNumber;

        }
        else
        {
            dowloadingParts.append(qMakePair(piceNumber,piceNumber+1));
        }

        previusPicNumber=piceNumber;
	}
    /*//qDebug() << parts;
    //qDebug() << dowloadingParts;*/
}

void PeiceDisplayWidget::setDowloadedParts( QVector<int> parts )
{
	dowloadedParts.clear();
	int previusPicNumber=0;
    foreach(int piceNumber,parts)
    {
		//qDebug() << piceNumber << previusPicNumber;
        if(piceNumber-previusPicNumber==1)
        {
            dowloadedParts.last().second=piceNumber;

        }
        else
        {
            dowloadedParts.append(qMakePair(piceNumber,piceNumber+1));
        }
        previusPicNumber=piceNumber;
    }
    /*//qDebug() << parts;
    //qDebug() << dowloadedParts;*/
}
