#include "ProgressItemDelegate.h"
#include <QDebug>


QString ProgressItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if(value.type() == QMetaType::Float)
	{
		return QString::number(value.toFloat(), 'f', 2) + " %";
	}

	return "";
}
