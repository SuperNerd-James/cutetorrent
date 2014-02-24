#include "backupwizard.h"
#include "intropage.h"
BackupWizard::BackupWizard(QWidget *parent) :
    QWizard(parent)
{
    setPage(Page_Intro,createIntroPage());
    setPage(Page_ApplyBakup,createApplyBakupPage());
    setPage(Page_CreateBakup,createCreateBakupPage());
    setPage(Page_Finish,createFinishPage());
    setOption(QWizard::NoBackButtonOnLastPage,true);
    setOption(QWizard::NoCancelButton,true);
    setWizardStyle(MacStyle);
    setPixmap(BackgroundPixmap,QPixmap(":/images/resources/bacground.png"));
}

int BackupWizard::nextId() const
{
    switch(currentId()) {
        case Page_Intro:
            if (field("create").toBool()) {
                return Page_CreateBakup;
            } else {
                return Page_ApplyBakup;
            }
        case Page_ApplyBakup:
        case Page_CreateBakup:
                return Page_Finish;
        case Page_Finish:
        default:
            return -1;
    }
}

QWizardPage* BackupWizard::createIntroPage()
{
    return new IntroPage(this);
}

QWizardPage *BackupWizard::createCreateBakupPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("CREATE_BAKUP_TITLE");

    QLabel *label = new QLabel("BACKUP_CREATE_BAKUP_MESSAGE");
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}

QWizardPage *BackupWizard::createApplyBakupPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("APPLY_BAKUP_TITLE");

    QLabel *label = new QLabel("APPLY_BAKUP_MESSAGE");
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}

QWizardPage *BackupWizard::createFinishPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("BAKUP_FINISH_TITLE");

    QLabel *label = new QLabel("BACKUP_FINISH_MESSAGE");
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}
