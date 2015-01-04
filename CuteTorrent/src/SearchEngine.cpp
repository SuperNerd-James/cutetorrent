#include "SearchEngine.h"
#include <QMessageBox>
#include "ExtratorrentSearchProvider.h"
SearchEngine::SearchEngine()
{
	m_pSearchProviders.append(new KickassSearchProvider());
	m_pSearchProviders.append(new ExtratorrentSearchProvider());

	for each(ISerachProvider * searchProvider in m_pSearchProviders)
	{
		QObject::connect(dynamic_cast<QObject*>(searchProvider), SIGNAL(SearchReady(QList<SearchResult*>)), this, SLOT(OnSearchReady(QList<SearchResult*>)));
	}

	m_result = SearchItemsStorrage::getInstance();
}

SearchEngine::~SearchEngine()
{
	SearchItemsStorrage::freeInstance();
	qDeleteAll(m_pSearchProviders);
}

void SearchEngine::DoSerach(QString& token, ISerachProvider::SearchCategories category, int page)
{
	m_result->clear();

	for each(ISerachProvider * searchProvider in m_pSearchProviders)
	{
		searchProvider->PeformSearch(token, category, page);
	}
}

void SearchEngine::OnSearchReady(QList<SearchResult*> result)
{
	m_result->append(result);
	emit GotResults();
}

SearchItemsStorrage* SearchEngine::GetResults()
{
	return m_result;
}

QList<ISerachProvider*> SearchEngine::GetSearchProviders()
{
	return m_pSearchProviders;
}
