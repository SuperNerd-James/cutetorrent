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
