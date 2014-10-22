#include "trackerrequestmapper.h"

TrackerRequestHandler::TrackerRequestHandler(QObject *parent) :
    HttpRequestHandler("TorrentTracker",parent)
{

    failtureText.insert(100,"Invalid request type: client request was not a HTTP GET.");
    failtureText.insert(101,"Missing info_hash.");
    failtureText.insert(102,"Missing peer_id.");
    failtureText.insert(103,"Missing port.");
    failtureText.insert(150,"Invalid infohash: infohash is not 20 bytes long.");
    failtureText.insert(151,"Invalid peerid: peerid is not 20 bytes long.");
    failtureText.insert(152,"Invalid numwant. Client requested more peers than allowed by tracker.");
    failtureText.insert(200,	"info_hash not found in the database.");
    failtureText.insert(500,"Client sent an eventless request before the specified time.");
    failtureText.insert(900,"Generic error.");
}

void TrackerRequestHandler::service(HttpRequest &request, HttpResponse &response)
{
	//qDebug() << "ToorentTracker" << request.getParameterMap();
	//qDebug() << "ToorentTracker" << request.getHeaderMap();
    if (request.getMethod()!="GET")
    {
        response.setStatus(100,"Method Not Allowed");
        response.write("<body><h1>100 Method Not Allowed</h1></body>");
        return;
    }
    if (QString(request.getPath()).startsWith("/announce", Qt::CaseInsensitive))
    {
        ProcessAnnounceRequest(request,response);

        return;
    }
    response.setStatus(400,"Bad Request" );
    response.write("<body><h1>400 Bad Request </h1></body>");


}

void TrackerRequestHandler::ProcessAnnounceRequest(HttpRequest &request, HttpResponse &response)
{
    AnnounceRequest announceRequest;
    announceRequest.ip=request.getSource();
    announceRequest.info_hash=QString::fromStdString(to_hex(std::string(request.getParameter("info_hash").data())));
    //qDebug() << announceRequest.info_hash;
    if(announceRequest.info_hash.length()==0)
    {
        //qDebug() << "Missing info hash";
        ReplyError(101,response);
        return;
    }
    //qDebug() << "info_hash.length()="<<announceRequest.info_hash.length();
    if (announceRequest.info_hash.length()!=40)
    {
        ReplyError(150,response);
        return;
    }
    announceRequest.peer_id=request.getParameter("peer_id");
    if(announceRequest.peer_id.length()==0)
    {
        //qDebug() << "Missing peer id";
        ReplyError(102,response);
        return;
    }
    //qDebug() << "peer_id.length()="<<announceRequest.peer_id.length();
    if (announceRequest.peer_id.length()!=20)
    {
        ReplyError(151,response);
        return;
    }
    QString port=request.getParameter("port");
    if (port.length()==0)
    {
        //qDebug() << "Missing port";
        ReplyError(103,response);
        return;
    }
    bool ok;
    announceRequest.port = port.toInt(&ok);
    if (!ok || announceRequest.port < 1 || announceRequest.port > 65535)
    {
        //qDebug() << "Invalid port" << announceRequest.port << port;
        ReplyError(103,response);
        return;
    }
    announceRequest.numwant=50;
    int tmp=request.getParameter("numwant").toInt();
    if (tmp >0)
    {
        announceRequest.numwant=tmp;
    }
    announceRequest.event = request.getParameter("event");
    announceRequest.compact = (request.getParameter("compact").length()>0);
    announceRequest.no_peer_id = (request.getParameter("no_peer_id").length()>0);
    PeerInfo info=announceRequest.getPeerInfo();
    if (torrents.contains(announceRequest.info_hash))
    {
        if (announceRequest.event=="stopped")
        {
            qDebug("TrackerRequestMapper: Peer %s stopped downloading %s, deleting it from the list",qPrintable(announceRequest.peer_id),qPrintable(announceRequest.info_hash));
            torrents[announceRequest.info_hash].remove(info.getID());
            return;
        }
    }
    PeerList peers=torrents[announceRequest.info_hash];
    //qDebug() << info.ip;
    //qDebug() << info.peer_id;
    //qDebug() << info.port;
    peers[info.getID()]=info;
    qDebug("%s has %s peers",qPrintable(announceRequest.info_hash),qPrintable(QString::number(peers.count())));
    torrents[announceRequest.info_hash]=peers;

    ReplyWithPeerList(response,announceRequest);
}

void TrackerRequestHandler::ReplyWithPeerList(HttpResponse &response, AnnounceRequest& announceRequest)
{
    entry::dictionary_type reply_dict;
    reply_dict["interval"] = entry(1800);
    QList<PeerInfo> peers = torrents.value(announceRequest.info_hash).values();
    entry::list_type peer_list;
	int count = 0;
    foreach (PeerInfo p, peers)
    {
        peer_list.push_back(p.toEntry(announceRequest.no_peer_id,announceRequest.compact));
		count++;
		if (count == announceRequest.numwant)
		{
			break;
		}
    }
    reply_dict["peers"] = entry(peer_list);
    entry reply_entry(reply_dict);
    // bencode
    std::vector<char> buf;
    bencode(std::back_inserter(buf), reply_entry);
    QByteArray reply(&buf[0], buf.size());
    response.setStatus(200,"OK");
    response.write(reply);
}

void TrackerRequestHandler::ReplyError(int code,HttpResponse &response)
{
     entry::dictionary_type reply_dict;
     reply_dict["failure code"]=code;
     if (failtureText.contains(code))
     {
         reply_dict["failure reason"]=failtureText[code];
     }
     entry reply_entry(reply_dict);
     // bencode
     std::vector<char> buf;
     bencode(std::back_inserter(buf), reply_entry);
     QByteArray reply(&buf[0], buf.size());
     response.setStatus(200,"OK");
     response.write(reply);

}
