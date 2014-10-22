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
#include "DT_mounter.h"
#include <QPainter>
#include <QStyleOption>
#include <QStyle>
#include "StyleEngene.h"

MultipleDTDialog::MultipleDTDialog(QStringList& _files, QWidget* parrent/* =0 */, int flags/* =0 */) : BaseWindow(BaseWindow::OnlyCloseButton, BaseWindow::NoResize)
{
	setupUi(this);
    setupCustomWindow();
    setupWindowIcons();
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(MountSelectedFILE()));
	files.append(_files);
	
	
	
	
	//qDebug() << "files set";
	QStandardItemModel *model = new QStandardItemModel(files.count(),1);
	listView->setModel(model);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	QString temp; 
    QIcon iso = StyleEngene::getInstance()->guessMimeIcon("iso",temp);
	for (int i=0;i<files.count();i++)
	{
		QStandardItem * item = new QStandardItem(iso,files.at(i));
		model->setItem(i,item);
	}
     StyleEngene* style = StyleEngene::getInstance();
     QObject::connect(style,SIGNAL(styleChanged()),this,SLOT(setupWindowIcons()));
}

void MultipleDTDialog::MountSelectedFILE()
{
	int selectedRow=listView->selectionModel()->selectedIndexes().first().row();
	Mount(files.at(selectedRow));
}


void MultipleDTDialog::Mount( QString filename )
{
	DT_mounter::mountImage(filename);
}

void MultipleDTDialog::changeEvent( QEvent *event )
{
	if (event->type()==QEvent::LanguageChange)
	{
		retranslateUi(this);
	}
}

QPushButton* MultipleDTDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* MultipleDTDialog::getTitleBar()
{
	return titleBar;
}

QWidget* MultipleDTDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* MultipleDTDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* MultipleDTDialog::getTitleIcon()
{
	return tbMenu;
}
