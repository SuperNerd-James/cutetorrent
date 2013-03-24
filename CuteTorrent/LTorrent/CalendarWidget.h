#ifndef  _CALENDAR_WIDGET_H
#define _CALENDAR_WIDGET_H
#include <QtGui>
#include <QCalendarWidget>
class QCalendarModel;
class CalendarWidget :	public QCalendarWidget
{
public:
	CalendarWidget(QWidget *parent = 0);
	bool eventFilter(QObject *obj, QEvent *event);
	QPair<QDateTime,QDateTime> getSelectedInterval();
private:
	QTableView *view;
	QPersistentModelIndex startIndex;
	QPersistentModelIndex endIndex;
};
#endif //_CALENDAR_WIDGET_H