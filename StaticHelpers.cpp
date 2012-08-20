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
#include "StaticHelpers.h"
#include <QDebug>
QString StaticHelpers::toKbMbGb(libtorrent::size_type size)
{
	float val=size;
	char* Suffix[] = { " B\0", " KB\0", " MB\0", " GB\0", " TB\0" };
    int i=0;
    float dblSByte=val;
	if (size > 1024)
	for (i ; (libtorrent::size_type)(val / 1024) > 0; i++, val /= 1024)
		dblSByte = val / 1024.f;
	QString str=QString::number(dblSByte,'f',i==0? 0 : 2);
	str.append(Suffix[i]);
	return str;
}
QIcon StaticHelpers::guessMimeIcon(QString suffix)
{
	enum { DISK=0, DOCUMENT=1, PICTURE=2, VIDEO=3, ARCHIVE=4, AUDIO=5, APP=6, TYPE_COUNT=7 };
	static QIcon fallback;
	static QIcon fileIcons[TYPE_COUNT];
	static QSet<QString> suffixes[TYPE_COUNT];
	
	if( fileIcons[0].isNull( ) )
	{
	
		fallback = QIcon(QString::fromUtf8(":/icons/my-folder.ico"));
	
	    suffixes[DISK] << QString::fromAscii("iso");
		suffixes[DISK] << QString::fromAscii("mdf");
		suffixes[DISK] << QString::fromAscii("mds");
	    fileIcons[DISK]= QIcon( ":/icons/my-iso.ico");
	
	    const char * doc_types[] = {
	            "abw", "csv", "doc", "dvi", "htm", "html", "ini", "log", "odp",
	            "ods", "odt", "pdf", "ppt", "ps",  "rtf", "tex", "txt", "xml" };
	    for( int i=0, n=sizeof(doc_types)/sizeof(doc_types[0]); i<n; ++i )
			suffixes[DOCUMENT] << QString::fromAscii(doc_types[i] );
		fileIcons[DOCUMENT] = QIcon( ":/icons/my-doc.ico" );
	
	    const char * pic_types[] = {
	            "bmp", "gif", "jpg", "jpeg", "pcx", "png", "psd", "ras", "tga", "tiff" };
	    for( int i=0, n=sizeof(pic_types)/sizeof(pic_types[0]); i<n; ++i )
			suffixes[PICTURE] << QString::fromAscii(pic_types[i]);
	    fileIcons[PICTURE]  = QIcon( ":/icons/my-picture.ico" );
	
	    const char * vid_types[] = {
	            "3gp", "asf", "avi", "mov", "mpeg", "mpg", "mp4", "mkv", "mov",
	            "ogm", "ogv", "qt", "rm", "wmv" , "m2ts" };
	    for( int i=0, n=sizeof(vid_types)/sizeof(vid_types[0]); i<n; ++i )
		{
			suffixes[VIDEO] << QString::fromAscii(vid_types[i]);
		}
        fileIcons[VIDEO] = QIcon( ":/icons/my-movie.ico") ;

	    const char * arc_types[] = {
	            "7z", "ace", "bz2", "cbz", "gz", "gzip", "lzma", "rar", "sft", "tar", "zip" };
	    for( int i=0, n=sizeof(arc_types)/sizeof(arc_types[0]); i<n; ++i )
			suffixes[VIDEO] << QString::fromAscii(arc_types[i]);
        fileIcons[ARCHIVE]  = QIcon( ":/icons/my-archive.ico" );

        const char * aud_types[] = {
	            "aac", "ac3", "aiff", "ape", "au", "flac", "m3u", "m4a", "mid", "midi", "mp2",
	            "mp3", "mpc", "nsf", "oga", "ogg", "ra", "ram", "shn", "voc", "wav", "wma" };
	    for( int i=0, n=sizeof(aud_types)/sizeof(aud_types[0]); i<n; ++i )
			suffixes[AUDIO] << QString::fromAscii(aud_types[i]);
	    fileIcons[AUDIO] = QIcon( ":/icons/my-audio.ico" );
	
	    const char * exe_types[] = { "bat", "cmd", "com", "exe" };
	    for( int i=0, n=sizeof(exe_types)/sizeof(exe_types[0]); i<n; ++i )
			suffixes[APP] << QString::fromAscii(exe_types[i]);
	    fileIcons[APP] = QIcon::fromTheme( "application-x-executable", fallback );
	}
	
	for( int i=0; i<TYPE_COUNT; ++i )
		if( suffixes[i].contains(suffix))
			return fileIcons[i];
	
	return fallback;
}

void StaticHelpers::dellDir(QString dirName)
{
	try
	{
	
	
	bool result = true;
	QDir dir(dirName);
	//qDebug() << "removing dirrectory" << dirName;
	if (dir.exists(dirName)) {
		//qDebug() << "dir exists " << dirName;
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
			if (info.isDir()) {
				//qDebug() << "removing dir " << info.absoluteFilePath();
				 dellDir(info.absoluteFilePath());
			}
			else {
				//qDebug() << "removing file " << info.absoluteFilePath();
				 QFile::remove(info.absoluteFilePath());
			}

			
			}
		}
		//qDebug() << "removing prrent dirrectory " << dirName;
		dir.rmdir(dirName);
	
	}
	catch (...)
	{
		//qDebug() << " exception caught int deldir";
	}
	
	
}
QString StaticHelpers::filePriorityToString(int priority)
{
	static char* priority_str[] = {"Не загружать","Низкий","Средний","Высокий"};
	if (!priority)
		return QString::fromLocal8Bit(priority_str[0]);
	if (priority < 3)
		return QString::fromLocal8Bit(priority_str[1]);
	if (priority >=3 && priority < 6)
		return QString::fromLocal8Bit(priority_str[2]);
	if (priority >=6)
		return QString::fromLocal8Bit(priority_str[3]);
	return "";
}

QString StaticHelpers::toTimeString( int seconds )
{
	int min=0,hour=0,day=0,Week=0;
	Week = seconds/(7*24*60*60);
	seconds -=Week*(7*24*60*60);
	day = seconds / (24*60*60);
	seconds -=day*(24*60*60);
	hour=seconds / (60*60);
	seconds -=hour*60*60;
	min=seconds/60;
	seconds-=min*60;
	QString result,tmp;

	if (Week>0)
	{
		result.append(QString::fromLocal8Bit("%1 Недель ").arg(QString::number(Week)));
	}
	if (day>0)
	{
		result.append(tmp.sprintf("%02d Дней",day));
	}
	if (hour>=0)
	{
		result.append(tmp.sprintf("%02d:",hour));
	}
	if (min>=0)
	{
		result.append(tmp.sprintf("%02d:",min));
	}
	result.append(tmp.sprintf("%02d",seconds));
	return result;
}
