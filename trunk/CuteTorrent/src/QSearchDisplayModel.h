#pragma once
#include <QtGui>
#include "SearchEngine.h"
#define SEARCH_RESULTS_COLUNS_COUNT 5
class QSearchDisplayModel: public QAbstractListModel
{
	Q_OBJECT
public:
	QSearchDisplayModel(SearchEngine* pSearchEngine);
	~QSearchDisplayModel();
	enum Role { SearchItemRole = Qt::UserRole };
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
private:
	SearchEngine* m_pSearchEngine;
private slots:
	void OnNewSearchResults();
};

