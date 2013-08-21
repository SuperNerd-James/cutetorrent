#ifndef TORRENTRACKER_H
#define TORRENTRACKER_H

#include <QObject>
#include "http/httplistener.h"
#include "trackerrequestmapper.h"
class TorrentTracker : public QObject
{
    Q_OBJECT
private:
    HttpListener* httpServer;
    TrackerRequestMapper* requestHandler;
public:
    explicit TorrentTracker(QObject *parent = 0);
    ~TorrentTracker();
    void start();
    void stop();
signals:
    
public slots:
    
};

#endif // TORRENTRACKER_H