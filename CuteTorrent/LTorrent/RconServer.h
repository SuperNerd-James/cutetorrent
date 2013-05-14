#ifndef _RCON_SERVER_H
#define _RCON_SERVER_H

#include <QObject>
#include <QTcpServer>
#include "defs.h"
class RconServer : 
	private QTcpServer
{
	Q_OBJECT
private:
	bool disabled,useIPFilters;
	QList<QHostAddress> allowedIPs,notAllowedIPs;
	RconServer(int port,QObject* parrent);
	~RconServer(void);
	static RconServer* _instance;
	static int _instanceCount;
public:
	static int ListenPort;
	static RconServer* getInstance();
	static void FreeInstance();
	void start();
	void stop();
	bool isRunning();
	void start_upnp();
	void stop_upnp();
	void incomingConnection(int socket);
private slots:
	 void readClient();
	 void discardClient();
};

#endif _RCON_SERVER_H