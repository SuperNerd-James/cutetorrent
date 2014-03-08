#include "applybackuppage.h"
#include "backupwizard.h"
ApplyBackupPage::ApplyBackupPage(QWidget *parent):
    QWizardPage(parent)
{
    layout = NULL;
}


int ApplyBackupPage::nextId() const
{
    return BackupWizard::Page_Finish;
}

void ApplyBackupPage::initializePage()
{
    setTitle("APPLY_BAKUP_TITLE");
    if (!layout) {
        layout = new QVBoxLayout;
        messageLabel = new QLabel("APPLY_BAKUP_MESSAGE");
        messageLabel->setWordWrap(true);
        layout->addWidget(messageLabel);
        setLayout(layout);
    }

}
