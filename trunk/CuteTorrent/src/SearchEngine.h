
#pragma once
#include <QtCore>
#include "KickassSearchProvider.h"

class SearchEngine : public QObject
{
	Q_OBJECT
public:
	SearchEngine();
	~SearchEngine();
	void DoSerach(QString& token, SearchCategories category, int page);
	QList<SearchResult> m_result;
signals:
	void GotResults();
private:
	QList<ISerachProvider*> m_pSearchProviders;

private slots:
	void OnSearchReady(QList<SearchResult> result);
};