#include "RequestMapper.h"
#include <QDir>
#include <QApplication>
RequestMapper::RequestMapper(QObject* parent)
	: HttpRequestHandler("WebControl", parent)
{
	staticFileController = new StaticFileController(this);
	torrentController = new TorrentApiController(this);
	uploadController = new UploadController(this);
	commandsController = new CommandsApiController(this);
	settingsController = new SettingsAPiController(this);
	magnetController = new MagnetApiController(this);
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
	QByteArray path = request.getPath();

	if(path.startsWith("/magnet/api"))
	{
		magnetController->service(request, response);
	}
	else if(path.startsWith("/settings/api"))
	{
		settingsController->service(request, response);
	}
	else if(path.startsWith("/torrent/api"))
	{
		torrentController->service(request, response);
	}
	else if(path.startsWith("/torrent/upload"))
	{
		uploadController->service(request, response);
	}
	else if(path.startsWith("/torrent/action"))
	{
		commandsController->service(request, response);
	}
	else
	{
		staticFileController->service(request, response);
	}
}

RequestMapper::~RequestMapper()
{
	delete staticFileController;
	delete torrentController ;
	delete uploadController ;
	delete commandsController ;
	delete settingsController;
	delete magnetController;
}
