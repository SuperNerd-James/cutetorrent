/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QtGui>
#include "customstyle.h"
CustomStyle::CustomStyle()
{
	
}
void CustomStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
								QPainter *painter, const QWidget *widget) const
{
	if (element == PE_IndicatorSpinUp || element == PE_IndicatorSpinDown) {
		QPolygon points(3);
		int x = option->rect.x();
		int y = option->rect.y();
		int w = option->rect.width() / 2;
		int h = option->rect.height() / 2;
		x += (option->rect.width() - w) / 2;
		y += (option->rect.height() - h) / 2;
		if (element == PE_IndicatorSpinUp) {
			points[0] = QPoint(x, y + h);
			points[1] = QPoint(x + w, y + h);
			points[2] = QPoint(x + w / 2, y);
		} else { // PE_SpinBoxDown
			points[0] = QPoint(x, y);
			points[1] = QPoint(x + w, y);
			points[2] = QPoint(x + w / 2, y + h);
		}
		if (option->state & State_Enabled) {
			painter->setPen(option->palette.mid().color());
			painter->setBrush(option->palette.buttonText());
		} else {
			painter->setPen(option->palette.buttonText().color());
			painter->setBrush(option->palette.mid());
		}
		painter->drawPolygon(points);
	} else {
		QWindowsStyle::drawPrimitive(element, option, painter, widget);
	}
}