/*
CuteTorrent BitTorrenttClient with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) <year>  <name of author>

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
#include "Torrent.h"
#include <QStringList>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QFileInfo>
#include <vector>
#include <QDebug>

bool Torrent::hasError() const
{
	if (cur_torrent.is_valid()){

 //       QString errorString="";
        bool hasErr=false;
        if (cur_torrent.status(torrent_handle::query_accurate_download_counters).error.length()>0)
        {
            hasErr=true;
        }
       /* const std::vector<announce_entry> trackers=cur_torrent.trackers();
        for (int i=0;i<trackers.size();i++)
        {
            if (trackers[i].message.length()>0)
            {
                hasErr=true;
            }
        }*/
        return hasErr;
    }
    return false;
}

bool Torrent::isActive() const
{
	torrent_status status = cur_torrent.status();
    return (status.download_rate > 2*1024 ||
            status.upload_rate > 2*1024 ||
            status.state==torrent_status::checking_files );
}
std::vector<peer_info> Torrent::GetPeerInfo()
{
    std::vector<peer_info> pInfo;
    if (cur_torrent.is_valid()) {
	    cur_torrent.get_peer_info(pInfo);
    }
    return pInfo;
}
std::vector<announce_entry> Torrent::GetTrackerInfo()
{
    std::vector<announce_entry> trackers;
	if (cur_torrent.is_valid())
        trackers=cur_torrent.trackers();
   
    return trackers;
}
QString Torrent::GetErrorMessage() const
{

    if (cur_torrent.is_valid()){
        QString errorString="";
        if (cur_torrent.status().error.length()>0)
        {
            errorString.append(QString::fromStdString(cur_torrent.status().error));
            errorString.append("\n");
        }
      /*     const std::vector<announce_entry> trackers=cur_torrent.trackers();
     for (int i=0;i<trackers.size();i++)
        {
            if (trackers[i].message.length()>0)
            {
                errorString.append(QString::fromLocal8Bit(trackers[i].message.c_str()));
                errorString.append("\n");
            }
        }*/
        return errorString;
    }
    return "";
}
bool Torrent::hasMetadata() const
{
   if (cur_torrent.is_valid()) {
        return cur_torrent.has_metadata();
    }
   
    return false;
}
bool Torrent::isDownloading() const
{
   if (cur_torrent.is_valid()) {
        return cur_torrent.status().state>=2 && cur_torrent.status().state<=3;
    }
   
    return false;
}
bool Torrent::isPaused() const
{
  if (cur_torrent.is_valid()) {
        return cur_torrent.is_paused();
    }
  
    return false;
}
bool Torrent::isSeeding() const
{
    if (cur_torrent.is_valid()) {
        if (cur_torrent.status().state==4) return true;
        return cur_torrent.is_seed();
    }
    return false;
}
int Torrent::GetProgress() const
{


    if (cur_torrent.is_valid()) {
        return cur_torrent.status().progress_ppm / 10000.f;
    }
    return 0;

}
QString Torrent::GetSuffix()
{
    return base_suffix;
}
QStringList& Torrent::GetImageFiles()
{
    imageFiles.clear();
	if (cur_torrent.is_valid()) {
		file_storage storrgae=cur_torrent.get_torrent_info().files();
		libtorrent::file_storage::iterator bg=storrgae.begin(),
				end=storrgae.end();

		for (libtorrent::file_storage::iterator i=bg;i!=end;++i)
		{
			QFileInfo curfile(QString::fromUtf8(storrgae.file_path(*i).c_str()));
			if (cur_torrent.file_priority(storrgae.file_index(*i)) > 0 && StyleEngene::suffixes[StyleEngene::DISK].contains(curfile.suffix().toLower()) )
			{
				imageFiles << QString::fromUtf8(cur_torrent.save_path().c_str())+QString::fromUtf8(storrgae.file_path(*i).c_str());
			}
		}
	}
    return imageFiles;
}
Torrent::Torrent(libtorrent::torrent_handle torrentStatus,QString group) 
    : QObject(0),mountable(false) , m_hasMedia(false) , cur_torrent(torrentStatus) , size(0)
{
    file_storage storrgae=cur_torrent.get_torrent_info().files();
    libtorrent::file_storage::iterator bg=storrgae.begin(),
            end=storrgae.end();
    this->group=group;
    StyleEngene::getInstance()->guessMimeIcon(base_suffix,type);
    for (libtorrent::file_storage::iterator i=bg;i!=end;++i)
    {
        QFileInfo curfile(QString::fromUtf8(storrgae.file_path(*i).c_str()));
		QString currentSuffix = curfile.suffix().toLower();
        if (cur_torrent.file_priority(storrgae.file_index(*i)) > 0 && StyleEngene::suffixes[StyleEngene::DISK].contains(currentSuffix))
        {
            imageFiles << QString::fromUtf8(cur_torrent.save_path().c_str())+QString::fromUtf8(storrgae.file_path(*i).c_str());
        }
        if (cur_torrent.file_priority(storrgae.file_index(*i)) > 0)
        {
            if (StyleEngene::suffixes[StyleEngene::VIDEO].contains(currentSuffix) || StyleEngene::suffixes[StyleEngene::AUDIO].contains(currentSuffix))
                m_hasMedia=true;
            size+=storrgae.file_size(*i);
        }
    }

    base_suffix=StaticHelpers::GetBaseSuffix(storrgae);
    //qDebug() << base_suffix << GetName();
    if (!base_suffix.isEmpty())
    {

        icon=StyleEngene::getInstance()->guessMimeIcon(base_suffix,type);

    }
    else
    {
        icon = QIcon(":/icons/my-folder.ico");
        type="folder";
    }
}

