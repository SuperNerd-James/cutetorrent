#ifndef _QAPPSETTINGS
#define _QAPPSETTINGS

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QMutex>
#include "GroupForFileFiltering.h"

// support only one sub level in settings
class QApplicationSettings
{
protected:
	
	static QApplicationSettings* _instance;
	static int _instanceCount;
	QApplicationSettings();
	~QApplicationSettings();
private:
	QSettings* settings;
	QMutex* locker;
	QMap<QString,QMap<QString,QVariant> > settingsStorrage;
	void ReedSettings();
	void WriteSettings();
	QVariant value(const QString group,const QString key);
	
public:
	void SaveFilterGropups(QList<GroupForFileFiltering>);
	QList<GroupForFileFiltering> GetFileFilterGroups();
	static void FreeInstance();
	static QApplicationSettings* getInstance();
	QStringList GetGroupNames();
	void setValue(const QString group, const QString key, const QVariant &value);
	int valueInt(const QString group,const QString key, int defalt = 0);
	QString valueString(const QString group,const QString key,QString defalt = "");
	bool valueBool(const QString group,const QString key,bool defalt=true);

};



#endif