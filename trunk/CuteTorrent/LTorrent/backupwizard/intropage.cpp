#include "intropage.h"

IntroPage::IntroPage(QWidget *parent) :
    QWizardPage (parent)
{
}


void IntroPage::initializePage()
{
    setTitle("BAKUP_INTRO_TITLE");

    QLabel *label = new QLabel("BACKUP_INTRO_MESSAGE");
    label->setWordWrap(true);
    QRadioButton *careateRadioButton = new QRadioButton("CREATE_BAKUP");
    QRadioButton *applyRadioButton  = new QRadioButton("APPLY_BAKUP");
    QVBoxLayout *layout = new QVBoxLayout;
    registerField("create",careateRadioButton);
    registerField("apply",applyRadioButton);
    layout->addWidget(label);
    layout->addWidget(careateRadioButton);
    layout->addWidget(applyRadioButton);
    setLayout(layout);
}

void IntroPage::cleanupPage()
{

}

bool IntroPage::validatePage()
{
    return field("create").toBool() || field("apply").toBool();
}
