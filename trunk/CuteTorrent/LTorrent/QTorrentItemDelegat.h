
#ifndef _ITEM_DELEGATE_H
#define _ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QSize>
#include <QStyle>
#include <QString>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QModelIndex>
#include "Torrent.h"

class Torrent;

class QTorrentItemDelegat: public QStyledItemDelegate
{
	Q_OBJECT
 protected:
	 QStyleOptionProgressBar * myProgressBarStyle;
	
	

protected:

     QSize margin( const QStyle& style ) const;
	 virtual QSize sizeHint( const QStyleOptionViewItem&, const Torrent& ) const;
	 virtual void setProgressBarPercentDone( const QStyleOptionViewItem& option, const Torrent& ) const;
	 virtual void drawTorrent( QPainter* painter, const QStyleOptionViewItem& option,const Torrent& , int row) const;
private:
	QString GetStatusString(const Torrent& tor) const;
	QString GetProgressString(const Torrent& tor) const;
public:
	QTorrentItemDelegat();
	QTorrentItemDelegat::QTorrentItemDelegat(const QTorrentItemDelegat &);
	static  QColor blueBrush, greenBrush;
     static QColor blueBack,  greenBack;
	 explicit QTorrentItemDelegat( QObject * parent );
	 virtual ~QTorrentItemDelegat( );
	
	 QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	 void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

Q_DECLARE_METATYPE(QTorrentItemDelegat)

#endif