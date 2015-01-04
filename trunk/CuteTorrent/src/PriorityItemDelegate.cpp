#include "PriorityItemDelegate.h"
#include "StaticHelpers.h"

QString PriorityItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if(value.type() == QVariant::Int)
	{
		return StaticHelpers::filePriorityToString(value.toInt());
	}

	
	return "";
}
