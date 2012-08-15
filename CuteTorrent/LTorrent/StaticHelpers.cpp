#include "StaticHelpers.h"
#include <QDebug>
QString StaticHelpers::toKbMbGb(libtorrent::size_type size)
{
	float val=size;
	char* Suffix[] = { " B\0", " KB\0", " MB\0", " GB\0", " TB\0" };
    int i;
    float dblSByte=0;
	for (i = 0; (libtorrent::size_type)(val / 1024) > 0; i++, val /= 1024)
		dblSByte = val / 1024.f;
	QString str=QString::number(dblSByte,'f',2);
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

void StaticHelpers::dellDir(QString path)
{
	QDir torrentDir(path);
	qDebug() << "removing dir " << path;
	//First delete any files in the current directory
	QFileInfoList files = torrentDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
	QFileInfoList dirs = torrentDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
	for(int file = 0; file < files.count(); file++)
	{
		qDebug() << "removing file " << files.at(file).filePath();
		torrentDir.remove(files.at(file).filePath());
	}

	
	
	qDebug() << "Now recursively delete any child directories";
	qDebug() << dirs.count();
	for(int dir = 0; dir < dirs.count(); dir++)
	{
		qDebug() << "subdirs not empty now try to remove one";
		qDebug() << "recursive call remove dir " << dirs.at(dir).absoluteFilePath();
		StaticHelpers::dellDir(dirs.at(dir).absoluteFilePath());
	}

	//Finally, remove empty parent directory
	qDebug() << "remove empty parent directory " << path;
	QString prevPath = torrentDir.path();
	torrentDir.cdUp();
	torrentDir.rmdir(path);
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
	QString result;
	if (Week>0)
	{
		result.append(QString("%1 Недель ").arg(QString::number(Week)));
	}
	if (day>0)
	{
		result.append(QString("%1 Дней ").arg(QString::number(day)));
	}
	if (hour>=0)
	{
		result.append(QString("%1:").arg(QString::number(hour)));
	}
	if (min>=0)
	{
		result.append(QString("%1:").arg(QString::number(min)));
	}
	result.append(QString::number(seconds));
	return result;
}
