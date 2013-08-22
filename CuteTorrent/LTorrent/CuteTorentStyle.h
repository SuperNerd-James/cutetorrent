#pragma once
#include <QCleanlooksStyle>
#include <QPainter>
#include <QStyleOption>
class CuteTorentStyle : public QCleanlooksStyle
{
public:
	CuteTorentStyle(void) : QCleanlooksStyle(){};
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
		const QWidget *widget = 0) const;
	
};
