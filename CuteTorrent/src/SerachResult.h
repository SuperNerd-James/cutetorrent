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
    long long size;
    bool operator == (SearchResult other)
    {
        if(Name != other.Name)
        {
            return false;
        }

        if(TorrentUrl != other.TorrentUrl)
        {
            return false;
        }

        if(Engine != other.Engine)
        {
            return false;
        }

        if(leechers != other.leechers)
        {
            return false;
        }

        if(seeders != other.seeders)
        {
            return false;
        }

        if(size != other.size)
        {
            return false;
        }

        return true;
    }
};


Q_DECLARE_METATYPE(SearchResult*)
