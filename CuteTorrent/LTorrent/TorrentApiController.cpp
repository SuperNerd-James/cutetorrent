#include "TorrentApiController.h"

#include "QTorrentDisplayModel.h"
#include <QUrl>
#include <QTextDocument>
#include "json/json.h"
TorrentApiController::TorrentApiController( QObject* parent/*=0*/ ): HttpRequestHandler(parent),tManager(TorrentManager::getInstance())
{
	
}

void TorrentApiController::service( HttpRequest& request, HttpResponse& response )
{
	/*if (!CheckCreditinals(request,response))
	{
		return;
	}*/
	QMultiMap<QByteArray,QByteArray> parametrs=request.getParameterMap();
	qDebug() << parametrs;
	if (request.getMethod().toUpper()=="GET")
	{
		QtJson::JsonArray respJson;

		QString page = request.getParameter("page");
		QString pageSize = request.getParameter("pageSize");
		if (page.length() >0)
		{
			bool ok;
			int iPage = page.toInt(&ok);
			if (ok)
			{
				int iPageSize = pageSize.toInt(&ok);
				if (ok)
				{
					QVector<Torrent*> torrents = tManager->GetQTorrents();
					for (int i=(iPage-1)*iPageSize;i<std::min(torrents.count(),iPage*iPageSize);i++)
					{
						QtJson::JsonObject torrent;
						torrent["id"] = torrents[i]->GetInfoHash();
						torrent["torrentType"]=torrents[i]->GetType();
						torrent["torrentName"]=torrents[i]->GetName();
						torrent["torrentStatus"]=QString::number(torrents[i]->GetStatus());
						torrent["hasError"]=torrents[i]->hasError();
						torrent["isPaused"]=torrents[i]->isPaused();
						torrent["errorMessage"]= torrents[i]->GetErrorMessage();
						torrent["progress"]=QString::number(torrents[i]->GetProgress()) ;
						torrent["escapedTime"]=torrents[i]->GetActiveTime() ;	
						torrent["remainingTime"]=torrents[i]->GetRemainingTime() ;	
						torrent["dlSpeed"]=torrents[i]->GetDwonloadSpeed() ;	
						torrent["ulSpeed"]=torrents[i]->GetUploadSpeed() ;	
						torrent["dlSize"]=torrents[i]->GetTotalDownloaded() ;	
						torrent["ulSize"]=torrents[i]->GetTotalUploaded() ;	
						torrent["describtion"]=torrents[i]->GetDiscribtion() ;	
						torrent["torrentSize"]=torrents[i]->GetTotalSize() ;	
						torrent["savePath"]= torrents[i]->GetSavePath();	
						torrent["seedStr"]=torrents[i]->GetSeedString() ;	
						torrent["peerStr"]=torrents[i]->GetPeerString() ;	
						torrent["torrentsCount"] = torrents.count();
						QtJson::JsonArray peers;
						std::vector<peer_info> peerInfos=torrents[i]->GetPeerInfo();
						for (int j=0;j<peerInfos.size();j++)
						{
							QtJson::JsonObject peer;
							peer["ip"]=QString::fromStdString(peerInfos[j].ip.address().to_string()) ;	
							peer["client"]=QString::fromUtf8(peerInfos[j].client.c_str()) ;	
							peer["percent"]=QString::number(peerInfos[j].progress_ppm/10000.f,'f',1) ;	
							peer["peerDlSpeed"]=StaticHelpers::toKbMbGb(peerInfos[j].down_speed)+"/s" ;	
							peer["peerUlSpeed"]=StaticHelpers::toKbMbGb(peerInfos[j].up_speed)+"/s" ;	
							peer["dlSize"]=StaticHelpers::toKbMbGb(peerInfos[j].total_download) ;
							peer["ulSize"]=StaticHelpers::toKbMbGb(peerInfos[j].total_upload) +"\"";
							peers.append(peer);
						}
						torrent["peers"]=peers;


						QtJson::JsonArray trackers;
						std::vector<announce_entry> trackerInfos=torrents[i]->GetTrackerInfo();
						for (int j=0;j<trackerInfos.size();j++)
						{
							QtJson::JsonObject tracker;
							tracker["url"]=QString::fromStdString(trackerInfos[j].url) ;	
							tracker["state"]= (trackerInfos[j].message.length() >0 ? QString::fromUtf8(trackerInfos[j].message.c_str()) : "Ok" ) +"\"";
							trackers.append(tracker);
						}
						torrent["trackers"]=trackers;


						QtJson::JsonArray files;
						QList<file_info> fileinfo=torrents[i]->GetFileDownloadInfo();
						for (int j=0;j<fileinfo.count();j++)
						{
							QtJson::JsonObject file;
							file_info current=fileinfo.at(j);

							file["path"]= current.name;	
							file["size"]=StaticHelpers::toKbMbGb(current.size) ;	
							file["percent"]=QString::number(current.progrss,'f',0)+" %";	
							file["priority"]= StaticHelpers::filePriorityToString(current.prioiry) +"\"";
							files.append(file);
						}
						torrent["files"]=files;
						respJson.append(torrent);
					}
					response.setHeader("Content-Type","application/json");
					response.write(QtJson::serialize(respJson));
				}
				else
				{
					response.setStatus(400,"Bad Request");
					response.write("<BODY><h3>400 Bad Request.</h3></BODY>");
				}
			}
			else
			{
				response.setStatus(400,"Bad Request");
				response.write("<h3>400 Bad Request.</h3>");
			}
		}
		else
		{
			response.setStatus(405,"Method Not Allowed");
			response.write("<BODY><h3>405 Method Not Allowed.</h3>");
			response.write("</BODY>");
		}



		
	}
}


