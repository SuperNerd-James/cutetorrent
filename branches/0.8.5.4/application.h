/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef APPLICATION_H

#include <QApplication>
#include <QHash>
#include <QStringList>
#include "qtsingleapplication.h"
class QDir;
class QTranslator;

typedef QHash<QString, QTranslator*> Translators;

class Application : public QtSingleApplication
{
	Q_OBJECT

public:
	explicit Application(int& argc, char* argv[]);
	~Application();

	static void loadTranslations(const QString& dir);
	static void loadTranslations(const QDir& dir);
	static const QStringList availableLanguages();

public slots:
	static void setLanguage(const QString& locale);
	static QString currentLocale();
private:
	static QString current_locale;
	static QTranslator* current;
	static Translators translators;
};

#endif // APPLICATION_H
