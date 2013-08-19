#include "torrentracker.h"

TorrentTracker::TorrentTracker(QObject *parent) :
    QObject(parent)
{
    requestHandler = new TrackerRequestMapper(this);
   
}

TorrentTracker::~TorrentTracker()
{
    stop();
}

void TorrentTracker::start()
{
    if (httpServer!=NULL)
    {
        stop();
    }
	httpServer = new HttpListener("TorrentTracker",requestHandler);
    httpServer->Start();
}

void TorrentTracker::stop()
{
    if (httpServer!=NULL)
    {
        httpServer->close();
        delete httpServer;
    }

}
