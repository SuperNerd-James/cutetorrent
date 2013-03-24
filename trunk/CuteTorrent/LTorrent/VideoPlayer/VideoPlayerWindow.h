#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGlobal>
#include <phonon/MediaObject>
#include <phonon/VideoWidget>
#include "ui_VideoPlayerWindow.h"
namespace Ui
{
    class MainWindow;
}

class VideoPlayerWindow : public QMainWindow
{
    Q_OBJECT

public:
    VideoPlayerWindow(QWidget *parent = 0);
    ~VideoPlayerWindow();
	void openFileT(QString filename);
private slots:

    void openFile();
	
    void updateTime(qint64 time = 0);

    void setTotalTime(qint64 time = 0);

    void updateStateStatus(Phonon::State newState, Phonon::State oldState);
private:
    bool totaltimeUpdated;
    Ui::VideoPlayerWindow *ui;
protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // MAINWINDOW_H
