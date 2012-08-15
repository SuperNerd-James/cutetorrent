#include "ltorrent.h"
#include <QDir>
#include <qtsingleapplication.h>
#include <cstdio>
#include <QDebug>
#include <QDateTime>
void myMessageOutput(QtMsgType type, const char *msg)
{
	fflush(stderr);
	switch (type) {
	 case QtDebugMsg:
		 fprintf(stderr, "Debug: %s\n", msg);
		 break;
	 case QtWarningMsg:
		 fprintf(stderr, "Warning: %s\n", msg);
		 break;
	 case QtCriticalMsg:
		 fprintf(stderr, "Critical: %s\n", msg);
		 break;
	 case QtFatalMsg:
		 fprintf(stderr, "Fatal: %s\n", msg);
		 abort();
	}
}

int main(int argc, char *argv[])
{
	FILE * fp=freopen("ct_debug.log","a+",stderr);
	qInstallMsgHandler(myMessageOutput);
	QtSingleApplication a(argc, argv);
	if (a.isRunning())
	{
		if (argc==2)
			a.sendMessage(argv[1]);
		return -1;
	}
	a.addLibraryPath(QCoreApplication::applicationDirPath ()+"/plugins");
	qDebug() << "=======================================================================================";
	qDebug() << "==Application started:" << QDateTime::currentDateTime();
	LTorrent w;
	QFile File(":/icons/BaseStyle.qss");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QString::fromUtf8(File.readAll().data());
	File.close();
	a.setStyleSheet(StyleSheet);
	a.setActivationWindow(&w);
	w.ConnectMessageReceved(&a);
	w.show();
	if (argc==2)
	{
		w.HandleNewTorrent(argv[1]);
	}
	int res=a.exec();
	qDebug() << "==Application exited:" << QDateTime::currentDateTime();
	qDebug() << "=======================================================================================";
	fclose(fp);
	return res;
}
