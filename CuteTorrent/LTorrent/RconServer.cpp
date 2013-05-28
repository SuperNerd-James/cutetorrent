#include "RconServer.h"
#include "QApplicationSettings.h"
#include <QTcpSocket>
#include <QDebug>
#include <QCryptographicHash>
RconServer::RconServer(int port,QTorrentDisplayModel* m_model=0,QObject* parrent=0) : QTcpServer(parrent), settings(QApplicationSettings::getInstance()) , tManager(TorrentManager::getInstance())
{
	model=m_model;
	ListenPort=port;
	QApplicationSettings* settings = QApplicationSettings::getInstance();
	disabled = ! settings->valueBool("WebUI","webui_enabled",false);
	useIPFilters = settings->valueBool("WebUI","webui_use_ipfilters",false);
	QString ipFiletrString = settings->valueString("WebUI","webui_ipfilter","allow all");
	if (useIPFilters)
		parseIpfiltrString(ipFiletrString);
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
	////qDebug() << "QApplicationSettings freeing " <<_instanceCount<< " instance " ;
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
	QHostAddress peerAdr = s->peerAddress();
	if (notAllowedIPs.contains(peerAdr))
	{
		s->close();
	}
	if (allowedIPs.count() > 0 && !allowedIPs.contains(peerAdr))
	{
		s->close();
	}
}

