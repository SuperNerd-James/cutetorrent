/********************************************************************************
** Form generated from reading UI file 'OpenTorrentDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENTORRENTDIALOG_H
#define UI_OPENTORRENTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenTorrentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QComboBox *GroupComboBox;
    QLabel *label;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *labelSizeTitle;
    QLabel *loaderTextLabel;
    QLabel *labelCommentTitle;
    QLabel *labelNameTitile;
    QLineEdit *labelNameData;
    QTreeView *torrentFilesTreeView;
    QLabel *labelComentData;
    QLabel *loaderGifLabel;
    QLabel *labelSizeData;
    QCheckBox *setSequntialCheckBox;
    QSpacerItem *horizontalSpacer;
    QPushButton *yesButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *OpenTorrentDialog)
    {
        if (OpenTorrentDialog->objectName().isEmpty())
            OpenTorrentDialog->setObjectName(QString::fromUtf8("OpenTorrentDialog"));
        OpenTorrentDialog->resize(436, 482);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        OpenTorrentDialog->setWindowIcon(icon);
        OpenTorrentDialog->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(OpenTorrentDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(OpenTorrentDialog);
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

        m_centralWidget = new QWidget(OpenTorrentDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        m_centralWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(m_centralWidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(m_centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pathEdit = new QLineEdit(groupBox);
        pathEdit->setObjectName(QString::fromUtf8("pathEdit"));

        gridLayout->addWidget(pathEdit, 0, 0, 1, 1);

        browseButton = new QPushButton(groupBox);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));

        gridLayout->addWidget(browseButton, 0, 1, 1, 1);

        GroupComboBox = new QComboBox(groupBox);
        GroupComboBox->setObjectName(QString::fromUtf8("GroupComboBox"));

        gridLayout->addWidget(GroupComboBox, 1, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 3);

        groupBox_2 = new QGroupBox(m_centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        labelSizeTitle = new QLabel(groupBox_2);
        labelSizeTitle->setObjectName(QString::fromUtf8("labelSizeTitle"));

        gridLayout_2->addWidget(labelSizeTitle, 2, 0, 1, 1);

        loaderTextLabel = new QLabel(groupBox_2);
        loaderTextLabel->setObjectName(QString::fromUtf8("loaderTextLabel"));

        gridLayout_2->addWidget(loaderTextLabel, 2, 5, 1, 1);

        labelCommentTitle = new QLabel(groupBox_2);
        labelCommentTitle->setObjectName(QString::fromUtf8("labelCommentTitle"));

        gridLayout_2->addWidget(labelCommentTitle, 1, 0, 1, 2);

        labelNameTitile = new QLabel(groupBox_2);
        labelNameTitile->setObjectName(QString::fromUtf8("labelNameTitile"));

        gridLayout_2->addWidget(labelNameTitile, 0, 0, 1, 1);

        labelNameData = new QLineEdit(groupBox_2);
        labelNameData->setObjectName(QString::fromUtf8("labelNameData"));

        gridLayout_2->addWidget(labelNameData, 0, 2, 1, 4);

        torrentFilesTreeView = new QTreeView(groupBox_2);
        torrentFilesTreeView->setObjectName(QString::fromUtf8("torrentFilesTreeView"));
        torrentFilesTreeView->setStyleSheet(QString::fromUtf8(""));

        gridLayout_2->addWidget(torrentFilesTreeView, 4, 0, 1, 6);

        labelComentData = new QLabel(groupBox_2);
        labelComentData->setObjectName(QString::fromUtf8("labelComentData"));
        labelComentData->setMaximumSize(QSize(16777215, 13));
        labelComentData->setScaledContents(true);
        labelComentData->setWordWrap(true);

        gridLayout_2->addWidget(labelComentData, 1, 2, 1, 4);

        loaderGifLabel = new QLabel(groupBox_2);
        loaderGifLabel->setObjectName(QString::fromUtf8("loaderGifLabel"));

        gridLayout_2->addWidget(loaderGifLabel, 2, 4, 1, 1);

        labelSizeData = new QLabel(groupBox_2);
        labelSizeData->setObjectName(QString::fromUtf8("labelSizeData"));

        gridLayout_2->addWidget(labelSizeData, 2, 1, 1, 3);

        setSequntialCheckBox = new QCheckBox(groupBox_2);
        setSequntialCheckBox->setObjectName(QString::fromUtf8("setSequntialCheckBox"));

        gridLayout_2->addWidget(setSequntialCheckBox, 3, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(266, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 2, 0, 1, 1);

        yesButton = new QPushButton(m_centralWidget);
        yesButton->setObjectName(QString::fromUtf8("yesButton"));

        gridLayout_3->addWidget(yesButton, 2, 1, 1, 1);

        cancelButton = new QPushButton(m_centralWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout_3->addWidget(cancelButton, 2, 2, 1, 1);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(OpenTorrentDialog);
        QObject::connect(browseButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(BrowseButton()));
        QObject::connect(cancelButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(reject()));
        QObject::connect(yesButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(AccepTorrent()));
        QObject::connect(GroupComboBox, SIGNAL(currentIndexChanged(int)), OpenTorrentDialog, SLOT(ChangeGroup()));

        QMetaObject::connectSlotsByName(OpenTorrentDialog);
    } // setupUi

    void retranslateUi(QDialog *OpenTorrentDialog)
    {
        OpenTorrentDialog->setWindowTitle(QApplication::translate("OpenTorrentDialog", "DIALOG_OPEN_TORRENT", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        OpenTorrentDialog->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("OpenTorrentDialog", "DIALOG_OPEN_TORRENT", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_SAVE_PATH", 0, QApplication::UnicodeUTF8));
        browseButton->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_BROWSE_BUTTON", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("OpenTorrentDialog", "DISCRIBTION_LABEL", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_INFO", 0, QApplication::UnicodeUTF8));
        labelSizeTitle->setText(QApplication::translate("OpenTorrentDialog", "SIZE", 0, QApplication::UnicodeUTF8));
        loaderTextLabel->setText(QApplication::translate("OpenTorrentDialog", "RECIVING_METADATA", 0, QApplication::UnicodeUTF8));
        labelCommentTitle->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_INFO_DESCRIBTION", 0, QApplication::UnicodeUTF8));
        labelNameTitile->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_INFO_NAME", 0, QApplication::UnicodeUTF8));
        labelNameData->setText(QString());
        labelComentData->setText(QString());
        loaderGifLabel->setText(QString());
        labelSizeData->setText(QString());
        setSequntialCheckBox->setText(QApplication::translate("OpenTorrentDialog", "SET_SEQUNTIAL", 0, QApplication::UnicodeUTF8));
        yesButton->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_ADD_BUTTON", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_CANCEL_BUTTON", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OpenTorrentDialog: public Ui_OpenTorrentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENTORRENTDIALOG_H
