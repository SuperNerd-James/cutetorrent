
/**
  @file
  @author Stefan Frings
*/

#include "httprequesthandler.h"
#include <QCryptographicHash>
#include <QStringList>
#include "QApplicationSettings.h"
HttpRequestHandler::HttpRequestHandler(QString name,QObject* parent)
    : QObject(parent)
{
    serverName=name;
}

HttpRequestHandler::~HttpRequestHandler() {}

void HttpRequestHandler::initSettings()
{
	//qDebug() << "QApplicationSettings::getInstance from HttpRequestHandler::initSettings";
	QApplicationSettings* settings=QApplicationSettings::getInstance();
    requireAuth = settings->valueBool(serverName,"requireAuth",false);
	
    account.username = settings->valueString(serverName,"webui_login","admin");
    account.password = settings->valueString(serverName,"webui_password","admin");
	
	//qDebug() << account.username << account.password;
	//qDebug() << "QApplicationSettings::FreeInstance from HttpRequestHandler::initSettings";
	QApplicationSettings::FreeInstance();
}
void HttpRequestHandler::service(HttpRequest& request, HttpResponse& response) {
    qCritical("HttpRequestHandler: you need to override the dispatch() function");
    //qDebug("HttpRequestHandler: request=%s %s %s",request.getMethod().data(),request.getPath().data(),request.getVersion().data());
    response.setStatus(501,"not implemented");
    response.write("501 not implemented",true);
}
bool HttpRequestHandler::CheckCreditinals(HttpRequest& request,HttpResponse& response)
{
	initSettings();
	if (requireAuth)
	{
	
		QString autorization = request.getHeader("Authorization");
		if (autorization.isEmpty())
		{
			response.setStatus(401,"Not authoriarised");
			response.setHeader("WWW-Authenticate","Digest realm=\"realm@host.com\",qop=\"auth,auth-int\",nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"");
			return false;
		}
		QString method=autorization.split(' ')[0];
		QString parametrs = autorization.remove(0,method.length());
		QMap<QString,QString>* parametrsMap = new QMap<QString,QString>();;
		QStringList paremaetrsParts = parametrs.split(',');
		//qDebug() << paremaetrsParts;
		for (int i=0;i<paremaetrsParts.count();i++)
		{
			QStringList keyValue = paremaetrsParts[i].split('=');
			//qDebug() << keyValue;
			if (keyValue.count() ==2)
			{
				parametrsMap->insert(keyValue[0].trimmed(),keyValue[1].trimmed().replace("\"",""));
			}
			else
			{
				QString key = keyValue[0];
				QString value;
				for (int i=1;i<keyValue.size();i++)
				{
					value+=keyValue[i];
					value+=(i==keyValue.size()-1? "" : "=");
				}
				value=value.replace("\"","");
				parametrsMap->insert(key,value);

				//qDebug() << key << value;
			}
		}
		if (parametrsMap->value("username")!=account.username)
		{
			//qDebug() << parametrsMap->value("username") << account.username;
			//qDebug() << "Username not match" ;
			response.setStatus(401,"Unauthorized");
			response.setHeader("WWW-Authenticate","Digest realm=\"realm@host.com\",qop=\"auth,auth-int\",nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"");
			response.write("<BODY><H1>401 Unauthorized.</H1></BODY>");
			return false;
		}
		QByteArray HA1=QCryptographicHash::hash((parametrsMap->value("username")+":"+parametrsMap->value("realm")+":"+account.password).toUtf8(),QCryptographicHash::Md5);
		QByteArray HA2=QCryptographicHash::hash(request.getMethod()+":"+parametrsMap->value("uri").toUtf8(),QCryptographicHash::Md5);
		QByteArray Response = QCryptographicHash::hash( (HA1.toHex() + ":" + \
			parametrsMap->value("nonce") + ":" + \
			parametrsMap->value("nc") + ":" + \
			parametrsMap->value("cnonce") + ":" + \
			parametrsMap->value("qop") + ":" + \
			HA2.toHex()).toUtf8(),QCryptographicHash::Md5);
		if (Response.toHex()!=parametrsMap->value("response"))
		{
			//qDebug() << Response.toHex() << parametrsMap->value("response");
			//qDebug() << "Password not match";
			response.setStatus(401,"Unauthorized");
			response.setHeader("WWW-Authenticate","Digest realm=\"realm@host.com\",qop=\"auth,auth-int\",nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"");
			response.write("<BODY><H1>401 Unauthorized.</H1></BODY>");
			return false;
		}

		//qDebug() << Response.toHex();												
		//qDebug() << *parametrsMap;
		delete parametrsMap;
	}
	return true;
}

