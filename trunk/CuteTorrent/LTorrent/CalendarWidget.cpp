#include "CalendarWidget.h"

CalendarWidget::CalendarWidget(QWidget *parent/* = 0*/) : QCalendarWidget(parent)
{
	view = qFindChild<QTableView *>(this);
	view->viewport()->installEventFilter(this);
}

bool CalendarWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (obj->parent() && obj->parent() == view) 
	{
		if (event->type() == QEvent::MouseButtonPress ||
			event->type() == QEvent::MouseButtonRelease) 
		{
				QMouseEvent *me = static_cast<QMouseEvent*>(event);
				QPoint pos = me->pos();
				if (event->type() == QEvent::MouseButtonPress &&
					!(me->modifiers() & Qt::ShiftModifier)) 
				{
						QModelIndex idx = view->indexAt(pos);
						if (idx.row() != 0 && idx.column() != 0)
							startIndex = idx;
				} 
				else if (event->type() == QEvent::MouseButtonRelease &&
					me->modifiers() & Qt::ShiftModifier) 
				{
						QModelIndex idx = view->indexAt(pos);
						if (idx.row() != 0 && idx.column() != 0)
							endIndex = idx;
						else
							return false;
						if (!startIndex.isValid())
							startIndex =
							view->selectionModel()->selectedIndexes().first();
						endIndex = view->indexAt(pos);
						int rowStart = startIndex.row();
						int rowEnd = endIndex.row();
						int colStart = startIndex.column();
						int colEnd = endIndex.column();
						QItemSelection sel;
						for (int i=rowStart;i<=rowEnd;i++) 
						{
							if (i == rowStart && i != rowEnd) 
							{
								for (int j=colStart;
									j<view->model()->columnCount();j++)
									view->selectionModel()->select(
									view->model()->index(i, j),
									QItemSelectionModel::Select);
							} 
							else if (i == rowEnd) 
							{
								int start = (i == rowStart) ? colStart : 1;
								for (int j = start;j<colEnd;j++)
									view->selectionModel()->select(
									view->model()->index(i, j),
									QItemSelectionModel::Select);
							}
							else 
							{
								for (int j=1;j<view->model()->columnCount();j++)
									view->selectionModel()->select(
									view->model()->index(i, j),
									QItemSelectionModel::Select);
							}
						}
						view->selectionModel()->select(endIndex,
							QItemSelectionModel::Select);
						emit selectionChanged();
						return true;
				}
		}
		return false;
	} 
	else 
	{
		return QCalendarWidget::eventFilter(obj, event);
	}
}

QPair<QDateTime,QDateTime> CalendarWidget::getSelectedInterval()
{
	QDateTime start,end;
	QDate tmp=selectedDate();
	start.setDate(tmp);
	int first=startIndex.data().toInt(),second=endIndex.data().toInt();
	if (first > second)
	{
		tmp=tmp.addMonths(1);
	}
	tmp.setDate(tmp.year(),tmp.month(),second);
	end.setDate(tmp);
	if (start > end)
	{
		qSwap(start,end);
	}
	return qMakePair<QDateTime,QDateTime>(start,end);
}

