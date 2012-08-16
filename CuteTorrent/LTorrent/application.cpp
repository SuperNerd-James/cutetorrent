#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include "application.h"
#include <QMessageBox>
QTranslator* Application::current = 0;
Translators Application::translators;
QString Application::current_locale="";
Application::Application(int& argc, char* argv[])
	: QtSingleApplication(argc, argv)
{
}

Application::~Application()
{
}

void Application::loadTranslations(const QString& dir)
{
	loadTranslations(QDir(dir));
}

void Application::loadTranslations(const QDir& dir)
{
	// <language>_<country>.qm
	QString filter = "*_*.qm";
	QDir::Filters filters = QDir::Files | QDir::Readable;
	QDir::SortFlags sort = QDir::Name;
	QFileInfoList entries = dir.entryInfoList(QStringList() << filter, filters, sort);
	foreach (QFileInfo file, entries)
	{
		// pick country and language out of the file name
		QStringList parts = file.baseName().split("_");
		QString language = parts.at(parts.count() - 2).toLower();
		QString country  = parts.at(parts.count() - 1).toUpper();

		// construct and load translator
		
		QTranslator* translator = new QTranslator(instance());
		if (translator->load(file.absoluteFilePath()))
		{
			QString locale = language + "_" + country;
			
			translators.insert(locale, translator);
		}
	}
}
QString Application::currentLocale()
{
	return current_locale;
}
const QStringList Application::availableLanguages()
{
	// the content won't get copied thanks to implicit sharing and constness
	return QStringList(translators.keys());
}

void Application::setLanguage(const QString& locale)
{
	// remove previous
	if (current)
	{
		removeTranslator(current);
	}
	current_locale=locale;
	// install new
	
	current = translators.value(locale, 0);
	if (current)
	{
		installTranslator(current);
	}
}
