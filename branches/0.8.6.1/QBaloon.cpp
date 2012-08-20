
#include <QtGui>
#include "QBaloon.h"
#include <QPropertyAnimation>
static QBalloonTip *theSolitaryBalloonTip = 0;

QWidget *QBalloonTip::showBalloon(const QString& title,
								  const QString& message,const QSystemTrayIcon::MessageIcon& icon,
								  int timeout, bool showArrow,QWidget* parent)
{
	
	hideBalloon();
	theSolitaryBalloonTip = new QBalloonTip(title, message,icon,parent);
	if (timeout < 0)
		timeout = 10000; // �� ��������� �������� ����� 10 ������
	theSolitaryBalloonTip->balloon(timeout, showArrow);
	return theSolitaryBalloonTip;
}

void QBalloonTip::hideBalloon()
{
	if (!theSolitaryBalloonTip)
		return;
	theSolitaryBalloonTip->hide();
	delete theSolitaryBalloonTip;
	theSolitaryBalloonTip = 0;
}

QBalloonTip::QBalloonTip(const QString& title, const QString& message,
						 const QSystemTrayIcon::MessageIcon& icon,QWidget* parent)
						 : QWidget(parent, Qt::ToolTip), timerId(-1)
{
	setAttribute(Qt::WA_DeleteOnClose); // ��� �������� ���� ���������� ������
	
	setObjectName("QBalloonTip" );
	
	QFile File(":/icons/BallonStyle.qss");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QString::fromUtf8(File.readAll().data());
	File.close();
	setStyleSheet(StyleSheet);

	cuurentIcon=icon;
	QLabel *titleLabel = new QLabel;
	titleLabel->installEventFilter(this);
	titleLabel->setText(title);
	QFont f = titleLabel->font();
	f.setBold(true);
	titleLabel->setFont(f);
	titleLabel->setTextFormat(Qt::PlainText);

	QPushButton *closeButton = new QPushButton;
	closeButton->setIcon(style()->standardIcon(QStyle::SP_DockWidgetCloseButton));
	closeButton->setIconSize(QSize(18, 18));
	closeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	closeButton->setFixedSize(18, 18);
	QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	QLabel *msgLabel = new QLabel;
	msgLabel->installEventFilter(this);
	msgLabel->setText(message);
	msgLabel->setTextFormat(Qt::PlainText);
	msgLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	int limit = QApplication::desktop()->availableGeometry(msgLabel).size().width() / 3;
	if (msgLabel->sizeHint().width() > limit) {
		msgLabel->setWordWrap(true);
		msgLabel->setFixedSize(limit, msgLabel->heightForWidth(limit));
	}
	QIcon si;
	switch (icon) {
	 case QSystemTrayIcon::Warning:
		 si = style()->standardIcon(QStyle::SP_MessageBoxWarning);
		 break;
	 case QSystemTrayIcon::Critical:
		 si = style()->standardIcon(QStyle::SP_MessageBoxCritical);
		 break;
	 case QSystemTrayIcon::Information:
		 si = style()->standardIcon(QStyle::SP_MessageBoxInformation);
		 break;
	 case QSystemTrayIcon::NoIcon:
	 default:
		 break;
	}
	QGridLayout *layout = new QGridLayout;
	if (!si.isNull()) {
		         QLabel *iconLabel = new QLabel;
		         iconLabel->setPixmap(si.pixmap(15, 15));
		         iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		         iconLabel->setMargin(2);
		         layout->addWidget(iconLabel, 0, 0);
		   layout->addWidget(titleLabel, 0, 1);
		     } else {
			   layout->addWidget(titleLabel, 0, 0, 1, 2);
			    }
	layout->addWidget(closeButton, 0, 2);
	layout->addWidget(msgLabel, 1, 0, 1, 3);
	layout->setSizeConstraint(QLayout::SetFixedSize);
	layout->setMargin(3);
	setLayout(layout);
	QPropertyAnimation  *anim = new QPropertyAnimation(this, "windowOpacity");
	anim->setDuration(2500);
	anim->setStartValue(0.f);
	anim->setEndValue(1.f);
	anim->start();
//	setPixmap(pixmap);
//setMask(pixmap.mask());
}

