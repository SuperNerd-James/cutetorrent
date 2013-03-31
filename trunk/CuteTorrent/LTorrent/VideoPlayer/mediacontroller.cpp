/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mediacontroller.h"

#include <phonon/mediasource.h>
#include <phonon/mediaobject.h>

#include <QFileDialog>

MediaController::MediaController(Phonon::VideoWidget *parent) :
QObject(parent)
{
    m_media = new Phonon::MediaObject(this);
	connect(m_media,SIGNAL(stateChanged(Phonon::State, Phonon::State)),SLOT(updateStateStatus(Phonon::State, Phonon::State)));
	m_AudioOutput = new Phonon::AudioOutput(Phonon::VideoCategory);
	Phonon::createPath( m_media, m_AudioOutput );
}

Phonon::MediaObject *MediaController::mediaObject() const
{
    return m_media;
}

void MediaController::openFile()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a new file to play"), "");
    if (!file.isEmpty()) {
        Phonon::MediaSource s(file);
        playSource(s);
    }
}

void MediaController::openURL()
{

}

void MediaController::playSource(const Phonon::MediaSource &s)
{
    m_media->setCurrentSource(s);
    m_media->play();
	
	
}

void MediaController::play()
{
	m_media->play();
}

void MediaController::pause()
{
	m_media->pause();
}

Phonon::AudioOutput * MediaController::audioOutput() const
{
	return m_AudioOutput;
}

void MediaController::updateStateStatus( Phonon::State newState, Phonon::State oldState)
{
	Q_UNUSED(oldState)
  if (newState == Phonon::ErrorState)
	  if(m_media->errorType()==Phonon::FatalError)
	  {
		  m_media->play();
		  qobject_cast<Phonon::VideoWidget*>(parent())->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
	  }
}

void MediaController::playFile( QString file )
{
	Phonon::MediaSource s(file);
	playSource(s);
}
