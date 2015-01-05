#include "RutorSearchProvider.h"

QString RutorSearchProvider::Name()
{
	return "Rutor";
}

QString RutorSearchProvider::Url()
{
	return "http://rutor.org/";
}

int RutorSearchProvider::SupportedCategories()
{
	return ISerachProvider::All;
}

void RutorSearchProvider::PeformSearch(QString token, SearchCategories category, int page)
{
	QList<QNetworkCookie>  cookies = m_pNetworAccessManager->cookieJar()->cookiesForUrl(QUrl("http://rutor.org/"));
	QVariant var;
	var.setValue(QString("_ddn_intercept_2_=a505e586720fbb88d8652a3e4911d88a; max-age=604800; path=/"));
	QNetworkRequest request;
	request.setUrl(BuildUrl(token, category, page));
	request.setHeader(QNetworkRequest::CookieHeader, var);
	m_pNetworAccessManager->get(request);
}

RutorSearchProvider::RutorSearchProvider()
{
	m_pNetworAccessManager = new QNetworkAccessManager(this);
	QObject::connect(m_pNetworAccessManager, SIGNAL(finished(QNetworkReply*)),
	                 this, SLOT(replyReady(QNetworkReply*)));
	m_categoryMap[Anime] = "10";
	m_categoryMap[Music] = "2";
	m_categoryMap[TV] = "6";
	m_categoryMap[Porn] = "1";
	m_categoryMap[Software] = "9";
	m_categoryMap[Games] = "8";
	m_categoryMap[Books] = "11";
	m_categoryMap[Movie] = "1";
}

RutorSearchProvider::~RutorSearchProvider()
{
	delete m_pNetworAccessManager;
}

QString RutorSearchProvider::BuildUrl(QString token, SearchCategories category, int page)
{
	if (category == All)
	{
		return QString("http://alt.rutor.org/search/%2/0/110/2/%1").arg(token, QString::number(page - 1));
	}
	else
	{
		return QString("http://alt.rutor.org/search/%3/%2/110/2/%1").arg(token, m_categoryMap[category], QString::number(page - 1));
	}
}

void RutorSearchProvider::replyReady(QNetworkReply* pReply)
{
	if (pReply->error() == QNetworkReply::NoError)
	{
		QList<SearchResult> results;
		QString data = pReply->readAll();
		QSgml sgml(data);
		QList<QSgmlTag*> tags;
		sgml.getElementsByAtribute("class", "backgr", &tags);

		if (tags.count() > 0)
		{
			if (tags.count() > 1)
			{
				qDebug() << "More than one search result table found.";
			}

			QSgmlTag* resaltsTable = tags.at(0)->Parent;
			QSgmlTag::QSgmlTaglist tableBody;
			int nChildCount = resaltsTable->Children.count();

			for (int i = 1; i < nChildCount; i++)
			{
				QSgmlTag* pChild = resaltsTable->Children.at(i);
				qDebug() << pChild->Name << pChild->Value << pChild->Attributes;

				if (pChild->Name.compare("tr", Qt::CaseInsensitive) == 0)
				{
					tableBody.append(pChild);
				}
			}

			if (!tableBody.empty())
			{
				ParseTableBody(tableBody, &sgml);
			}
		}
	}
}