QBalloonTip::~QBalloonTip()
{
	theSolitaryBalloonTip = 0;
}

void QBalloonTip::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(rect(), pixmap);
	

}

void QBalloonTip::resizeEvent(QResizeEvent *ev)
{
	QWidget::resizeEvent(ev);
}

void QBalloonTip::balloon(int msecs, bool showArrow)
{
	QSize sh = sizeHint();
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QPoint bottomRight=desktopRect.bottomRight();
	const QPoint& pos=mapToGlobal(QPoint(bottomRight.x(),bottomRight.y()/*-sh.height()*/));
	const QPoint& localpos=QPoint(pos);
	const QRect& prect=desktopRect;

	QRect scr = prect;
	
	const int border = 1;
	const int ah = 18, ao = 18, aw = 18, rc = 7;

	bool arrowAtTop = (localpos.y() - sh.height() - ah < 0);//scr.height());
	bool arrowAtLeft = (localpos.x() + sh.width() - ao < scr.width());
	setContentsMargins(border + 3, border + (arrowAtTop ? ah : 0) + 2, border + 3, border + (arrowAtTop ? 0 : ah) + 2);
	updateGeometry();
	sh = sizeHint();

	int ml, mr, mt, mb;
	QSize sz = sizeHint();
	if (!arrowAtTop) {
		ml = mt = 0;
		mr = sz.width() - 1;
		mb = sz.height() - ah - 1;
	} else {
		ml = 0;
		mt = ah;
		mr = sz.width() - 1;
		mb = sz.height() - 1;
	}

	QPainterPath path;
	path.moveTo(ml + rc, mt);
	if (arrowAtTop && arrowAtLeft) {
		if (showArrow) {
			path.lineTo(ml + ao, mt);
			path.lineTo(ml + ao, mt - ah);
			path.lineTo(ml + ao + aw, mt);
		}
		move(qMax(pos.x() - ao, scr.left() + 2), pos.y());
	} else if (arrowAtTop && !arrowAtLeft) {
		if (showArrow) {
			path.lineTo(mr - ao - aw, mt);
			path.lineTo(mr - ao, mt - ah);
			path.lineTo(mr - ao, mt);
		}
		move(qMin(pos.x() - sh.width() + ao, scr.right() - sh.width() - 2), pos.y());
	}
	path.lineTo(mr - rc, mt);
	path.arcTo(QRect(mr - rc*2, mt, rc*2, rc*2), 90, -90);
	path.lineTo(mr, mb - rc);
	path.arcTo(QRect(mr - rc*2, mb - rc*2, rc*2, rc*2), 0, -90);
	if (!arrowAtTop && !arrowAtLeft) {
		if (showArrow) {
			path.lineTo(mr - ao, mb);
			path.lineTo(mr - ao, mb + ah);
			path.lineTo(mr - ao - aw, mb);
		}
		move(qMin(pos.x() - sh.width() + ao, scr.right() - sh.width() - 2),
			pos.y() - sh.height());
	} else if (!arrowAtTop && arrowAtLeft) {
		if (showArrow) {
			path.lineTo(ao + aw, mb);
			path.lineTo(ao, mb + ah);
			path.lineTo(ao, mb);
		}
		move(qMax(pos.x() - ao, scr.x() + 2), pos.y() - sh.height());
	}
	path.lineTo(ml + rc, mb);
	path.arcTo(QRect(ml, mb - rc*2, rc*2, rc*2), -90, -90);
	path.lineTo(ml, mt + rc);
	path.arcTo(QRect(ml, mt, rc*2, rc*2), 180, -90);

	

	if (msecs > 0)
		timerId = startTimer(msecs);
	show();
}

void QBalloonTip::mousePressEvent(QMouseEvent *e)
{
	close(); // �� ������� �� ��������� ��������� ��
}

void QBalloonTip::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == timerId) {
		killTimer(timerId);
		if (!underMouse())
			close(); // ��������� �� �������
		return;
	}
	QWidget::timerEvent(e);
}