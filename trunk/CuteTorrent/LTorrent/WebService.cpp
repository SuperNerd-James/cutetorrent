#include "WebService.h"
#include <QApplication>
WebService::WebService(void)
{
	
	mapper = new RequestMapper(this);
	
}

WebService::~WebService(void)
{
	listener->close();
	delete mapper;
}

void WebService::Start()
{
	QSettings* settings = new QSettings( QApplication::applicationDirPath()+"/CuteTorrent.ini", QSettings::IniFormat);
	settings->beginGroup("listener");
	settings->setValue("port",8080);
	qDebug() << settings->value("port");
	settings->setValue("minThreads",1);
	settings->setValue("maxThreads",10);
	settings->setValue("cleanupInterval",1000);
	settings->setValue("readTimeout",60000);
	settings->setValue("maxRequestSize",16000);
	settings->setValue("maxMultiPartSize",1000000);
	
	listener = new HttpListener(settings,mapper,this);
	settings->endGroup();
}
