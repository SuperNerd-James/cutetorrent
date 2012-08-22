/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "UpdateNotyfier.h"
#include <QDebug>
#include "versionInfo.h"
#include <QUrl>
UpdateNotifier::UpdateNotifier()
{
	m_manager = new QNetworkAccessManager(this);

	connect(m_manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(replyFinished(QNetworkReply*)));

}

void UpdateNotifier::fetch()
{
	m_manager->get(QNetworkRequest(QUrl("http://cutetorrent.googlecode.com/svn/trunk/CuteTorrent/LTorrent/version.txt")));
}

void UpdateNotifier::replyFinished(QNetworkReply* pReply)
{

	QByteArray data=pReply->readAll();
	QString str(data);
	qDebug() << str;
	if (str.compare(CT_VERSION)!=0)
		emit showUpdateNitify(str);
	else
		emit showNoUpdateNitify(str);
}

UpdateNotifier::~UpdateNotifier()
{
	delete m_manager;
}
