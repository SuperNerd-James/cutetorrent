/********************************************************************************
** Form generated from reading UI file 'MediaControls.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDIACONTROLS_H
#define UI_MEDIACONTROLS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <phonon/seekslider.h>
#include <phonon/volumeslider.h>

QT_BEGIN_NAMESPACE

class Ui_MediaControls
{
public:
    QPushButton* reverseButton;
    QPushButton* openFileButton;
    QPushButton* playButton;
    QPushButton* fullScreenButton;
    QPushButton* pauseButton;
    QPushButton* openUrlButton;
    QPushButton* forwardButton;
    QLabel* totalTimeLabel;
    QLabel* currentTimeLabel;
    Phonon::SeekSlider* seekSlider;
    Phonon::VolumeSlider* volumeSlider;

    void setupUi(QWidget* MediaControls)
    {
        if(MediaControls->objectName().isEmpty())
        { MediaControls->setObjectName(QString::fromUtf8("MediaControls")); }

        MediaControls->resize(372, 66);
        MediaControls->setFocusPolicy(Qt::ClickFocus);
        MediaControls->setContextMenuPolicy(Qt::NoContextMenu);
        MediaControls->setStyleSheet(QString::fromUtf8("#MediaControls{\n"
                                     "border-radius: 10px;\n"
                                     "border:solid 1px 2D2D2D;\n"
                                     "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(93, 93, 93, 255), stop:1 rgba(45, 45, 45, 255));\n"
                                     "}\n"
                                     ""));
        reverseButton = new QPushButton(MediaControls);
        reverseButton->setObjectName(QString::fromUtf8("reverseButton"));
        reverseButton->setGeometry(QRect(188, 10, 28, 24));
        reverseButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                     "border-color:\"transparent\""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/PlayerControlImages/RateButtonReverse.png"), QSize(), QIcon::Normal, QIcon::Off);
        reverseButton->setIcon(icon);
        reverseButton->setFlat(true);
        openFileButton = new QPushButton(MediaControls);
        openFileButton->setObjectName(QString::fromUtf8("openFileButton"));
        openFileButton->setGeometry(QRect(256, 10, 28, 24));
        openFileButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                      "border-color:\"transparent\""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/PlayerControlImages/FileButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        openFileButton->setIcon(icon1);
        openFileButton->setFlat(true);
        playButton = new QPushButton(MediaControls);
        playButton->setObjectName(QString::fromUtf8("playButton"));
        playButton->setGeometry(QRect(120, 10, 28, 24));
        playButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                  "border-color:\"transparent\""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/PlayerControlImages/PlayButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        playButton->setIcon(icon2);
        playButton->setFlat(true);
        fullScreenButton = new QPushButton(MediaControls);
        fullScreenButton->setObjectName(QString::fromUtf8("fullScreenButton"));
        fullScreenButton->setGeometry(QRect(324, 10, 28, 24));
        fullScreenButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                        "border-color:\"transparent\""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/PlayerControlImages/FullscreenButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        fullScreenButton->setIcon(icon3);
        fullScreenButton->setFlat(true);
        pauseButton = new QPushButton(MediaControls);
        pauseButton->setObjectName(QString::fromUtf8("pauseButton"));
        pauseButton->setGeometry(QRect(154, 10, 28, 24));
        pauseButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                   "border-color:\"transparent\""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/PlayerControlImages/PauseButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        pauseButton->setIcon(icon4);
        pauseButton->setFlat(true);
        openUrlButton = new QPushButton(MediaControls);
        openUrlButton->setObjectName(QString::fromUtf8("openUrlButton"));
        openUrlButton->setGeometry(QRect(290, 10, 28, 24));
        openUrlButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                     "border-color:\"transparent\""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/PlayerControlImages/UrlButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        openUrlButton->setIcon(icon5);
        openUrlButton->setFlat(true);
        forwardButton = new QPushButton(MediaControls);
        forwardButton->setObjectName(QString::fromUtf8("forwardButton"));
        forwardButton->setGeometry(QRect(222, 10, 28, 24));
        forwardButton->setStyleSheet(QString::fromUtf8("background-color:\"transparent\";\n"
                                     "border-color:\"transparent\""));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/PlayerControlImages/RateButtonForward.png"), QSize(), QIcon::Normal, QIcon::Off);
        forwardButton->setIcon(icon6);
        forwardButton->setFlat(true);
        totalTimeLabel = new QLabel(MediaControls);
        totalTimeLabel->setObjectName(QString::fromUtf8("totalTimeLabel"));
        totalTimeLabel->setGeometry(QRect(305, 40, 44, 22));
        QPalette palette;
        QBrush brush(QColor(227, 227, 227, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 0));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        totalTimeLabel->setPalette(palette);
        currentTimeLabel = new QLabel(MediaControls);
        currentTimeLabel->setObjectName(QString::fromUtf8("currentTimeLabel"));
        currentTimeLabel->setGeometry(QRect(10, 40, 44, 22));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        currentTimeLabel->setPalette(palette1);
        seekSlider = new Phonon::SeekSlider(MediaControls);
        seekSlider->setObjectName(QString::fromUtf8("seekSlider"));
        seekSlider->setGeometry(QRect(73, 40, 211, 20));
        volumeSlider = new Phonon::VolumeSlider(MediaControls);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        volumeSlider->setGeometry(QRect(10, 10, 109, 22));
        retranslateUi(MediaControls);
        QMetaObject::connectSlotsByName(MediaControls);
    } // setupUi

    void retranslateUi(QWidget* MediaControls)
    {
        MediaControls->setWindowTitle(QApplication::translate("MediaControls", "Form", 0, QApplication::UnicodeUTF8));
        reverseButton->setText(QString());
        openFileButton->setText(QString());
        playButton->setText(QString());
        fullScreenButton->setText(QString());
        pauseButton->setText(QString());
        openUrlButton->setText(QString());
        forwardButton->setText(QString());
        totalTimeLabel->setText(QApplication::translate("MediaControls", "00:00:00", 0, QApplication::UnicodeUTF8));
        currentTimeLabel->setText(QApplication::translate("MediaControls", "00:00:00", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui
{
class MediaControls: public Ui_MediaControls {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDIACONTROLS_H