Torrent::Torrent( const Torrent &other )
{
    cur_torrent = other.cur_torrent;
    file_storage storrgae=cur_torrent.get_torrent_info().files();
    libtorrent::file_storage::iterator bg=storrgae.begin(),
            end=storrgae.end();

    for (libtorrent::file_storage::iterator i=bg;i!=end;++i)
    {
        QFileInfo curfile(QString::fromUtf8(storrgae.file_path(*i).c_str()));
		QString currentSuffix = curfile.suffix().toLower();
        if (cur_torrent.file_priority(storrgae.file_index(*i)) > 0 && StyleEngene::suffixes[StyleEngene::DISK].contains(currentSuffix))
        {
            imageFiles << QString::fromUtf8(cur_torrent.save_path().c_str())+QString::fromUtf8(storrgae.file_path(*i).c_str());
        }
        if (cur_torrent.file_priority(storrgae.file_index(*i)) > 0)
        {
            if (StyleEngene::suffixes[StyleEngene::VIDEO].contains(currentSuffix) || StyleEngene::suffixes[StyleEngene::AUDIO].contains(currentSuffix))
                m_hasMedia=true;
            size+=storrgae.file_size(*i);
        }
    }

    base_suffix=StaticHelpers::GetBaseSuffix(storrgae);
    //qDebug() << base_suffix << GetName();
    if (!base_suffix.isEmpty())
    {

        icon=StyleEngene::getInstance()->guessMimeIcon(base_suffix,type);
    }
    else
    {
        icon = StyleEngene::getInstance()->getIcon("folder");
        type="folder";
    }
}

