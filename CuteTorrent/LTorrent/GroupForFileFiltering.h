#ifndef _GROUPFORFILTER_H
#define _GROUPFORFILTER_H

#include <QSet>
#include <QString>

class GroupForFileFiltering
{
private:
	QSet<QString> extensions;
	QString name,path;
public:
	GroupForFileFiltering(QString name,QString extensions,QString savepath);
	bool Contains(QString extension) const;
	QString SavePath() const;
	QString Name() const;
	QString Extensions() const;

};


#endif