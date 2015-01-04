#pragma once
#include <QStyledItemDelegate>
#include <QVariant>
class PriorityItemDelegate :
    public QStyledItemDelegate
{
public:
    PriorityItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {};

    QString displayText(const QVariant& value, const QLocale& locale) const;


};
