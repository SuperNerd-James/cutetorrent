#pragma once
#include <Qt>

class SearchResult
{
public:
	QString Name;
	QString TorrentUrl;
	QString Engine;
	long leechers;
	long seeders;
	long size;
};