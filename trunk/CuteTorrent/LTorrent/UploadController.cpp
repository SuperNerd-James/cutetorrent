#include "UploadController.h"
#include <QDir>
#include <QApplication>
UploadController::UploadController( QObject* parent ) : HttpRequestHandler(parent),tManager(TorrentManager::getInstance())
{
	
}

void UploadController::service( HttpRequest& request, HttpResponse& response )
{

	if (!CheckCreditinals(request,response))
	{
		return;
	}
	if (request.getMethod()=="POST")
	{
		std::map<QByteArray,QByteArray> parametrs=request.getParameterMap().toStdMap();
		qDebug() << request.getParameterMap();
		QString save_path = request.getParameter("savePath");
		QTemporaryFile* tfile=request.getUploadedFile("files[]");


		if (save_path.isEmpty() ||  !QDir(save_path).exists())
		{
			response.setStatus(400,"Bad Request");
			response.write("<BODY><h3>400 Bad Request.</h3>");
			response.write("<h3>Invalid save-path</h3></BODY>");
			qDebug() << "Invalid save-path: "+save_path;
			return;
		}
		QFile uploaded(QApplication::applicationDirPath()+"/uploaded/"+request.getParameter("files[]"));
		if (uploaded.open(QFile::WriteOnly))
		{
			uploaded.write(tfile->readAll());
			uploaded.close();
		}
		else
		{
			response.setStatus(500," Internal Server Error");
			response.write("<BODY><h3>500  Internal Server Error.</h3>");
			response.write("<h3>Unable to open output file</h3></BODY>");
			qDebug() << "Unable to open output file";
			return;
		}
		error_code ec;
		tManager->AddTorrent(QFileInfo(uploaded).absoluteFilePath(),save_path,QMap<QString,int>(),ec);
		if (ec)
		{
			response.setStatus(500," Internal Server Error");
			response.write("<BODY><h3>500  Internal Server Error.</h3>");
			response.write(QString("<h3>"+QString::fromStdString(ec.message())+"</h3></BODY>").toUtf8());
			qDebug() << "Unable to open output file";
			return;
		}
	}
	else
	{
		response.setStatus(405,"Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
	
}

UploadController::~UploadController()
{
	TorrentManager::freeInstance();
}
