#ifndef TRACKERREQUESTMAPPER_H
#define TRACKERREQUESTMAPPER_H

#include <QObject>
#include "http/httprequesthandler.h"
#include "AnnounceRequest.h"
class TrackerRequestMapper : public HttpRequestHandler
{
    Q_OBJECT
public:
    typedef QMap<QString,PeerInfo> PeerList;
    QMap<QString,PeerList> torrents;
    explicit TrackerRequestMapper(QObject *parent = 0);
     void service(HttpRequest& request, HttpResponse& response);
private:
     void ProcessAnnounceRequest(HttpRequest& request, HttpResponse& response);
     void ReplyWithPeerList(HttpResponse& response,AnnounceRequest announceRequest);

signals:
    
public slots:
    
};

#endif // TRACKERREQUESTMAPPER_H
