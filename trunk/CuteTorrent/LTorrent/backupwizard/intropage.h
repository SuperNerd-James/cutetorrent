#ifndef INTROPAGE_H
#define INTROPAGE_H

#include <QWizardPage>
#include <QtGui>
class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit IntroPage(QWidget *parent = 0);

signals:

public slots:


    // QWizardPage interface
public:
    void initializePage();
    void cleanupPage();
    bool validatePage();
};

#endif // INTROPAGE_H
