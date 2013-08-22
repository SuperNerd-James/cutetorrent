#include "torrentracker.h"

TorrentTracker::TorrentTracker(QObject *parent) :
    QObject(parent)
{
    httpServer = new HttpListener("TorrentTracker",requestHandler);
    requestHandler = new TrackerRequestHandler(this);
   
}

TorrentTracker::~TorrentTracker()
{
    stop();
}

bool TorrentTracker::isRunning()
{
    return httpServer->isListening();
}

void TorrentTracker::start()
{
    if (httpServer!=NULL)
    {
        stop();
    }

    httpServer->Start();
}

void TorrentTracker::stop()
{
    if (httpServer!=NULL)
    {
		if (httpServer->isListening())
			httpServer->close();

    }

}
