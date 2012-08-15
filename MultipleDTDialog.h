#include "ui_multipleDt.h"

#include <QDialog>

class MultipleDTDialog : public QDialog, Ui::DTMountDialog
{
public:
	MultipleDTDialog(QStringList _files,QWidget* parrent=0,int flags=0);
	
private:
	
	QStringList files;
private slots:
	void MountSelected();
	void Mount(QString filename);
	
};