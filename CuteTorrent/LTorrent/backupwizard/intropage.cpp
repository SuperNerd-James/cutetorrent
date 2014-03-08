#include "intropage.h"
#include "backupwizard.h"
#include <QDebug>

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel(tr("This wizard will help you register your copy of "
                             "<i>Super Product One</i>&trade; or start "
                             "evaluating the product."));
    topLabel->setWordWrap(true);

    registerRadioButton = new QRadioButton(tr("&Register your copy"));
    evaluateRadioButton = new QRadioButton(tr("&Evaluate the product for 30 "
                                              "days"));
    registerRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(registerRadioButton);
    layout->addWidget(evaluateRadioButton);
    setLayout(layout);
}

int IntroPage::nextId() const
{

    if (registerRadioButton->isChecked()) {
        return BackupWizard::Page_CreateBakup;
    } else {
        return BackupWizard::Page_ApplyBakup;
    }
}



bool IntroPage::validatePage()
{
    return true;
}
