#include "applybackuppage.h"
#include "backupwizard.h"
#include "../FileTreeModel.h"
#include <algorithm>
template <class T> void swap ( T& a, T& b )
{
  T c(a); a=b; b=c;
}

ApplyBackupPage::ApplyBackupPage(QWidget *parent):
    QWizardPage(parent)
{
    setTitle("APPLY_BAKUP_TITLE");


    gridLayout = new QGridLayout(this);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    backupPathLineEdit = new QLineEdit(this);
    backupPathLineEdit->setObjectName(QString::fromUtf8("lineEdit"));
    backupPathLineEdit->setEnabled(false);
    gridLayout->addWidget(backupPathLineEdit, 0, 0, 1, 1);

    browsePushButton = new QPushButton(this);
    browsePushButton->setText(tr("BROWSE"));
    QObject::connect(browsePushButton,SIGNAL(clicked()),this,SLOT(browseButtonClicked()));
    gridLayout->addWidget(browsePushButton, 0, 1, 1, 1);

    drivesCheckBox = new QCheckBox(this);
    drivesCheckBox->setObjectName(QString::fromUtf8("checkBox"));
    drivesCheckBox->setText(tr("CHANGE_DRIVE"));
    gridLayout->addWidget(drivesCheckBox, 1, 0, 1, 1);

    drivesComboBox = new QComboBox(this);
    drivesComboBox->setObjectName(QString::fromUtf8("comboBox"));
    QStringList drives;
    foreach (QFileInfo driveInfo, QDir::drives()) {
        drives.append(driveInfo.path());
    }
    drivesComboBox->addItems(drives);
    drivesComboBox->setMaximumWidth(100);
    gridLayout->addWidget(drivesComboBox, 1, 1, 1, 1);
    changePathGroupBox = new QGroupBox(this);
    changePathGroupBox->setObjectName(QString::fromUtf8("groupBox"));
    changePathGroupBox->setCheckable(true);
    changePathGroupBox->setTitle(tr("CHANGE_PATH"));
    changePathGroupBox->setChecked(false);
    gridLayout_2 = new QGridLayout(changePathGroupBox);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    tableWidget = new QTableWidget(changePathGroupBox);
    if (tableWidget->columnCount() < 3)
        tableWidget->setColumnCount(3);
    QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
    __qtablewidgetitem->setText(tr("CHANGE"));
    tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
    QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
    __qtablewidgetitem1->setText(tr("PATH"));
    tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
    QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
    __qtablewidgetitem2->setText(tr("NEW_PATH"));
    tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
    tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
    tableWidget->verticalHeader()->hide();
    gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);
    gridLayout->addWidget(changePathGroupBox, 2, 0, 1, 3);
    setLayout(gridLayout);

}


int ApplyBackupPage::nextId() const
{
    return BackupWizard::Page_Finish;
}
QStringList ApplyBackupPage::GetLongestCommonSubstr(QStringList strings) {
    strings.sort();
    FileTreeModel model;
    foreach (QString string, strings) {
        model.addPath(string,"");
    }
    return model.getUnickPathes();
}


bool ApplyBackupPage::parseData(QZipReader &reader)
{
    QString pathResumeData = reader.fileData("CT_DATA/path.resume");
    if (pathResumeData.isEmpty()) {
        QMessageBox::critical(this,tr("ERROR"),tr("BAD_RESUME_DATA"));
        return false;
    }
    QStringList pathes;
    QStringList lines = pathResumeData.split("\n");
    foreach (QString line, lines) {
        QStringList parts = line.split('|');
        if (parts.length()>2) {
            pathes << parts[1];
        }

    }
    QStringList unick=GetLongestCommonSubstr(pathes);
    int row=0;
    tableWidget->setRowCount(unick.size());
    foreach (QString line, unick) {
        QTableWidgetItem* first =  new QTableWidgetItem();
        first->setFlags(first->flags() | Qt::ItemIsUserCheckable);
        first->setCheckState(Qt::Unchecked);
        tableWidget->setItem(row,0,first);
        QTableWidgetItem* second = new QTableWidgetItem();
        second->setText(line);
        tableWidget->setItem(row,1,second);
        row++;
    }
    tableWidget->resizeColumnsToContents();
    return true;
}

void ApplyBackupPage::browseButtonClicked()
{
    backupPathLineEdit->setText(QFileDialog::getOpenFileName(this,tr("BACKUP_CHOOSE"),QApplication::applicationDirPath()+QDir::separator()));
    QZipReader reader(backupPathLineEdit->text());
    if (reader.exists()) {
        parseData(reader);
    } else {
        QMessageBox::critical(this,tr("ERROR"),tr("UNABLE_TO_OPEN_BACKUP"));
        return;
    }

}