bool Torrent::isDaemonToolsMountable()
{
    return imageFiles.length() > 0;
}
QString Torrent::GetProgresString() const
{
    if (cur_torrent.is_valid()) {
        return QString::number(cur_torrent.status().progress_ppm / 10000.f ,'f',2)+"%";
    }
    
    return "";
}
QString Torrent::GetStatusString() const
{
    static QStringList state_str = QStringList() << (QT_TR_NOOP("STATE_FILE_CHEACKING (q)"))
              << (QT_TR_NOOP("STATE_FILE_CHEACKING"))
              << (QT_TR_NOOP("STATE_DOWNLOADING"))
              << (QT_TR_NOOP("STATE_DOWNLOADING"))
              << (QT_TR_NOOP("STATE_SEEDING"))
              << (QT_TR_NOOP("STATE_SEEDING"))
              << (QT_TR_NOOP("STATE_PREPARING"))
              << (QT_TR_NOOP("STATE_FILE_CHEACKING (r)"));

	if (cur_torrent.is_valid()) {
        return tr(state_str.at(cur_torrent.status().state).toLatin1().data()) + (cur_torrent.is_sequential_download() ? (" [S]"):"");
    }
    return "";

}/*
QString Torrent::GetHashString() const
{


    try
    {
        return to_hex(cur_torrent.info_hash().to_string()).c_str();
    }
    catch (...)
    {

    }
    return "";
}*/

QString Torrent::GetName() const
{
    
        if (cur_torrent.is_valid())
            return QString::fromUtf8(cur_torrent.name().c_str());
    return "";
}
QString Torrent::GetDwonloadSpeed()
{
    if (cur_torrent.is_valid()) {
        return (cur_torrent.status().download_rate > 2*1024 ) ? StaticHelpers::toKbMbGb(cur_torrent.status().download_rate)+"\\s" : "";
    }
    
    return "";
}
QString Torrent::GetDwonloadSpeed() const
{
 if (cur_torrent.is_valid()) {
        return (cur_torrent.status().download_rate > 2*1024 ) ? StaticHelpers::toKbMbGb(cur_torrent.status().download_rate)+"\\s" : "";
    }
    return "";
}
QString Torrent::GetUploadSpeed()
{
    if (cur_torrent.is_valid()) {
        return (cur_torrent.status().upload_rate > 2*1024) ? StaticHelpers::toKbMbGb(cur_torrent.status().upload_rate)+"\\s" : "";
    }
    
    return "";
}

QString Torrent::GetUploadSpeed() const
{
   if (cur_torrent.is_valid()) {
        return (cur_torrent.status().upload_rate > 2*1024) ? StaticHelpers::toKbMbGb(cur_torrent.status().upload_rate)+"\\s" : "";
    }
   
    return "";
}
int Torrent::GetPeerCount()
{
    if (cur_torrent.is_valid()) {
        return cur_torrent.status().num_peers;
    }
    
    return 0;
}
int Torrent::GetActivePeerCount()
{
    if (cur_torrent.is_valid()) {
        return cur_torrent.status().num_seeds;
    }
   
    return 0;
}
QIcon Torrent::GetMimeTypeIcon() const
{

    return StyleEngene::getInstance()->guessMimeIcon(base_suffix);
}
//-V659
QIcon Torrent::GetMimeTypeIcon()
{

    return StyleEngene::getInstance()->guessMimeIcon(base_suffix,type);
}

void Torrent::announceRehash()
{
    if (cur_torrent.is_valid()) {
        cur_torrent.force_recheck();
    }
   
}
bool Torrent::isSquential() const
{
    if (cur_torrent.is_valid()) {
        return cur_torrent.is_sequential_download();
    }
    
    return false;
}
void Torrent::seqensialDownload()
{
    if (cur_torrent.is_valid()) {
        cur_torrent.set_sequential_download(!cur_torrent.is_sequential_download());
    }
   

}

