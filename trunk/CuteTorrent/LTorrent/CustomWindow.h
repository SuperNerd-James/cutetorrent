
#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QtGui/QMainWindow>
class CustomWindow : public QMainWindow
{
	Q_OBJECT

public:
	CustomWindow(QWidget *parent = 0);
	~CustomWindow();

protected:
	void paintEvent(QPaintEvent *event);
	void showEvent(QShowEvent *event);

private:
	void CenterOnScreen();
};
#endif