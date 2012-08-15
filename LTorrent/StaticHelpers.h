
#ifndef _S_H_H
#define _S_H_H
#include <QString>
#include <QIcon>
#include <QSet>
#include <QFileInfo>
#include <QApplication>

#include <QFile>
#include <QFileDialog>

#include <QInputDialog>
#include <QObject>

#include <QStyle>
#include "TorrentManager.h"

class StaticHelpers
{
public:
	 static QString toKbMbGb(libtorrent::size_type size);
	 static QIcon guessMimeIcon(QString path);
	 static void dellDir(QString path);
};

#endif