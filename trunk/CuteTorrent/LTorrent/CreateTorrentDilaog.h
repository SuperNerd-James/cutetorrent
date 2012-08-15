#ifndef CTD_H
#define CTD_H

#include <QDialog>
#include <QMessageBox>
#include "ui_CreateTorrentDialog.h"
#include "TorrentManager.h"
#include <QThread>

class torrentCreatorThread : public QThread {
	Q_OBJECT

		QString input_path;
	QString save_path;
	QStringList trackers;
	QStringList url_seeds;
	QString comment;
	bool is_private;
	qint64 piece_size;
	bool abort;
	QDialog *parent;
	QString filter;

public:
	torrentCreatorThread(QDialog *_parent) {
		parent = _parent;
	}
	~torrentCreatorThread() {
		abort = true;
		wait();
	}
	void create(QString _input_path, QString _save_path,QString _filter, QStringList _trackers, QStringList _url_seeds, QString _comment, bool _is_private, qint64 _piece_size);
	void sendProgressSignal(int progress);

protected:
	void run();

signals:
	void ShowCreationFailture(QString msg);
	void ShowCreationSucces(QString path);

signals:
	void updateProgress(int progress);
};

class CreateTorrentDialog : public QDialog , private Ui::CreateTorrentDialog
{
	Q_OBJECT

public:
	CreateTorrentDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CreateTorrentDialog();

private:
	QString path;
	quint64 listFolder ( QString path );
	torrentCreatorThread* creator;
	TorrentManager* mgr;
	quint64 getPiceSize();
signals:
	void AbortCreation();
private slots:
	void ShowCreationSucces(QString);
	void ShowCreationFailture(QString);
	void UpdateProgressBar(int);
	void BrowseDir();
	void BrowseFile();
	void BeginCreate();
	void Cancel();
	
};
#endif