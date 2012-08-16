/*
CuteTorrent BitTorrenttClient with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) <year>  <name of author>

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
#include <QAbstractItemView>
#include "StaticHelpers.h"
MultipleDTDialog::MultipleDTDialog(QStringList _files,QWidget* parrent/* =0 */,int flags/* =0 */)
{
	
	files=_files;
	QStandardItemModel *model = new QStandardItemModel(files.count(),1);
	listView->setModel(model);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	QIcon iso = StaticHelpers::guessMimeIcon("iso");
	for (int i=0;i<files.count();i++)
	{
		QStandardItem * item = new QStandardItem(iso,files.at(i));
		model->setItem(0,i,item);
	}
}

void MultipleDTDialog::MountSelected()
{
	
	int selectedRow=listView->selectionModel()->selectedIndexes().first().row();
	Mount(files.at(selectedRow));
	
}


void MultipleDTDialog::Mount( QString filename )
{

	QApplicationSettings* settings=QApplicationSettings::getInstance();
	QString exe = settings->valueString("DT","Executable");
	bool useCustomCmd = settings->valueBool("DT","UseCustomCommand");
	int driveNum = settings->valueInt("DT","Drive");
	QString command = useCustomCmd ?  settings->valueString("DT","CustomtCommand"): settings->valueString("DT","DefaultCommand"); 
	QProcess dt;
	QStringList args;
	args << "-mount" << command.arg(QString::number(driveNum)).arg(filename);
	dt.start(exe,args);
	QApplicationSettings::FreeInstance();
}