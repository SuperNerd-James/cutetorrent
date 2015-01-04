#pragma once
#include "TorrentManager.h"
#include "http/httprequesthandler.h"
#include "TorrentStorrage.h"
class TorrentApiController: public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(TorrentApiController)
private:
	TorrentManager* tManager;
	TorrentStorrage* torrents;
public:

	TorrentApiController(QObject* parent = 0);
	~TorrentApiController();
	void service(HttpRequest& request, HttpResponse& response);

};
