#ifndef PEICEDISPLAYWIDGET_H
#define PEICEDISPLAYWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>
class PeiceDisplayWidget : public QWidget
{
	Q_OBJECT

public:
	PeiceDisplayWidget(QWidget *parent = 0);
	~PeiceDisplayWidget();
	void setDowloadedParts(QVector<int> parts);
	void setDowloadingParts(QVector<int> parts);
	void setPiceCount(int);
protected:
	void paintEvent(QPaintEvent *);
private:
    void UpdateImage();
    QImage img;
	int piceCount;
    QVector<QPair<int,int> > dowloadingParts;
    QVector<QPair<int,int> > dowloadedParts;

};

#endif // PEICEDISPLAYWIDGET_H