void Torrent::RemoveTorrent(bool delfiles)
{
    //qDebug() << "Torrent::RemoveTorrent " << cur_torrent.name().c_str();
    TorrentManager::getInstance()->RemoveTorrent(cur_torrent,delfiles);
    TorrentManager::freeInstance();
}
void Torrent::pause()
{
    if (cur_torrent.is_valid()) {

        cur_torrent.auto_managed(false);
        cur_torrent.pause();
        cur_torrent.scrape_tracker();


    }
    
}
void Torrent::resume()
{

    if (cur_torrent.is_valid()) {
        cur_torrent.resume();

    }
    
}
QString Torrent::GetSavePath()
{
    if (cur_torrent.is_valid()) {
        return QDir::toNativeSeparators(QString::fromStdString(cur_torrent.status(torrent_handle::query_save_path).save_path));
    }
    return "";
}
QString Torrent::GetTotalUploaded() const
{
	 if (cur_torrent.is_valid()) {
        return StaticHelpers::toKbMbGb(cur_torrent.status(torrent_handle::query_accurate_download_counters).all_time_upload);
    }
   
    return "";
}
QString Torrent::GetTotalDownloaded() const
{
     if (cur_torrent.is_valid()) {
        return StaticHelpers::toKbMbGb(cur_torrent.status(torrent_handle::query_accurate_download_counters).all_time_download);
    }
    
    return "";
}
QString Torrent::GetActiveTime()
{
     if (cur_torrent.is_valid()) {
        return StaticHelpers::toTimeString(cur_torrent.status(torrent_handle::query_accurate_download_counters).active_time);
    }
   
    return "";
}

QString Torrent::GetTotalSize() const
{
   return StaticHelpers::toKbMbGb(size);
}
QString Torrent::GetSeedString()
{
    if (cur_torrent.is_valid()) {
        torrent_status status = cur_torrent.status(torrent_handle::query_accurate_download_counters);
        return QObject::tr("CT_CONNECTED %1 CT_FROM %2").arg(status.num_seeds).arg(status.list_seeds);
    }
    return "";
}
QString Torrent::GetPeerString()
{
     if (cur_torrent.is_valid()) {
        torrent_status status = cur_torrent.status(torrent_handle::query_accurate_download_counters);
        return QObject::tr("CT_CONNECTED %1 CT_FROM %2").arg(status.num_peers).arg(status.list_peers);
    }
    return "";
}

QString Torrent::GetRemainingTime()
{
    QString res;
    if (cur_torrent.is_valid()) {
        if (isSeeding() || isPaused())
        {
            res.append(QChar(8734));
            return res;
        }
        torrent_status status = cur_torrent.status(torrent_handle::query_accurate_download_counters);
        if (status.download_rate < 1024*10)
            res.append(QChar(8734));
        else
        {
            int time=size*(1.0f-status.progress_ppm/1000000.f) / status.download_rate;
            res = StaticHelpers::toTimeString(time);
        }
    }
    return res;
}

files_info Torrent::GetFileDownloadInfo()
{
    files_info filesInfo;
    filesInfo.infoHash=GetInfoHash();
	if (cur_torrent.is_valid()) {
		filesInfo.storrage= cur_torrent.get_torrent_info().files();
		cur_torrent.file_progress(filesInfo.progresses);
		filesInfo.priorities = cur_torrent.file_priorities();
	}
    return filesInfo;
    /*
        std::vector<float> progresses;
        cur_torrent.file_progress(progresses);
        int counter=0;
        for (file_storage::iterator i=storage.begin();i!=storage.end();i++)
        {
            file_info current;
            current.name=storage.file_path(*i).c_str();
            current.index=storage.file_index(*i);
            current.prioiry = cur_torrent.file_priority(current.index);
            current.progrss =progresses[counter]*100.f;
            current.size = storage.file_size(*i);
            res.append(current);
            counter++;
        }

    }
    catch (...)
    {

    }
    return res;*/
}
void Torrent::SetFilePriority(int index,int prioryty)
{
     if (cur_torrent.is_valid()) {
        cur_torrent.file_priority(index,prioryty);
    }
}

void Torrent::updateTrackers()
{
    if (cur_torrent.is_valid()) {
        cur_torrent.force_reannounce();
    }
}

void Torrent::MoveStorrage( QString path )
{
    if (cur_torrent.is_valid()) {
        cur_torrent.move_storage(path.toStdString());
    }
 }

