#include "QApplicationSettings.h"
#include <QMessageBox>
#include <QDebug>

QApplicationSettings* QApplicationSettings::_instance = NULL;
int QApplicationSettings::_instanceCount = 0;

QApplicationSettings::QApplicationSettings()
{
	settings = new QSettings("SevLan","CuteTorrent");
	
	locker= new QMutex();
	ReedSettings();
}
QApplicationSettings::~QApplicationSettings()
{
	qDebug() << "QApplicationSettings: object destruction";
	WriteSettings();

}
QApplicationSettings* QApplicationSettings::getInstance()
{
	
	if (_instance==NULL)
		_instance = new QApplicationSettings();
	qDebug() << "QApplicationSettings giving " <<_instanceCount<< " instance " ;
	_instanceCount++;
	return _instance;
}
void QApplicationSettings::FreeInstance()
{
	
	_instanceCount--;
	qDebug() << "QApplicationSettings freeing " <<_instanceCount<< " instance " ;
	if (!_instanceCount)
	{
		_instance->~QApplicationSettings();
		_instance=NULL;
	}
	
}
QStringList QApplicationSettings::GetGroupNames()
{
	locker->lock();
	QStringList res=settings->childGroups();
	locker->unlock();
	return res;
}

void QApplicationSettings::setValue(const QString group,const QString key,const QVariant &value)
{
	locker->lock();
	settingsStorrage[group][key]=value;
	qDebug() << "QApplicationSettings::setValue " << group << " " << key << " " << value;
	locker->unlock();
}

QVariant QApplicationSettings::value(const QString group,const QString key)
{
	locker->lock();
	QVariant res;
	if (settingsStorrage.contains(group))
		if (settingsStorrage[group].contains(key))
			res=settingsStorrage[group][key];
	locker->unlock();
	return res;
}
int QApplicationSettings::valueInt(const QString group,const QString key,int defalt)
{
	QVariant val=value(group,key);
	if (val.isNull())
		return defalt;
	else
		return val.toInt();
}
QString	QApplicationSettings::valueString(const QString group,const QString key,QString defalt)
{
	QVariant val=value(group,key);
	if (val.isNull())
		return defalt;
	else
		return val.toString();
}

bool QApplicationSettings::valueBool(const QString group,const QString key,bool defalt)
{
	QVariant val=value(group,key);
	if (val.isNull())
		return defalt;
	else
		return val.toBool();
}
void  QApplicationSettings::ReedSettings()
{
	
	QStringList rootGroups=settings->childGroups();
	for(int i=0;i<rootGroups.size();i++)
	{
		
		settings->beginGroup(rootGroups.at(i));
		QStringList keys= settings->childKeys();
		for (int j=0;j<keys.size();j++)
		{
			
			settingsStorrage[rootGroups.at(i)][keys.at(j)]=settings->value(keys.at(j));
		}
		settings->endGroup();
	}
	
}
void QApplicationSettings::SaveFilterGropups(QList<GroupForFileFiltering> filters)
{
	locker->lock();
	settings->beginGroup("FileFiltering");
	settings->remove("");
	for (int i=0;i<filters.count();i++)
	{
		GroupForFileFiltering group = filters.at(i);
		settings->setValue(group.Name(),QString("savepath;;%1;;extensions;;%2").arg(group.SavePath()).arg(group.Extensions()));
	}
	settings->endGroup();
	locker->unlock();
}
QList<GroupForFileFiltering> QApplicationSettings::GetFileFilterGroups()
{
	QList<GroupForFileFiltering> res;
	locker->lock();
	settings->beginGroup("FileFiltering");
	QStringList groupNames=settings->childKeys();
	for (int i=0;i<groupNames.count();i++)
	{
		QStringList tmpVal= settings->value(groupNames.at(i)).toString().split(";;");
		if (tmpVal.at(0)!="savepath" && tmpVal.count()<4)
			continue;
		else
		{
			QString savepath=tmpVal.at(1);
			QString extensions = tmpVal.at(3);
			res.append(GroupForFileFiltering(groupNames.at(i),extensions,savepath));
		}
	}
	settings->endGroup();
	locker->unlock();
	return res;
}
void  QApplicationSettings::WriteSettings()
{
	
	QMap<QString, QMap<QString,QVariant> >::const_iterator i = settingsStorrage.constBegin();
	while (i != settingsStorrage.constEnd())
	{
		if (i.key()=="FileFiltering")
		{
			i++;
			continue;
		}
		settings->beginGroup(i.key());
		QMap<QString,QVariant>::const_iterator j=i.value().constBegin();
		while (j != i.value().constEnd())
		{
			qDebug() << QString("[%1] %2 %3" ).arg(i.key()).arg(j.key()).arg(j.value().toString());
			settings->setValue(j.key(),j.value());
			++j;
		}
		settings->endGroup();
		++i;
	}
	
}
