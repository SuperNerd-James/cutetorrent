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
#ifndef OTD_H
#define OTD_H

#include <QDialog>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include "ui_OpenTorrentDialog.h"
#include "TorrentManager.h"
#include "StaticHelpers.h"
#include "FileTreeModel.h"
#include "MetaDataDownloadWaiter.h"

class OpenTorrentDialog : public QDialog , private Ui::OpenTorrentDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent *event);
public:
    enum TitleMode { CleanTitle = 0, OnlyCloseButton, MenuOff, MaxMinOff, FullScreenMode, MaximizeModeOff, MinimizeModeOff, FullTitle };
	OpenTorrentDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OpenTorrentDialog();
	void SetData(QString filename);
	
    int execConditional();
public slots:
    void minimizeBtnClicked();
private:
	bool useGroup;
	QList<GroupForFileFiltering> filters;
	bool validTorrent;
	TorrentManager* mgr;
	QString torrentFilename;
	openmagnet_info _info;
	FileTreeModel* model;
    /**
        * @brief centralLayout Main layout where the centralwidget will be set.
        */
    QHBoxLayout *centralLayout;
    /**
        * @brief dragPosition Increment of the position movement.
        */
    QPoint dragPosition;
    /**
        * @brief m_titleMode Flags that defines the current titlebar mode.
        */
    TitleMode m_titleMode;
    /**
        * @brief moveWidget Specifies if the window is in move action.
        */
    bool moveWidget;
    /**
        * @brief inResizeZone Specifies if the mouse is in resize zone.
        */
    bool inResizeZone;
    /**
        * @brief allowToResize Specifies if the mouse is allowed to resize.
        */
    bool allowToResize;
    /**
        * @brief resizeVerSup Specifies if the resize is in the top of the window.
        */
    bool resizeVerSup;
    /**
        * @brief resizeHorEsq Specifies if the resize is in the left of the window.
        */
    bool resizeHorEsq;
    /**
        * @brief resizeDiagSupEsq Specifies if the resize is in the top left of the window.
        */
    bool resizeDiagSupEsq;
    /**
        * @brief resizeDiagSupDer Specifies if the resize is in the top right of the window.
        */
    bool resizeDiagSupDer;

    /**
        * @brief mouseMoveEvent Overloaded member that moves of resizes depending of the
        * configuration sotred at mousePressEvent().
        * @param e The mouse event.
        */
    void mouseMoveEvent(QMouseEvent *e);
    /**
        * @brief mousePressEvent Overloaded member that stores that changes the cursor and
        * configures the side that is changing.
        * @param e The mouse event.
        */
    void mousePressEvent(QMouseEvent *e);
    /**
        * @brief mouseReleaseEvent Overloaded member that removes the configuration set in mousePressEvent().
        * @param e The mouse event.
        */
    void mouseReleaseEvent(QMouseEvent *e);
    /**
        * @brief mouseDoubleClickEvent Overloadad member that maximize/restore the window if is
        * doubleclicked and the position of the mouse is not the top left of the window (menu zone).
        * @param e The mouse event.
        */
    void mouseDoubleClickEvent(QMouseEvent *e);
    /**
        * @brief paintEvent Overloaded method that allows to customize the styles of the window.
        */
    void paintEvent (QPaintEvent *);
    /**
        * @brief resizeWindow Method that calculates the resize and new position of the window an
        * does this actions.
        * @param e The mouse event to calculate the new position and size.
        */
    void resizeWindow(QMouseEvent *e);

    void moveWindow(QMouseEvent *e);
	void setupGroupComboBox();
    void setupCustomeWindow();
private slots:
    virtual void reject();
	void BrowseButton();
	void AccepTorrent();
	void ChangeGroup();
	void DownloadMetadataCompleted(openmagnet_info);
};
#endif
