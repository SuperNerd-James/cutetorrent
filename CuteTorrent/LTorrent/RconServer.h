#ifndef _RCON_SERVER_H
#define _RCON_SERVER_H

#include <QObject>
#include <QTcpServer>
#include "QApplicationSettings.h"
#include "TorrentManager.h"
#include "QTorrentDisplayModel.h"
#include "defs.h"
class RconServer : 
	private QTcpServer
{
	Q_OBJECT
private:
	QTorrentDisplayModel* model;
	TorrentManager* tManager;
	QApplicationSettings* settings;
	bool disabled,useIPFilters;
	QSet<QHostAddress> allowedIPs,notAllowedIPs;
	QVector<int> activeSessions;
	RconServer(int port,QTorrentDisplayModel* model,QObject* parrent);
	~RconServer(void);
	static RconServer* _instance;
	static int _instanceCount;
	void parseIpfiltrString(QString ipFilterStr);
	void handleSetSettingsVal( QByteArray data );
	void handleSetGlobalUL( QByteArray data );
	void handleSetGlobalDL( QByteArray data );
	void handleSetUL( QByteArray data );
	void handleSetDL( QByteArray data );
	void handleRemoveTorrentAll( QByteArray data );
	void handleRemoveTorrent( QByteArray data );
	void handlePauseTorrent( QByteArray data );
	void handleLogout( QByteArray data );
	void handleLogin( QTcpSocket* socket,QByteArray data );
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