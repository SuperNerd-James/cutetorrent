#ifndef QBALLOONTIP_H
#define QBALLOONTIP_H
#include <QWidget>
class QBalloonTip : public QWidget
{
public:
	static QWidget *showBalloon(const QString& title,
		const QString& msg,const QSystemTrayIcon::MessageIcon& icon=QSystemTrayIcon::Information ,
		int timeout=0, bool showArrow = true,QWidget* parrent=0);
	static void hideBalloon();

private:
	QBalloonTip(const QString& title,const QString& msg,
		const QSystemTrayIcon::MessageIcon& icon, QWidget* parent=0);
	~QBalloonTip();
	void balloon(int, bool);

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void mousePressEvent(QMouseEvent *e);
	void timerEvent(QTimerEvent *e);

private:
	QSystemTrayIcon::MessageIcon cuurentIcon;
	QPixmap pixmap;
	int timerId;
};
#endif