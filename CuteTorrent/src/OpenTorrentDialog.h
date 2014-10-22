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
#pragma once

#include <QDialog>
#include <QFile>
#include <QString>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include "ui_OpenTorrentDialog.h"
#include "TorrentManager.h"
#include "StaticHelpers.h"
#include "FileTreeModel.h"
#include "MetaDataDownloadWaiter.h"
#include "CustomWindow.h"
class OpenTorrentDialog : public BaseWindow<QDialog> , private Ui::OpenTorrentDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent *event);
public:
    
    OpenTorrentDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~OpenTorrentDialog();
	void SetData(QString filename);
	
    int execConditional();
private:
	bool useGroup;
	QList<GroupForFileFiltering> filters;
	bool validTorrent;
	TorrentManager* mgr;
	QString torrentFilename;
	openmagnet_info _info;
	FileTreeModel* model;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
private slots:
    virtual void reject();
	void BrowseButton();
	void AccepTorrent();
	void ChangeGroup();
	void DownloadMetadataCompleted(openmagnet_info);
};

