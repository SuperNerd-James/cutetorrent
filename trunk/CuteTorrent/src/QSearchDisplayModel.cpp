#include "QSearchDisplayModel.h"
#include "StaticHelpers.h"


QSearchDisplayModel::QSearchDisplayModel(SearchEngine* pSearchEngine, QListView* pTorrentListView) : QAbstractListModel()
{
	if(pSearchEngine == NULL)
	{
		throw std::logic_error("Search engine could not be null.");
	}

	m_pTorrentListView = pTorrentListView;
	m_pSearchEngine = pSearchEngine;
	QObject::connect(m_pSearchEngine, SIGNAL(GotResults()), this, SLOT(OnNewSearchResults()));
	SearchItemsStorrage* pItems = m_pSearchEngine->GetResults();
	QObject::connect(pItems, SIGNAL(reset()), this, SLOT(OnNewSearchResults()));
	setupContextMenu();
}


QSearchDisplayModel::~QSearchDisplayModel()
{
}

int QSearchDisplayModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return m_pSearchEngine->GetResults()->length();
}

int QSearchDisplayModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
	return SEARCH_RESULTS_COLUNS_COUNT;
}

QVariant QSearchDisplayModel::data(const QModelIndex& index, int role /*= Qt::DisplayRole*/) const
{
	if(index.isValid())
	{
		int row = index.row();
		int col = index.column();
		SearchItemsStorrage* results = m_pSearchEngine->GetResults();

		if(row > results->length())
		{
			return QVariant();
		}

		SearchResult* res = (*results) [row];

		switch(role)
		{
			case Qt::DisplayRole:
			{
				switch(col)
				{
					case 0:
					{
						return qVariantFromValue(res->Name);
					}

					case 1:
					{
						return qVariantFromValue(StaticHelpers::toKbMbGb(res->size));
					}

					case 2:
					{
						return qVariantFromValue(res->leechers);
					}

					case 3:
					{
						return qVariantFromValue(res->seeders);
					}

					case 4:
					{
						return qVariantFromValue(res->Engine);
					}
				}
			}

			case SearchItemRole:
			{
				return qVariantFromValue(res);
			}
		}
	}

	return QVariant();
}

QVariant QSearchDisplayModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
	if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		static QStringList captions;

		if(captions.empty())
		{
			captions << "cap1" << "cap2" << "cap3" << "cap4" << "cap5";
		}

		return captions[section];
	}

	return QVariant();
}

void QSearchDisplayModel::OnNewSearchResults()
{
	reset();
}

void QSearchDisplayModel::contextualMenu(const QPoint& point)
{
	if (m_pTorrentListView->model() != this)
	{
		return;
	}

	QModelIndex modelIndex = m_pTorrentListView->indexAt(point);

	if (modelIndex.isValid())
	{
		m_pContextMenu->exec(m_pTorrentListView->mapToGlobal(point));
	}
}

void QSearchDisplayModel::setupContextMenu()
{
	m_pContextMenu = new QMenu(m_pTorrentListView);
	QAction* act1 = new QAction("Act1", m_pContextMenu);
	m_pContextMenu->addAction(act1);
	QAction* act2 = new QAction("Act2", m_pContextMenu);
	m_pContextMenu->addAction(act2);
}

void QSearchDisplayModel::retranslate()
{
	
}



