#ifndef CREATEBACKUPPAGE_H
#define CREATEBACKUPPAGE_H

#include <QWizardPage>
#include <QtGui>
class CreateBackupPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit CreateBackupPage(QWidget *parent = 0);
    int	nextId () const;
signals:

public slots:
    void browseButtonClick();

    // QWizardPage interface
public:
    void initializePage();
    bool validatePage();

private:
    QLabel *messageLabel;
    QGridLayout *layout;
    QLineEdit* savePathEdit;
    QPushButton* browseButton;
    void createBackup() const;
};

#endif // CREATEBACKUPPAGE_H
