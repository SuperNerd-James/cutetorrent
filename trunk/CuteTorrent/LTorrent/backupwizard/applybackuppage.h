#ifndef APPLYBACKUPPAGE_H
#define APPLYBACKUPPAGE_H


#include <QtGui>
class ApplyBackupPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ApplyBackupPage(QWidget *parent = 0);
    int	nextId () const;
signals:

public slots:


    // QWizardPage interface
public:
    void initializePage();
private:
    QLabel *messageLabel;
    QVBoxLayout *layout;
};

#endif // APPLYBACKUPPAGE_H
