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
#include <QDialog>
#include <QStackedWidget>
#include "ui_SettingsDialog.h"
#include "QApplicationSettings.h"
#include "GroupForFileFiltering.h"
#include "searchitem.h"
#include <QMouseEvent>
#include "tracker/torrentracker.h"
#include "webControll/RconWebService.h"
class SettingsDialog : public QDialog , private Ui::SettingsDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent *event);
public:
    enum TitleMode { CleanTitle = 0, OnlyCloseButton, MenuOff, MaxMinOff, FullScreenMode, MaximizeModeOff, MinimizeModeOff, FullTitle };
private:
	QApplicationSettings* settings;
    QList<SearchItem> searchSources;
	QList<GroupForFileFiltering> filterGroups;
	QList<SchedulerTask> tasks;
	QDateTimeEdit* previousFocuse;
    TorrentTracker* tracker;
    RconWebService* rcon;
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
	void FillFilteringGroups();
	void FillTorrentTab();
	void FillHDDTab();
	void FillDTTab();
	void FillWebUITab();
    void FillKeyMapTab();
	void SetupSchedullerTab();
    void FillSearchTab();
    void setupCustomeWindow();
public:
    SettingsDialog(QWidget* parent=0,int flags=0);
	~SettingsDialog();
    void ApplySettingsToSession();
public slots:
    void minimizeBtnClicked();
signals:
	void needRetranslate();
	void tasksChanged();
private slots:
    void chooseAction(QAbstractButton * button);
	void ApplyAndClose();
	void ApplySettings();
	void addGroup();
	void removeGroup();
	void browseSavepathGroup();
	void browseDTPath();
	void showSelectedGroup(int);
	void SetDate(QDate date);
	void AddTask();
	void DeleteTask();
	void UpdateSchedullerTab( int index );
	void StartRcon();
	void StopRcon();
    void searchItemChanged(int index);
    void addSearchitem();
    void removeSearchItem();

    void on_pushButton_clicked();
};
