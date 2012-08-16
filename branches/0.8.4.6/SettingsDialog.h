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
#include <QDialog>
#include "ui_SettingsDialog.h"
#include "QApplicationSettings.h"
#include "GroupForFileFiltering.h"
class SettingsDialog : public QDialog , private Ui::SettingsDialog
{
	Q_OBJECT
private:
	QApplicationSettings* settings;
	QList<GroupForFileFiltering> filterGroups;
public:
 	SettingsDialog(QWidget* parent=0,int flags=0);
	~SettingsDialog();
	void ApplySettingsToSession();
private slots:
	void saveSettings();
	void proxySwitcher();
	void addGroup();
	void removeGroup();
	void browseSavepathGroup();
	void browseDTPath();
	void customCommandSwitcher();
	void showCurrentTrackerSettings();
	void addTracker();
	void trackerAuthSwitcher();
	void showHint();
	void showSelectedGroup(int);
};