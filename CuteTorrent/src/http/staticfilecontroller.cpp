/**
  @file
  @author Stefan Frings
*/

#include "staticfilecontroller.h"
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QCryptographicHash>
#include <QApplication>
StaticFileController::StaticFileController(QObject* parent)
	: HttpRequestHandler("WebControl", parent)
{
	settings = QApplicationSettings::getInstance();
	maxAge = settings->valueInt("WebControl", "maxAge", 60000);
	encoding = settings->valueString("WebControl", "encoding", "UTF-8");
	docroot = settings->valueString("WebControl", "path", "./webControll/");

	if(QDir::isRelativePath(docroot))
	{
		QString dataDir = QApplication::applicationDirPath() + QDir::separator();
		docroot = QDir::cleanPath(dataDir + docroot);
	}

	maxCachedFileSize = settings->valueInt("WebControl", "maxCachedFileSize", 65536);
	cache.setMaxCost(settings->valueInt("WebControl", "cacheSize", 1000000));

	try
	{
		cacheTimeout = settings->valueInt("WebControl", "cacheTime", 60000);
	}
	catch(...)
	{
		qDebug() << "Exception in StaticFileController::StaticFileController";
	}
}


void StaticFileController::service(HttpRequest& request, HttpResponse& response)
{
	QByteArray path = request.getPath();

	// Forbid access to files outside the docroot directory
	if(path.startsWith("/.."))
	{
		qWarning("StaticFileController: somebody attempted to access a file outside the docroot directory");
		response.setStatus(403, "forbidden");
		response.write("403 forbidden", true);
	}

	if(!CheckCreditinals(request, response))
	{
		return;
	}

	// Check if we have the file in cache
	qint64 now = QDateTime::currentMSecsSinceEpoch();
	mutex.lock();
	CacheEntry* entry = cache.object(path);

	if(entry && (cacheTimeout == 0 || entry->created > now - cacheTimeout) && QFileInfo(docroot + path).lastModified().toMSecsSinceEpoch() <= entry->created)
	{
		QByteArray document = entry->document; //copy the cached document, because other threads may destroy the cached entry immediately after mutex unlock.
		mutex.unlock();
		setContentType(path, response);
		response.setHeader("Cache-Control", "max-age=" + QByteArray::number(maxAge / 1000));
		response.write(document);
	}
	else
	{
		mutex.unlock();

		// The file is not in cache.
		// If the filename is a directory, append index.html.
		if(QFileInfo(docroot + path).isDir())
		{
			path += "/index.html";
		}

		QFile file(docroot + path);

		if(file.exists())
		{
			if(file.open(QIODevice::ReadOnly))
			{
				setContentType(path, response);
				response.setHeader("Cache-Control", "max-age=" + QByteArray::number(maxAge / 1000));

				if(file.size() <= maxCachedFileSize)
				{
					// Return the file content and store it also in the cache
					entry = new CacheEntry();

					while(!file.atEnd() && !file.error())
					{
						QByteArray buffer = file.read(65536);
						response.write(buffer);
						entry->document.append(buffer);
					}

					entry->created = now;
					mutex.lock();

					if(!cache.contains(path))
					{
						cache.insert(request.getPath(), entry, entry->document.size());
					}
					else
					{
						cache.remove(path);
						cache.insert(request.getPath(), entry, entry->document.size());
					}

					mutex.unlock();
				}
				else
				{
					// Return the file content, do not store in cache
					while(!file.atEnd() && !file.error())
					{
						response.write(file.read(65536));
					}
				}

				file.close();
			}
			else
			{
				qWarning("StaticFileController: Cannot open existing file %s for reading", qPrintable(file.fileName()));
				response.setStatus(403, "forbidden");
				response.write("403 forbidden", true);
			}
		}
		else
		{
			response.setStatus(404, "not found");
			response.write("404 not found", true);
		}
	}
}

void StaticFileController::setContentType(QString fileName, HttpResponse& response) const
{
	if(fileName.endsWith(".png"))
	{
		response.setHeader("Content-Type", "image/png");
	}
	else if(fileName.endsWith(".jpg"))
	{
		response.setHeader("Content-Type", "image/jpeg");
	}
	else if(fileName.endsWith(".gif"))
	{
		response.setHeader("Content-Type", "image/gif");
	}
	else if(fileName.endsWith(".pdf"))
	{
		response.setHeader("Content-Type", "application/pdf");
	}
	else if(fileName.endsWith(".txt"))
	{
		response.setHeader("Content-Type", qPrintable("text/plain; charset=" + encoding));
	}
	else if(fileName.endsWith(".html") || fileName.endsWith(".htm"))
	{
		response.setHeader("Content-Type", qPrintable("text/html; charset=" + encoding));
	}
	else if(fileName.endsWith(".css"))
	{
		response.setHeader("Content-Type", "text/css");
	}
	else if(fileName.endsWith(".js"))
	{
		response.setHeader("Content-Type", "text/javascript");
	}
	else if(fileName.endsWith(".woff"))
	{
		response.setHeader("Content-Type", "application/font-woff");
	}
	else if(fileName.endsWith(".ico"))
	{
		response.setHeader("Content-Type", "image/x-icon");
	}

	// Todo: add all of your content types
}

StaticFileController::~StaticFileController()
{
	QApplicationSettings::FreeInstance();
}

/*
bool StaticFileController::CheckCreditinals( HttpRequest& request,HttpResponse& response )
{
	QString autorsation = request.getHeader("Authorization");
	QString method=autorsation.split(' ')[0];
	QString parametrs = autorsation.remove(0,method.length());
	QMap<QString,QString>* parametrsMap = new QMap<QString,QString>();;
	QStringList paremaetrsParts = parametrs.split(',');

	for (int i=0;i<paremaetrsParts.count();i++)
	{
		QStringList keyValue = paremaetrsParts[i].split('=');

		if (keyValue.count() ==2)
		{
			parametrsMap->insert(keyValue[0].trimmed(),keyValue[1].trimmed().replace("\"",""));
		}
		else
		{

		}
	}
	if (parametrsMap->value("username")!=account.username)
	{

		response.setStatus(401,"Unauthorized");
		response.setHeader("WWW-Authenticate","Digest realm=\"realm@host.com\",qop=\"auth,auth-int\",nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"");
		response.write("<BODY><H1>401 Unauthorized.</H1></BODY>");
		return false;
	}
	QByteArray HA1=QCryptographicHash::hash((parametrsMap->value("username")+":"+parametrsMap->value("realm")+":"+account.password).toUtf8(),QCryptographicHash::Md5);
	QByteArray HA2=QCryptographicHash::hash(request.getMethod()+":"+request.getPath(),QCryptographicHash::Md5);
	QByteArray Response = QCryptographicHash::hash( (HA1.toHex() + ":" + \
													parametrsMap->value("nonce") + ":" + \
													parametrsMap->value("nc") + ":" + \
													parametrsMap->value("cnonce") + ":" + \
													parametrsMap->value("qop") + ":" + \
													HA2.toHex()).toUtf8(),QCryptographicHash::Md5);
	if (Response.toHex()!=parametrsMap->value("response"))
	{

		response.setStatus(401,"Unauthorized");
		response.setHeader("WWW-Authenticate","Digest realm=\"realm@host.com\",qop=\"auth,auth-int\",nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"");
		response.write("<BODY><H1>401 Unauthorized.</H1></BODY>");
		return false;
	}



	delete parametrsMap;
	return true;

}*/
