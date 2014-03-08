#include "finishpage.h"
FinishPage::FinishPage(QWidget *parent) :
    QWizardPage(parent)
{
    layout = NULL;
}

int FinishPage::nextId() const
{
    return -1;
}

void FinishPage::initializePage()
{
    setTitle("BAKUP_FINISH_TITLE");

    if (!layout) {
        layout = new QVBoxLayout;
        messageLabel = new QLabel("BACKUP_FINISH_MESSAGE");
        messageLabel->setWordWrap(true);
        layout->addWidget(messageLabel);
        setLayout(layout);
    }
}
