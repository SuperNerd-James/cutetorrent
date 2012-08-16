#include "GroupForFileFiltering.h"

#include <QStringList>
#include <QMessageBox>

GroupForFileFiltering::GroupForFileFiltering(QString _name,QString _extensions,QString _savepath)
{
	name=_name;
	path  = _savepath;
	QStringList extensionList = _extensions.split('|');
	if (extensionList.count()>0)
	{
		for (int i = 0; i < extensionList.count() ; i++)
		{
			
			if (!extensionList.at(i).isEmpty())
			{
			
				extensions.insert(extensionList.at(i));
			}
		}
		
	}
}

bool GroupForFileFiltering::Contains(QString extension) const
{
	return extensions.contains(extension);
}
QString GroupForFileFiltering::Name() const
{
	return name;
}
QString GroupForFileFiltering::Extensions() const
{
	QString res;
	for(QSet<QString>::ConstIterator i=extensions.begin();i!=extensions.end();i++)
	{
		res=res.append(*i);
		res=res.append("|");
	}
	
	return res;
}
QString GroupForFileFiltering::SavePath() const
{
	return path;
}