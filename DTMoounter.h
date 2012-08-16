#include <QString>
#include <QObject>


class DTMounter : private QObject
{
	Q_OBJECT
private:
	QString dtPath;

public slots:
	void Mount(QString filepath,QString dtComandPrototype);

};