void RutorSearchProvider::ParseTableBody(QSgmlTag::QSgmlTaglist tableBody, QSgml* sgml)
{
	QQueue<QSgmlTag*> traverseQueue;
	traverseQueue.append(tableBody);
	int tdCounter = 0;
	QList<SearchResult*> searchResults;
	SearchResult* pCurrentSearchResult = NULL;

	while (!traverseQueue.empty())
	{
		QSgmlTag* pCurrent = traverseQueue.dequeue();
		traverseQueue.append(pCurrent->Children);

		if (pCurrent->Name.compare("td", Qt::CaseInsensitive) == 0)
		{
			QString sizeHtmlStr;
			QString seedersCountStr;
			QString peersCountStr;

			switch (tdCounter)
			{
				case 1:
				{
					int nChildCount = pCurrent->Children.count();

					for (int i = 0; i < nChildCount; i++)
					{
						QSgmlTag* pChild = pCurrent->Children.at(i);

						if (pChild->Name.compare("a", Qt::CaseInsensitive) == 0)
						{
							QString url = pChild->getAttributeValue("href");

							if (url.startsWith("http://d.rutor.org"))
							{
								pCurrentSearchResult = new SearchResult();
								pCurrentSearchResult->TorrentFileUrl = url;
							}
							else if (url.startsWith("/torrent"))
							{
								pCurrentSearchResult->Name = sgml->getInnerText(pChild);
								pCurrentSearchResult->TorrentDescUrl = Url() + url;
							}
						}
					}

					break;
				}

				case 2:
				case 3:
				{
					//Size
					sizeHtmlStr = sgml->getInnerText(pCurrent);
					sizeHtmlStr = sizeHtmlStr.replace("&nbsp;", " ");

					if (sizeHtmlStr.contains(" tb", Qt::CaseInsensitive) || sizeHtmlStr.contains(" gb", Qt::CaseInsensitive) ||
					        sizeHtmlStr.contains(" kb", Qt::CaseInsensitive) || sizeHtmlStr.contains(" mb", Qt::CaseInsensitive) ||
					        sizeHtmlStr.contains(" b", Qt::CaseInsensitive))
					{
						QStringList parts1 = sizeHtmlStr.split(' ');
						bool ok;
						double size = parts1[0].toDouble(&ok);

						if (ok && pCurrentSearchResult != NULL)
						{
							switch (parts1[1][0].toLower().toLatin1())
							{
								case 'k':
									size *= 1024.0;
									break;

								case 'm':
									size *= 1024 * 1024.0;
									break;

								case 'g':
									size *= 1024 * 1024 * 1024.0;
									break;

								case 't':
									size *= 1024 * 1024 * 1024 * 1024.0;
									break;

								case 'b':
									break;
							}

							pCurrentSearchResult->size = size;

							if (tdCounter == 2)
							{
								tdCounter++;
							}
						}
						else if (pCurrentSearchResult != NULL)
						{
							pCurrentSearchResult->size = -1;
						}
					}

					break;
				}

				case 4:
				{
					//seed/peer count
					int nChildCount = pCurrent->Children.count();

					for (int i = 0; i < nChildCount; i++)
					{
						QSgmlTag* pChild = pCurrent->Children.at(i);

						if (pChild->Name.compare("span", Qt::CaseInsensitive) == 0 && pCurrentSearchResult != NULL)
						{
							if (pChild->checkAttribute("class", "green"))
							{
								QString innerHtml = sgml->getInnerText(pChild);
								innerHtml = innerHtml.replace("&nbsp;", "");
								int innerTagEnd = innerHtml.indexOf('>');

								if (innerTagEnd < 0)
								{
									pCurrentSearchResult->seeders = -1;
								}
								else
								{
									QString seechCountStr = innerHtml.mid(innerTagEnd + 1);
									bool ok;
									long seechCount = seechCountStr.toLong(&ok);

									if (ok)
									{
										pCurrentSearchResult->seeders = seechCount;
									}
									else
									{
										pCurrentSearchResult->seeders = -1;
									}
								}
							}
							else if (pChild->checkAttribute("class", "red"))
							{
								QString innerHtml = sgml->getInnerText(pChild);
								innerHtml = innerHtml.replace("&nbsp;", "");
								bool ok;
								long leechCount = innerHtml.toLong(&ok);

								if (ok)
								{
									pCurrentSearchResult->leechers = leechCount;
								}
								else
								{
									pCurrentSearchResult->leechers = -1;
								}
							}
						}
					}

					if (pCurrentSearchResult != NULL)
					{
						pCurrentSearchResult->Engine = Name();
						searchResults.append(pCurrentSearchResult);
						pCurrentSearchResult = NULL;
					}

					break;
				}

				default:
					break;
			}

			tdCounter++;

			if (tdCounter >= 5)
			{
				tdCounter = 0;
			}
		}
	}

	emit SearchReady(searchResults);
}


