#include "RconServer.h"
#include "QApplicationSettings.h"
#include <QTcpSocket>
#include <QDebug>
RconServer::RconServer(int port,QObject* parrent=0) : QTcpServer(parrent)
{
	ListenPort=port;
	QApplicationSettings* settings = QApplicationSettings::getInstance();
	disabled = ! settings->valueBool("WebUI","webui_enabled",false);
	useIPFilters = settings->valueBool("WebUI","webui_use_ipfilters",false);
	QString ipFiletrString = settings->valueString("WebUI","webui_ipfilter","allow all");
	QApplicationSettings::FreeInstance();
	if (!disabled)
		listen(QHostAddress::Any, port);
}

RconServer::~RconServer(void)
{
	close();
}

RconServer* RconServer::getInstance()
{
	if (_instance==NULL)
	{
		_instance=new RconServer(ListenPort);
	}
	_instanceCount++;
	return _instance;
}

void RconServer::FreeInstance()
{
	_instanceCount--;
	//qDebug() << "QApplicationSettings freeing " <<_instanceCount<< " instance " ;
	if (!_instanceCount)
	{
		_instance->~RconServer();
		_instance=NULL;
	}
}

void RconServer::incomingConnection( int socket )
{
	if (disabled)
		return;

	QTcpSocket* s = new QTcpSocket(this);
	connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
	s->setSocketDescriptor(socket);
}

void RconServer::readClient()
{
	if (disabled)
		return;

	QTcpSocket* socket = (QTcpSocket*)sender();
	QByteArray data=socket->readAll();
	qDebug() << data;
	 // parse data
	RconMessageType type = (RconMessageType)data.toUInt();
	data.remove(0,sizeof(uint));
	// execute command
	switch (type)
	{
		case LOGIN:
		{
			break;
		}
		case LOGOUT:
		{
			break;
		}
		case PAUSE_TORRENT:
		{
			break;
		}
		case REMOVE_TORRENT_ONLY:
		{
			break;
		}
		case REMOVE_TORRENT_WITH_DATA:
		{
			break;
		}
		case SET_DOWNLOAD_LIMIT:
		{
			break;
		}
		case SET_UPLOAD_LIMIT:
		{
			break;
		}
		case SET_GLOBAL_DOWNLOAD_LIMIT:
		{
			break;
		}
	
		case SET_GLOBAL_UPLOAD_LIMIT:
		{
			break;
		}
		case SET_SETTINGS_VALUE:
		{
			break;
		}
		
	}
	 
}

void RconServer::discardClient()
{
	QTcpSocket* socket = (QTcpSocket*)sender();
	socket->deleteLater();
}

void RconServer::start_upnp()
{

}

void RconServer::stop_upnp()
{

}

void RconServer::start()
{
	disabled=false;
	listen(QHostAddress::Any, ListenPort);
}

void RconServer::stop()
{
	if(!disabled)
		close();
	disabled=true;
}

bool RconServer::isRunning()
{
	return isListening();
}



int RconServer::ListenPort=18366;

int RconServer::_instanceCount=0;

RconServer* RconServer::_instance=NULL;
