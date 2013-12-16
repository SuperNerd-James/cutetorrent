/*
CuteTorrent BitTorrent Client with dht support, user friendly interface
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
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#include "application.h"
#include "messagebox.h"
#include <QTextCodec>
#include "CuteTorentStyle.h"
#include  "StyleEngene.h"
#ifdef DEBUG

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

#endif // DEBUG



int main(int argc, char *argv[])
{
	

    Application a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/app.ico"));

    QTextCodec *wantUnicode = QTextCodec::codecForName("UTF-8");
	/*QTextCodec::setCodecForTr(wantUnicode);
	QTextCodec::setCodecForLocale(wantUnicode);*/
    QTextCodec::setCodecForLocale(wantUnicode);
    bool minimize=false,nodebug=false;
	QString file2open;


    qsrand(time(NULL));
	if (a.isRunning())
	{
		if (argc>=2)
		for (int i=1;i<argc;i++)
		{
			if (argv[i][0]!='-')
			{
				a.sendMessage(QString::fromLocal8Bit(argv[i]));
				
			}
			
		}
		return -1;
	}
	else
	{
		if (argc>=2)
		{
			for (int i=1;i<argc;i++)
			{
				if (argv[i][0]=='-')
				{
                    if (!strcmp(argv[i],"-m")) {
                        minimize = true;
                    } else
                        if (!strcmp(argv[i],"-nodebug")) {
                           nodebug = true;

                    }
					

				}
				else
				{
					file2open=QString::fromLocal8Bit(argv[i]);
				}
			
			}
		}
	}
    FILE * fp;
    if (!nodebug) {

       fp = freopen("ct_debug.log","a+",stderr);
       qInstallMsgHandler(myMessageOutput);
    }
	a.loadTranslations(":/translations");
    a.loadTranslationsQt(":/translations_qt");
	a.addLibraryPath(QCoreApplication::applicationDirPath ()+"/plugins");
	
	
	CuteTorrent w;
	
	a.setActivationWindow(&w);
    a.setActiveWindow(&w);
	w.ConnectMessageReceved(&a);
	
	if (minimize)
	{
		w.showMinimized();
	}
	else
	{
		w.showNormal();
	}
	if (!file2open.isEmpty())
	{
		w.HandleNewTorrent(file2open);
	}
    a.setActiveWindow(&w);
	int res=a.exec();
	
    if (!nodebug && !fp) {
        fclose(fp);
    }

	return res;
}
