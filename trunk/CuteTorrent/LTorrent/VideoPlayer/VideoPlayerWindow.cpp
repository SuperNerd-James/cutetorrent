#include "VideoPlayerWindow.h"

#include <QFileDialog>
#include "microtime.h"
#include <QDebug>
#include <QMessageBox>
VideoPlayerWindow::VideoPlayerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::VideoPlayerWindow)
{
    ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose, true);
    ui->m_allTime->setText("00:00");
    ui->m_currentTime->setText("00:00");

   
    ui->m_seekSlider->setMediaObject(ui->m_player->mediaObject());
   
    ui->m_volume->setAudioOutput(ui->m_player->audioOutput());

    connect(ui->m_exitAction,SIGNAL(triggered()),SLOT(close()));
    connect(ui->m_openAction,SIGNAL(triggered()),SLOT(openFile()));
    connect(ui->m_pauseButton,SIGNAL(clicked()),ui->m_player,SLOT(pause()));
    connect(ui->m_playButton,SIGNAL(clicked()),ui->m_player,SLOT(play()));
    connect(ui->m_stopButton,SIGNAL(clicked()),ui->m_player,SLOT(stop()));
    connect(ui->m_player->mediaObject(),SIGNAL(tick(qint64)),SLOT(updateTime(qint64)));
    connect(ui->m_player->mediaObject(),SIGNAL(totalTimeChanged(qint64)),SLOT(setTotalTime(qint64)));
    connect(ui->m_player->mediaObject(),SIGNAL(stateChanged(Phonon::State, Phonon::State)),SLOT(updateStateStatus(Phonon::State, Phonon::State)));
    ui->m_player->videoWidget()->installEventFilter(this);
}

VideoPlayerWindow::~VideoPlayerWindow()
{
	ui->m_player->mediaObject()->stop();
	ui->m_player->mediaObject()->clearQueue();
    delete ui;
}

void VideoPlayerWindow::openFile()
{
    QFileDialog dialog(this);


    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilter("Video Files (*.avi *.mkv)");

    if (dialog.exec())
    {
        QStringList list = dialog.selectedFiles();

        if (!list.isEmpty())
        {
            ui->m_player->mediaObject()->stop();
            ui->m_player->mediaObject()->clearQueue();
            ui->m_player->play(Phonon::MediaSource(list[0]));

        }
    }

}

void VideoPlayerWindow::openFileT( QString filename )
{
	ui->m_player->mediaObject()->stop();
	ui->m_player->mediaObject()->clearQueue();
	ui->m_player->play(Phonon::MediaSource(filename));
}


void VideoPlayerWindow::setTotalTime(qint64 time)
{

    QString format("mm:ss");
    QTime microTime((time/3600000) % 24,(time / 60000) % 60, (time / 1000) % 60);

    if (microTime.hour() > 0)
        format = "h:mm:ss";

    ui->m_allTime->setText(microTime.toString(format));
}

void VideoPlayerWindow::updateStateStatus(Phonon::State newState, Phonon::State oldState)
{
    switch(newState)
    {
    case  Phonon::LoadingState:
        statusBar()->setStatusTip("Loading...");
        break;
    case Phonon::StoppedState:
        statusBar()->setStatusTip("Stoped");
        break;
    case Phonon::PlayingState:
        statusBar()->setStatusTip("Playing...");
        resize(width()+1,height());
        break;
    case Phonon::BufferingState:
        statusBar()->setStatusTip("Buffering...");
        break;
    case Phonon::PausedState:
        statusBar()->setStatusTip("Paused");
        break;
    case Phonon::ErrorState:
        if(ui->m_player->mediaObject()->errorType()==Phonon::FatalError)
        {
            ui->m_player->play();
        }
        else
            statusBar()->setStatusTip("Error "+ui->m_player->mediaObject()->errorString());
        break;
    }


}

bool VideoPlayerWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==ui->m_player->videoWidget() && event->type()==QEvent::MouseButtonDblClick)
    {
        ui->m_player->videoWidget()->setFullScreen(!ui->m_player->videoWidget()->isFullScreen());
        return true;
    }
    return QMainWindow::eventFilter(obj,event);
}

void VideoPlayerWindow::updateTime(qint64 time)
{
    qint64 totalTime = ui->m_player->mediaObject()->totalTime();
    qint64 currentTime = time;
    QString timeStr;
    QString format("h:mm:ss");
    QTime microTime((totalTime/3600000) % 24,(totalTime / 60000) % 60, (totalTime / 1000) % 60);

   //if (ui->m_seekSlider->mediaObject()!=ui->m_player->mediaObject())
        ui->m_seekSlider->setMediaObject( ui->m_player->mediaObject());
    ui->m_allTime->setText(microTime.toString(format));
    if (totalTime > 0 || currentTime > 0)
    {
        ui->m_allTime->setText(microTime.toString(format));
        QString format;
        MicroTime playTime(currentTime);
        MicroTime totTime(totalTime - currentTime);
        format = "h:mm:ss";



        timeStr = playTime.toString(format);
       
    }
    else
        timeStr = "00:00";

    ui->m_currentTime->setText(timeStr);
}