void RconServer::readClient()
{
	if (disabled)
		return;

	QTcpSocket* socket = (QTcpSocket*)sender();
	QByteArray data=socket->readAll();
	//qDebug() << data;
	 // parse data
	RconMessageType type = (RconMessageType)data.toUInt();
	data.remove(0,sizeof(uint));
	// execute command
	switch (type)
	{
		case LOGIN:
		{
			handleLogin(socket,data);
			break;
		}
		case LOGOUT:
		{
			handleLogout(data);
			break;
		}
		case PAUSE_TORRENT:
		{
			handlePauseTorrent(data);
			break;
		}
		case REMOVE_TORRENT_ONLY:
		{
			handleRemoveTorrent(data);
			break;
		}
		case REMOVE_TORRENT_WITH_DATA:
		{
			handleRemoveTorrentAll(data);
			break;
		}
		case SET_DOWNLOAD_LIMIT:
		{
			handleSetDL(data);
			break;
		}
		case SET_UPLOAD_LIMIT:
		{
			handleSetUL(data);
			break;
		}
		case SET_GLOBAL_DOWNLOAD_LIMIT:
		{
			handleSetGlobalDL(data);
			break;
		}
	
		case SET_GLOBAL_UPLOAD_LIMIT:
		{
			handleSetGlobalUL(data);
			break;
		}
		case SET_SETTINGS_VALUE:
		{
			handleSetSettingsVal(data);
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

void RconServer::handleSetSettingsVal( QByteArray data )
{

}

void RconServer::handleSetGlobalUL( QByteArray data )
{
	tManager->SetUlLimit(data.toInt());
}

void RconServer::handleSetGlobalDL( QByteArray data )
{
	tManager->SetDlLimit(data.toInt());
}

void RconServer::handleSetUL( QByteArray data )
{
	QByteArray bSha1;
	for (int i=0;i<20;i++)
	{
		bSha1.append(data[i]);
	}
	data.remove(0,20);
	int limit = data.toInt();
	sha1_hash sha1 = sha1_hash(bSha1.data()); 
	Torrent* torrent = tManager->GetTorrentByInfoHash(sha1);
	if (torrent!=NULL)
	{
		torrent->SetUlLimit(limit);
		delete torrent;
	}
}

void RconServer::handleSetDL( QByteArray data )
{
	QByteArray bSha1;
	for (int i=0;i<20;i++)
	{
		bSha1.append(data[i]);
	}
	data.remove(0,20);
	int limit = data.toInt();
	sha1_hash sha1 = sha1_hash(bSha1.data()); 
	Torrent* torrent = tManager->GetTorrentByInfoHash(sha1);
	if (torrent!=NULL)
	{
		torrent->SetDlLimit(limit);
		delete torrent;
	}
}

void RconServer::handleRemoveTorrentAll( QByteArray data )
{
	QByteArray bSha1;
	for (int i=0;i<20;i++)
	{
		bSha1.append(data[i]);
	}
	sha1_hash sha1 = sha1_hash(bSha1.data()); 
	int index = model->hasTorrent(QString::fromStdString(sha1.to_string()));
	if (index >= 0)
		model->removeRow(index,true);
}

void RconServer::handleRemoveTorrent( QByteArray data )
{
	QByteArray bSha1;
	for (int i=0;i<20;i++)
	{
		bSha1.append(data[i]);
	}
	sha1_hash sha1 = sha1_hash(bSha1.data()); 
	int index = model->hasTorrent(QString::fromStdString(sha1.to_string()));
	if (index >= 0)
		model->removeRow(index,false);
}

void RconServer::handlePauseTorrent( QByteArray data )
{
	QByteArray bSha1;
	for (int i=0;i<20;i++)
	{
		bSha1.append(data[i]);
	}
	sha1_hash sha1 = sha1_hash(bSha1.data()); 
	Torrent* torrent = tManager->GetTorrentByInfoHash(sha1);
	if (torrent!=NULL)
	{
		torrent->pause();
		delete torrent;
	}
	
}

void RconServer::handleLogout( QByteArray data )
{
	int ID=data.toInt();
	if (activeSessions.contains(ID))
	{
		activeSessions.remove(ID);
	}
}

void RconServer::handleLogin(QTcpSocket* socket, QByteArray data )
{
	QString login = QString(data);
	QByteArray passHash;
	passHash.resize(data.size() - login.length());
	for (int i=login.length();i<data.size();i++)
	{
		passHash.append(data[i]);
	}
	QString settingsPasswordStr = settings->valueString("WebControl","webui_password");
	QByteArray settingsPassHash=QCryptographicHash::hash(settingsPasswordStr.toUtf8(),QCryptographicHash::Md5);
	if (passHash==settingsPassHash)
	{
		int newID=qrand();
		activeSessions.append(newID);
		socket->write(reinterpret_cast< const char* >( &newID ), sizeof( newID ) );
	}
}

void RconServer::parseIpfiltrString(QString ipFilterStr)
{
	QStringList lines = ipFilterStr.split(QRegExp("[ \r\n][ \r\n]*"));
	foreach(QString line,lines)
	{
		if (line.trimmed().startsWith("#"))
		{
			continue;
		}
		QStringList parts = line.trimmed().split(' ');
		if(parts[0]=="allow" || parts[0]=="deny")
		{
			if(parts[0]=="allow")
			{
				
				if (parts[1].contains('*') && !parts[1].contains('-'))
				{
					QString startIP = parts[1].replace("*","1").trimmed();
					QString endIP = parts[1].replace("*","255").trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					for (int i=start.toIPv4Address();i<=end.toIPv4Address();i++)
					{
						allowedIPs.insert(QHostAddress(i));
					}
				}
				else
				{
					//log message invalid string
				}
				if (parts[1].contains('-') && !parts[1].contains('*'))
				{
					QString startIP = parts[1].split('-')[0].trimmed();
					QString endIP = parts[1].split('-')[1].trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					for (int i=start.toIPv4Address();i<=end.toIPv4Address();i++)
					{
						allowedIPs.insert(QHostAddress(i));
					}
				}
				else
				{
					//log message invalid string
				}
			}
			else
			{
				if(parts[1]=="all")
					disabled=true;
				if (parts[1].contains('*') && !parts[1].contains('-'))
				{
					QString startIP = parts[1].replace("*","1").trimmed();
					QString endIP = parts[1].replace("*","255").trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					for (int i=start.toIPv4Address();i<=end.toIPv4Address();i++)
					{
						notAllowedIPs.insert(QHostAddress(i));
					}
				}
				else
				{
					//log message invalid string
				}
				if (parts[1].contains('-') && !parts[1].contains('*'))
				{
					QString startIP = parts[1].split('-')[0].trimmed();
					QString endIP = parts[1].split('-')[1].trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					for (int i=start.toIPv4Address();i<=end.toIPv4Address();i++)
					{
						notAllowedIPs.insert(QHostAddress(i));
					}
				}
				else
				{
					//log message invalid string
				}
			}
		}
		else
		{
			//log message invalid string
		}
	}
}



int RconServer::ListenPort=18366;

int RconServer::_instanceCount=0;

RconServer* RconServer::_instance=NULL;
