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
#define DEBUG
#include "CuteTorrent.h"
#include <QDir>
#include <qtsingleapplication.h>
#include <cstdio>
#include <QDebug>
#include <QDateTime>
#include "application.h"
#include <QMessageBox>
#include <QMacStyle>
#include "CustomStyle.h"
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
#ifdef DEBUG

	FILE * fp=freopen("ct_debug.log","a+",stderr);
	qInstallMsgHandler(myMessageOutput);
#endif // DEBUG

	Application a(argc, argv);
	if (a.isRunning())
	{
		if (argc==2)
			a.sendMessage(argv[1]);
		return -1;
	}
	a.loadTranslations(":/translations");
	a.addLibraryPath(QCoreApplication::applicationDirPath ()+"/plugins");
	
	qDebug() << "=======================================================================================";
	qDebug() << "==Application started:" << QDateTime::currentDateTime();
	CuteTorrent w;
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
#ifdef DEBUG
	fclose(fp);
#endif // DEBUG
	return res;
}
