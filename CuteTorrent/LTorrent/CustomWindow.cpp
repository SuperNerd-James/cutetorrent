#include "customwindow.h"

#include <QDesktopWidget>
#include <QPainter>
#include <QBrush>
CustomWindow::CustomWindow(QWidget *parent) : QMainWindow(parent)
{
	
	setWindowFlags(Qt::FramelessWindowHint);
}

CustomWindow::~CustomWindow()
{

}

void CustomWindow::showEvent(QShowEvent *event)
{
	Q_UNUSED(event);

	CenterOnScreen();
}
void CustomWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	QBrush background(QColor(23, 23, 34));

	painter.setBrush(background);
	painter.setPen  (Qt::NoPen ); // No stroke

	painter.drawRect(0, 0, width(), height());
}

void CustomWindow::CenterOnScreen()
{
	QDesktopWidget screen;

	QRect screenGeom = screen.screenGeometry(this);

	int screenCenterX = screenGeom.center().x();
	int screenCenterY = screenGeom.center().y();

	move(screenCenterX - width () / 2,
		screenCenterY - height() / 2);
}