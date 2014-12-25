#include "SearchEngine.h"
#include <QMessageBox>
SearchEngine::SearchEngine()
{
	m_pSearchProviders.append(new KickassSearchProvider());
	for each (ISerachProvider* searchProvider in m_pSearchProviders)
	{
		QObject::connect(dynamic_cast<QObject*>(searchProvider), SIGNAL(SearchReady(QList<SearchResult>)), this, SLOT(OnSearchReady(QList<SearchResult>)));
	}
}

SearchEngine::~SearchEngine()
{
	qDeleteAll(m_pSearchProviders);
}

void SearchEngine::DoSerach(QString& token, SearchCategories category, int page)
{
	m_result.clear();
	for each (ISerachProvider* searchProvider in m_pSearchProviders)
	{
		searchProvider->PeformSearch(token, category, page);
	}
}

void SearchEngine::OnSearchReady(QList<SearchResult> result)
{
	QMessageBox::information(NULL, QString("OnSearchReady"), QString::number(result.length()));
	m_result.append(result);
	emit GotResults();
}
