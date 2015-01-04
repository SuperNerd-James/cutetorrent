/********************************************************************************
** Form generated from reading UI file 'messagebox.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOX_H
#define UI_MESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageBox
{
public:
    QVBoxLayout* verticalLayout;
    QWidget* titleBar;
    QHBoxLayout* horizontalLayout;
    QLabel* tbMenu;
    QLabel* LTitle;
    QPushButton* pbClose;
    QWidget* m_centralWidget;
    QGridLayout* gridLayout_4;
    QDialogButtonBox* buttonBox;
    QLabel* text;
    QLabel* icon;

    void setupUi(QDialog* MessageBox)
    {
        if(MessageBox->objectName().isEmpty())
        {
            MessageBox->setObjectName(QString::fromUtf8("MessageBox"));
        }

        MessageBox->setWindowModality(Qt::WindowModal);
        MessageBox->resize(400, 129);
        verticalLayout = new QVBoxLayout(MessageBox);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        titleBar = new QWidget(MessageBox);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setMinimumSize(QSize(0, 23));
        titleBar->setMaximumSize(QSize(16777215, 23));
        titleBar->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(titleBar);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 0, 1, 0);
        tbMenu = new QLabel(titleBar);
        tbMenu->setObjectName(QString::fromUtf8("tbMenu"));
        tbMenu->setMaximumSize(QSize(15, 15));
        tbMenu->setPixmap(QPixmap(QString::fromUtf8(":/icons/app.ico")));
        tbMenu->setScaledContents(true);
        horizontalLayout->addWidget(tbMenu);
        LTitle = new QLabel(titleBar);
        LTitle->setObjectName(QString::fromUtf8("LTitle"));
        LTitle->setMinimumSize(QSize(100, 22));
        LTitle->setMaximumSize(QSize(16777215, 22));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        font.setStyleStrategy(QFont::PreferDefault);
        LTitle->setFont(font);
        LTitle->setStyleSheet(QString::fromUtf8(""));
        LTitle->setTextFormat(Qt::AutoText);
        horizontalLayout->addWidget(LTitle);
        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));
        pbClose->setMinimumSize(QSize(22, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);
        horizontalLayout->addWidget(pbClose);
        verticalLayout->addWidget(titleBar);
        m_centralWidget = new QWidget(MessageBox);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        m_centralWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_4 = new QGridLayout(m_centralWidget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(0);
        gridLayout_4->setVerticalSpacing(2);
        gridLayout_4->setContentsMargins(9, 6, 9, 9);
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::NoButton);
        buttonBox->setCenterButtons(true);
        gridLayout_4->addWidget(buttonBox, 1, 0, 1, 2);
        text = new QLabel(m_centralWidget);
        text->setObjectName(QString::fromUtf8("text"));
        text->setWordWrap(true);
        text->setTextInteractionFlags(Qt::NoTextInteraction);
        gridLayout_4->addWidget(text, 0, 1, 1, 1);
        icon = new QLabel(m_centralWidget);
        icon->setObjectName(QString::fromUtf8("icon"));
        icon->setMaximumSize(QSize(60, 60));
        icon->setStyleSheet(QString::fromUtf8("#icon{\n"
                                              "  margin-right:10px;\n"
                                              "  margin-top:10px;\n"
                                              "}"));
        icon->setScaledContents(true);
        icon->setTextInteractionFlags(Qt::NoTextInteraction);
        gridLayout_4->addWidget(icon, 0, 0, 1, 1);
        verticalLayout->addWidget(m_centralWidget);
        retranslateUi(MessageBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), MessageBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MessageBox, SLOT(reject()));
        QMetaObject::connectSlotsByName(MessageBox);
    } // setupUi

    void retranslateUi(QDialog* MessageBox)
    {
        MessageBox->setWindowTitle(QApplication::translate("MessageBox", "Dialog", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("MessageBox", "MB_TITLE", 0, QApplication::UnicodeUTF8));
        text->setText(QApplication::translate("MessageBox", "TextLabel", 0, QApplication::UnicodeUTF8));
        icon->setText(QString());
    } // retranslateUi

};

namespace Ui
{
class MessageBox: public Ui_MessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOX_H
