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
//#include "FileTreeCreator.h"
class OpenTorrentDialog : public QDialog , private Ui::OpenTorrentDialog
{
	Q_OBJECT

public:
	OpenTorrentDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OpenTorrentDialog();
	void SetData(QString filename);
	
	int execConditional();
private:
	bool validTorrent;
	StaticHelpers* helper;
	TorrentManager* mgr;
	QString torrentFilename;

private slots:
	void BrowseButton();
	void AccepTorrent();
};
#endif