int Torrent::GetPieceCount()
{
     if (cur_torrent.is_valid()) {
        return cur_torrent.get_torrent_info().num_pieces();
    }
    return 0;
}

QVector<int> Torrent::GetDownloadedPieces()
{
    QVector<int> res;
    if (cur_torrent.is_valid()) {
        int max_num=cur_torrent.get_torrent_info().num_pieces();
        for (int i=0;i<max_num;i++)
        {
            if (cur_torrent.have_piece(i))
                res.append(i);
        }


    }
    return res;

}

QVector<int> Torrent::GetDownloadingPieces()
{
    QVector<int> res;
    if (cur_torrent.is_valid()) {

        std::vector<partial_piece_info> pieces;
        cur_torrent.get_download_queue(pieces);
        for (std::vector<partial_piece_info>::iterator i=pieces.begin();i!=pieces.end();++i)
        {
            if (i->finished+i->writing > 0)
            {
                res.append(i->piece_index);
            }
        }

    }
    return res;
}

QString Torrent::GetDiscribtion()
{
    if (cur_torrent.is_valid()) {
        return QString::fromStdString(cur_torrent.get_torrent_info().comment());
    }
    return "";
}

void Torrent::SetUlLimit( int val )
{
    //qDebug() << "Torrent::SetUlLimit" << val;
    if (cur_torrent.is_valid()) {

        cur_torrent.set_upload_limit(val);
    }
    

}

void Torrent::SetDlLimit( int val )
{
    //qDebug() << "Torrent::SetDlLimit" << val;
    if (cur_torrent.is_valid()) {
        cur_torrent.set_download_limit(val);
    }
}

int Torrent::GetDownloadLimit()
{
    if (cur_torrent.is_valid()) {
        return cur_torrent.download_limit();
    }
    return 0;
}

int Torrent::GetUploadLimit()
{
    if (cur_torrent.is_valid()) {
        return cur_torrent.upload_limit();
    }
    return 0;
}
bool Torrent::hasMediaFiles()
{
    return m_hasMedia;
}

void Torrent::AddPeer( QHostAddress adr,short port )
{
	if (cur_torrent.is_valid()) {
		tcp::endpoint ep(boost::asio::ip::address_v4(adr.toIPv4Address()),port);
		cur_torrent.connect_peer(ep);
	}
}

void Torrent::AddTracker( QString url )
{
	if (cur_torrent.is_valid()) {
		cur_torrent.add_tracker(url.toStdString());
	}
}

void Torrent::stop()
{
	if (cur_torrent.is_valid()) {
		cur_torrent.auto_managed(false);
		cur_torrent.pause(torrent_handle::graceful_pause);
	}
}

bool Torrent::isStoped() const
{
    return false;//m_stoped;
}

bool Torrent::operator<(const Torrent& other) const
{
	return GetName() < other.GetName();
}

bool Torrent::operator<( Torrent* other)
{

    return other->GetName() < (GetName());
}

void Torrent::SuperSeed()
{
    if (cur_torrent.is_valid()) {
        cur_torrent.super_seeding(!cur_torrent.status().super_seeding);
    }
}

bool Torrent::isSuperSeed()
{

    if (cur_torrent.is_valid()) {
        return cur_torrent.status().super_seeding;
    }
    return false;
}

QString Torrent::GetInfoHash()
{
   if (cur_torrent.is_valid()) {
        return QString::fromStdString(to_hex(cur_torrent.info_hash().to_string()));
    }
    return "";
}

QString Torrent::GetType()
{
    return type;
}

int Torrent::GetStatus()
{
	if (cur_torrent.is_valid()) {
		return cur_torrent.status().state;
	}
	return 0;
}

QString Torrent::generateMagnetLink()
{
    return QString::fromStdString(libtorrent::make_magnet_uri(cur_torrent));
}

QString Torrent::GetGroup()
{
    return group;
}

void Torrent::setGroup( QString newGroup )
{
    group = newGroup;
	cur_torrent.save_resume_data();
}


