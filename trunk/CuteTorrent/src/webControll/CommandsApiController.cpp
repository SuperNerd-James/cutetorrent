#include "CommandsApiController.h"
#include "QApplicationSettings.h"
CommandsApiController::CommandsApiController(QObject* parent/*=0*/) : HttpRequestHandler("WebControl", parent), tManager(TorrentManager::getInstance())
{}

void CommandsApiController::service(HttpRequest& request, HttpResponse& response)
{
	
	if(requireAuth)
	{
		
		if(!CheckCreditinals(request, response))
		{
			return;
		}
	}

	if(request.getMethod().toUpper() == "POST")
	{
		QString action = request.getParameter("action");
		QString id = request.getParameter("id");
		Torrent* tor = tManager->GetTorrentByInfoHash(id);

		if(tor != NULL)
		{
			
			if(action == "pause")
			{
				tor->pause();
			}
			else if(action == "resume")
			{
				tor->resume();
			}
			else if(action == "remove")
			{
				tor->RemoveTorrent(tManager);
			}
			else
			{
				
			}
		}
		else
		{
			
		}
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}

CommandsApiController::~CommandsApiController()
{
	TorrentManager::freeInstance();
}
