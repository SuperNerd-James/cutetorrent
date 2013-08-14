#include "RequestMapper.h"
#include <QDir>
#include <QApplication>
RequestMapper::RequestMapper(QObject* parent)
:HttpRequestHandler(parent) {
	QSettings* settings = new QSettings( QApplication::applicationDirPath()+"/CuteTorrent.ini", QSettings::IniFormat);
	settings->beginGroup("listener");
	settings->setValue("path",QApplication::applicationDirPath()+"/webControll");
	settings->setValue("encoding","UTF-8");
	settings->setValue("maxAge",60000);
	settings->setValue("cacheTime",60000);
	settings->setValue("cacheSize",1000000);
	settings->setValue("maxCachedFileSize",65536);
	staticFileController = new StaticFileController(settings,this);
	torrentController = new TorrentApiController(this);
	uploadController = new UploadController(this);
	commandsController = new CommandsApiController(this);
	settingsController = new SettingsAPiController(this);
	magnetController = new MagnetApiController(this);
	settings->endGroup();
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response) {
	QByteArray path=request.getPath();
	qDebug("RequestMapper: path=%s",path.data());

	if (path.startsWith("/magnet/api"))
	{
		qDebug("RequestMapper: using magnetController");
		magnetController->service(request,response);
	}
	else
	if (path.startsWith("/settings/api"))
	{
		qDebug("RequestMapper: using settingsController");
		settingsController->service(request,response);
	}
	else
	if (path.startsWith("/torrent/api"))
	{
		qDebug("RequestMapper: using torrentController");
		torrentController->service(request,response);
	}
	else
		if (path.startsWith("/torrent/upload"))
		{
			qDebug("RequestMapper: using uploadController");
			uploadController->service(request,response);
		}
		else
			if (path.startsWith("/torrent/action"))
			{
				qDebug("RequestMapper: using commandsController");
				commandsController->service(request,response);
			}
			else
				staticFileController->service(request, response);
	
	qDebug("RequestMapper: finished request");
}

RequestMapper::~RequestMapper()
{
	delete staticFileController;
	delete torrentController ;
	delete uploadController ;
	delete commandsController ;
}
