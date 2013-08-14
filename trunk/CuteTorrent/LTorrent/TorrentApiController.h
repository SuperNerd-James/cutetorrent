#pragma once
#include "TorrentManager.h"
#include "http/httprequesthandler.h"
class TorrentApiController: public HttpRequestHandler 
{
	Q_OBJECT
	Q_DISABLE_COPY(TorrentApiController)
private:
	TorrentManager * tManager;
public:

	TorrentApiController(QObject* parent=0);

	void service(HttpRequest& request, HttpResponse& response);
	
};
