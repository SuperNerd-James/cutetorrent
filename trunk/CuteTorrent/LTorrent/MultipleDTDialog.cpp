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

#include "MultipleDTDialog.h"
#include "QApplicationSettings.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QProcess>
#include <QDebug>
#include <QAbstractItemView>
#include "StaticHelpers.h"


MultipleDTDialog::MultipleDTDialog(QStringList& _files,QWidget* parrent/* =0 */,int flags/* =0 */)
{
	setupUi(this);
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(MountSelectedFILE()));
	//qDebug() << "files will be set now";
	for (int i=0;i<_files.count();i++)
	{
	
		files.append(_files.at(i));
	
	}
	
	
	//qDebug() << "files set";
	QStandardItemModel *model = new QStandardItemModel(files.count(),1);
	listView->setModel(model);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	QIcon iso = StaticHelpers::guessMimeIcon("iso");
	for (int i=0;i<files.count();i++)
	{
		QStandardItem * item = new QStandardItem(iso,files.at(i));
		model->setItem(i,item);
	}
}

void MultipleDTDialog::MountSelectedFILE()
{
	
	int selectedRow=listView->selectionModel()->selectedIndexes().first().row();
	Mount(files.at(selectedRow));
	
}


void MultipleDTDialog::Mount( QString filename )
{

	QApplicationSettings* settings=QApplicationSettings::getInstance();
	
	QString exe = settings->valueString("DT","Executable");
	if (exe.isEmpty())
	{
		QMessageBox::warning(this,"DT Mounter",QString::fromLocal8Bit("Укажите в настройках путь к Daemon Tools!"));
		return;
	}
	bool useCustomCmd = settings->valueBool("DT","UseCustomCommand");
	int driveNum = settings->valueInt("DT","Drive");
	QString command = useCustomCmd ?  settings->valueString("DT","CustomtCommand"): settings->valueString("DT","DefaultCommand"); 
	QProcess *dt = new QProcess(this);
	QStringList args;
	/*args << "-mount";
	args << command.arg(QString::number(driveNum)).arg(images.first());*/
	//qDebug() << exe << command.arg(QString::number(driveNum)).arg(filename);
	dt->setNativeArguments(command.arg(QString::number(driveNum)).arg(filename));
	dt->start(exe,args);
	QApplicationSettings::FreeInstance();
	if (!dt->waitForStarted(5000))
	{
		QMessageBox::warning(this,"DT Mounter",QString::fromLocal8Bit("Не удалось запустить ")+exe);
		return;
	}
			
	dt->waitForFinished();
	delete dt;
	
}
