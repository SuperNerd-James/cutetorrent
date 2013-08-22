#include "CuteTorentStyle.h"



void CuteTorentStyle::drawControl( ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget /*= 0*/ ) const
{
	if (element == CE_ToolBoxTab) {
		const QStyleOptionToolBox *tb = qstyleoption_cast<const QStyleOptionToolBox *>(option);
		painter->save();
		painter->setBrush(QBrush(Qt::darkBlue));
		painter->drawRoundedRect(option->rect.x() + 10, option->rect.y(), option->rect.width() - 20,
			option->rect.height(), 10.0, 5.0);
		painter->setPen(Qt::white);
		painter->drawText(option->rect.x() + 15, option->rect.y() + 15, tb->text);
		painter->restore();
	} else {
		return QCleanlooksStyle::drawControl(element, option, painter, widget);
	}

}
