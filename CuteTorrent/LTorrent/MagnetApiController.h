#pragma once
#include "http\HttpRequestHandler.h"
#include "TorrentManager.h"
#include "defs.h"
class MagnetApiController :
	public HttpRequestHandler
{
	Q_OBJECT
private:
	QMap<QString,QString> savePathMap;
	TorrentManager* tManager;
public:
	MagnetApiController(QObject*);

	void service( HttpRequest& request, HttpResponse& response );
	~MagnetApiController(void);
public slots:
	void DownloadMetadataCompleted(openmagnet_info info);
};
