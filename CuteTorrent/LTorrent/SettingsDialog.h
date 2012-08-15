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