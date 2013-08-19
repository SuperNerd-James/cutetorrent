#include "RconWebService.h"
#include <QApplication>
RconWebService::RconWebService(void)
{
	
	mapper = new RequestMapper(this);
    listener = new HttpListener("WebControl",mapper,this);
}

RconWebService::~RconWebService(void)
{
	if (listener!=NULL)
		listener->close();
	delete mapper;
}

void RconWebService::Start()
{
	
	if (listener!=NULL)
	{
		listener->close();
		listener->Start();
		
	}
	else
	{
        listener = new HttpListener("WebControl",mapper,this);
		listener->Start();
	}
		
	
	
}

void RconWebService::parseIpFilter( QString ipFilterStr )
{
	QStringList lines = ipFilterStr.split("\n");
	//qDebug() << lines;
	foreach(QString line,lines)
	{
		//qDebug()<< "parsing line:" << line;
		if (line.trimmed().startsWith("#"))
		{
			//qDebug()<< "line:" << line << "is comment so skiping it";
			continue;
		}
		QStringList parts = line.trimmed().split(' ');
		//qDebug() << "parts:" << parts;
		if(parts[0]=="allow" || parts[0]=="deny")
		{
			if(parts[0]=="allow")
			{

				if (parts[1].contains('*') && !parts[1].contains('-'))
				{

					QString pattern=parts[1];
					QString startIP = pattern.replace("*","1").trimmed();
					QString endIP = parts[1].replace("*","255").trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					allowedIP.append(QPair<uint,uint>(start.toIPv4Address(),end.toIPv4Address()));
				}
				else
				{
					//qDebug()<< "line:" << line << "not match parts[1].contains('*') && !parts[1].contains('-')";

				}
				if (parts[1].contains('-') && !parts[1].contains('*'))
				{

					QString pattern=parts[1];
					QString startIP = pattern.replace("*","1").trimmed();
					QString endIP = parts[1].replace("*","255").trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					allowedIP.append(QPair<uint,uint>(start.toIPv4Address(),end.toIPv4Address()));
				}
				else
				{
					//qDebug()<< "line:" << line << "not match parts[1].contains('-') && !parts[1].contains('*')";
				}
			}
			else
			{
				if(parts[1]=="all")
				{
					if (listener!=NULL)
					listener->close();
				}
				if (parts[1].contains('*') && !parts[1].contains('-'))
				{
					QString pattern=parts[1];
					QString startIP = pattern.replace("*","1").trimmed();
					QString endIP = parts[1].replace("*","255").trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					//qDebug() << "deny: start: " << start << "end: " <<end;
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					notAllowedIP.append(QPair<uint,uint>(start.toIPv4Address(),end.toIPv4Address()));
					
				}
				else
				{
					//qDebug()<< "line:" << line << "not match parts[1].contains('*') && !parts[1].contains('-')";
				}
				if (parts[1].contains('-') && !parts[1].contains('*'))
				{

					QString pattern=parts[1];
					QString startIP = pattern.replace("*","1").trimmed();
					QString endIP = parts[1].replace("*","255").trimmed();
					QHostAddress start(startIP);
					QHostAddress end(endIP);
					if(start.toIPv4Address() > end.toIPv4Address())
					{
						qSwap(start,end);
					}
					notAllowedIP.append(QPair<uint,uint>(start.toIPv4Address(),end.toIPv4Address()));
				}
				else
				{
				//qDebug()<< "line:" << line << "not match parts[1].contains('-') && !parts[1].contains('*')";
				}
			}
		}
		else
		{
			//qDebug()<< "line:" << line << "is invalid";
		}
	}
	HttpConnectionHandler::allowedIP=allowedIP;
	HttpConnectionHandler::notAllowedIP=notAllowedIP;
	//qDebug() << "notAllowedIP:" << notAllowedIP;
	//qDebug() << "allowedIP:"    << allowedIP;
}

bool RconWebService::isRunning()
{
	if (listener!=NULL)
	{
		return listener->isListening();
	}
	return false;
}

void RconWebService::Stop()
{
	if (listener!=NULL)
	{
		listener->close();
		delete listener;
		listener=NULL;
	}
}

RconWebService* RconWebService::getInstance()
{
	if (instnce==NULL)
	{
		instnce=new RconWebService();
	}
	intanceCount++;
	return instnce;
}

void RconWebService::freeInstance()
{
	intanceCount--;
	if (intanceCount==0)
	{
		delete instnce;
		instnce=NULL;
	}
	
}

int RconWebService::intanceCount=0;

RconWebService* RconWebService::instnce=NULL;
