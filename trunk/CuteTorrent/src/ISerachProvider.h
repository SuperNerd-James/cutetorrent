#pragma once
#include <QtCore>
#include "SerachResult.h"



class ISerachProvider : public QObject
{
    Q_OBJECT
public:
    enum SearchCategories
    {
        Music = 0,
        TV = 1,
        Porn = 2,
        Software = 4,
        Games = 8,
        Anime = 16,
        Books = 32,
        Movie = 64,
        All = Music | TV | Porn | Software | Games | Anime | Books | Movie
    };

    virtual QString Name() = 0;
    virtual QString Url() = 0;
    virtual int SupportedCategories() = 0;
    virtual void PeformSearch(QString token, SearchCategories category, int page) = 0;
signals:
    virtual void SearchReady(QList<SearchResult*> result) = 0;
    virtual void Error(QString error) = 0;
};

Q_DECLARE_INTERFACE(ISerachProvider, "ISerachProvider")