#include "CommandsApiController.h"
#include "QApplicationSettings.h"
CommandsApiController::CommandsApiController( QObject* parent/*=0*/ ) : HttpRequestHandler("WebControl",parent),tManager(TorrentManager::getInstance())
{
	
}

void CommandsApiController::service( HttpRequest& request, HttpResponse& response )
{
	//qDebug() << request.getParameterMap();
	if (requireAuth)
	{
		if (request.getHeader("Authorization").isEmpty())
		{
			//qDebug() << "header: " << request.getHeaderMap()<<  "parametrs: "  << request.getParameterMap();
			response.setStatus(401,"Not authoriarised");
			response.setHeader("WWW-Authenticate","Digest realm=\"realm@host.com\",qop=\"auth,auth-int\",nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"");
		}
		else
		{
			//qDebug() << "Authorization" << request.getHeader("Authorization");
			if (!CheckCreditinals(request,response))
			{
				return;
			}
		}
	}
	if (request.getMethod().toUpper()=="POST")
	{
		QString action = request.getParameter("action");
		QString id = request.getParameter("id");
		char* hashVal = new char[20];
		if (id.length()!=40)
		{
			response.setStatus(200,"Ok");
			response.write("Invalid id parametr");
			return;
		}
		memset(hashVal,0,20);
		for (int i=0;i<id.length();i+=2)
		{
			QString dig = QString(id[i])+id[i+1];
			bool ok;
			int digVal=dig.toInt(&ok,16);
			hashVal[i/2]=(char)(digVal & 0xFF );
		}
		libtorrent::sha1_hash hash(hashVal);
		Torrent* tor=tManager->GetTorrentByInfoHash(hash);
		if (tor!=NULL)
		{
			//qDebug() << "Found torrent: " << tor->GetName();
			if (action=="pause")
			{
				tor->pause();
			}
			else if (action=="resume")
			{
				tor->resume();
			}
			else if (action=="remove")
			{
				tor->RemoveTorrent(tManager);
			}
			else
			{
				//qDebug() << "Unknown action: " << action;
			}

		}
		else
		{
			//qDebug() << "Not found torrent";
		}
	}
	else
	{
		response.setStatus(405,"Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}

CommandsApiController::~CommandsApiController()
{
	TorrentManager::freeInstance();
}
