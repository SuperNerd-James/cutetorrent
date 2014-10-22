#pragma once
#include "http/httprequesthandler.h"
#include "TorrentManager.h"
class CommandsApiController: public HttpRequestHandler 
{
	Q_OBJECT
	Q_DISABLE_COPY(CommandsApiController)
private:
	TorrentManager* tManager;
public:

	CommandsApiController(QObject* parent=0);

	void service(HttpRequest& request, HttpResponse& response);
	~CommandsApiController();
};

