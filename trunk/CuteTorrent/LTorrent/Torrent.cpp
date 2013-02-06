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
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QMap>
#include <QFileInfo>
#include <vector>
#include <QDebug>
bool Torrent::hasError() const
{
	try
	{
		QString errorString="";
		bool hasErr=false;
		if (cur_torrent.status().error.length()>0)
		{
			hasErr=true;
		}
		const std::vector<announce_entry> trackers=cur_torrent.trackers();
		for (int i=0;i<trackers.size();i++)
		{
			if (trackers[i].message.length()>0)
			{	
				hasErr=true;
			}
		}
		return hasErr;
	}
	catch (...)
	{
		
	}
	return false;
}
std::vector<peer_info> Torrent::GetPeerInfo()
{
	std::vector<peer_info> pInfo;
	try
	{
		if (cur_torrent.is_valid())
				cur_torrent.get_peer_info(pInfo);
	}
	catch (...)
	{

	}	
	return pInfo;
}
std::vector<announce_entry> Torrent::GetTrackerInfo()
{
	std::vector<announce_entry> trackers;
	try
	{
		trackers=cur_torrent.trackers();
	}
	catch (...)
	{

	}
	return trackers;
}
QString Torrent::GetErrorMessage() const
{

	try
	{
		QString errorString="";
		if (cur_torrent.status().error.length()>0)
		{
			errorString.append(QString::fromLocal8Bit(cur_torrent.status().error.c_str())); 
			errorString.append("\n"); 
		}
		const std::vector<announce_entry> trackers=cur_torrent.trackers();
		for (int i=0;i<trackers.size();i++)
		{
			if (trackers[i].message.length()>0)
			{
				errorString.append(QString::fromLocal8Bit(trackers[i].message.c_str())); 
				errorString.append("\n"); 
			}
		}		
		return errorString;
	}
	catch (...)
	{

	}
	return "";
}
bool Torrent::hasMetadata() const
{
	try
	{
		return cur_torrent.has_metadata();
	}
	catch (...)
	{

	}
	return false;
}
bool Torrent::isDownloading() const
{
	try
	{
		return cur_torrent.status().state>=2 && cur_torrent.status().state<=3;
	}
	catch (...)
	{

	}
	return false;
}
bool Torrent::isPaused() const
{
	try
	{
		return cur_torrent.is_paused();
	}
	catch (...)
	{

	}
	return false;
}
bool Torrent::isSeeding() const
{
	try
{
	if (cur_torrent.status().state==4) return true;
		return cur_torrent.is_seed();
	}	catch (...)
	{

	}
	return false;
}
int Torrent::GetProgress() const
{


	try
	{
		return cur_torrent.status().progress_ppm / 10000.f;
	}
	catch (...)
	{

	}
	return 0;

}
QString Torrent::GetSuffix()
{
	return base_suffix;
}
QStringList& Torrent::GetImageFiles()
{
	
	return imageFiles;
}
Torrent::Torrent(libtorrent::torrent_handle torrentStatus)
{

	mountable=false;
	cur_torrent=torrentStatus;
	file_storage storrgae=cur_torrent.get_torrent_info().files();
	libtorrent::file_storage::iterator bg=storrgae.begin(),
		end=storrgae.end();
	QMap<QString,int> suffixesCount;
	QSet<QString> mauntableTypes;
	mauntableTypes << QString::fromAscii("iso");
	mauntableTypes << QString::fromAscii("mdf");
	mauntableTypes << QString::fromAscii("mds");
	int maxSuffix=0;
	
	for (libtorrent::file_storage::iterator i=bg;i!=end;i++)
	{
		QFileInfo curfile(QString::fromUtf8(storrgae.file_path(*i).c_str()));
		if (mauntableTypes.contains(curfile.suffix()))
		{
			imageFiles << QString::fromUtf8(cur_torrent.save_path().c_str())+QString::fromUtf8(storrgae.file_path(*i).c_str());
		}
	}
	//qDebug()<< "found " << imageFiles.count() << " imagefiles for torrent " << QString(torrentStatus.name().c_str());
	for(libtorrent::file_storage::iterator i=bg;i!=end;i++)
	{
		QFileInfo curfile(QString::fromUtf8(storrgae.file_path(*i).c_str()));
		
		if (curfile.suffix()=="mds")
		{
			base_suffix="mdf";
			break;
		}
		if (curfile.suffix()=="mdf")
		{
			base_suffix="mdf";
			break;
		}
		if (curfile.suffix()=="m2ts")
		{
			base_suffix="m2ts";
			break;
		}
		if (!suffixesCount.contains(curfile.suffix()))
		{
			suffixesCount.insert(curfile.suffix(),1);
			if (suffixesCount[curfile.suffix()] > maxSuffix)
			{
				maxSuffix=suffixesCount[curfile.suffix()];
				base_suffix=curfile.suffix();
			}
		}
		else
		{

			suffixesCount[curfile.suffix()]++;
			if (suffixesCount[curfile.suffix()] > maxSuffix)
			{
				maxSuffix=suffixesCount[curfile.suffix()];
				base_suffix=curfile.suffix();
			}
		}
	}
	if (!base_suffix.isEmpty())
	{

		if (mauntableTypes.contains(base_suffix))
			mountable=true;
		icon=StaticHelpers::guessMimeIcon(base_suffix);
	}
	else
	{
		icon = QIcon(":/icons/my-folder.ico");
	}
}
bool Torrent::isDaemonToolsMountable()
{
	return mountable;
}
QString Torrent::GetProgresString() const
{
	try
	{
		return QString::number(cur_torrent.status().progress_ppm / 10000.f ,'f',2)+"%";
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetStatusString() const
{
	QStringList state_str;
	
	state_str << (QT_TR_NOOP("STATE_FILE_CHEACKING (q)")) 
			  << (QT_TR_NOOP("STATE_FILE_CHEACKING")) 
			  << (QT_TR_NOOP("STATE_DOWNLOADING")) 
			  << (QT_TR_NOOP("STATE_DOWNLOADING")) 
			  << (QT_TR_NOOP("STATE_SEEDING")) 
			  << (QT_TR_NOOP("STATE_SEEDING"))
			  << (QT_TR_NOOP("STATE_PREPARING")) 
			  << (QT_TR_NOOP("STATE_FILE_CHEACKING (r)"));

	try
	{
		return tr(state_str.at(cur_torrent.status().state).toAscii().data()) + (cur_torrent.is_sequential_download() ? (" [S]"):"");
	}
	catch (...)
	{

	}
	return "";

}
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
}

QString Torrent::GetName() const
{
	try
	{
		if (cur_torrent.is_valid())
				return QString::fromUtf8(cur_torrent.name().c_str());
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetDwonloadSpeed()
{
	try
	{
		return (cur_torrent.status().download_rate > 2*1024 ) ? StaticHelpers::toKbMbGb(cur_torrent.status().download_rate)+"\\s" : "";
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetDwonloadSpeed() const
{
	try
	{
		return (cur_torrent.status().download_rate > 2*1024 ) ? StaticHelpers::toKbMbGb(cur_torrent.status().download_rate)+"\\s" : "";
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetUploadSpeed()
{
	try
	{
		return (cur_torrent.status().upload_rate > 2*1024) ? StaticHelpers::toKbMbGb(cur_torrent.status().upload_rate)+"\\s" : "";
	}
	catch (...)
	{

	}
	return "";
}

QString Torrent::GetUploadSpeed() const
{
	try
	{
		return (cur_torrent.status().upload_rate > 2*1024) ? StaticHelpers::toKbMbGb(cur_torrent.status().upload_rate)+"\\s" : "";
	}
	catch (...)
	{

	}
	return "";
}
int Torrent::GetPeerCount()
{
	try
	{
		return cur_torrent.status().num_peers;
	}
	catch (...)
	{

	}
	return 0;
}
int Torrent::GetActivePeerCount()
{
	try
	{
		return cur_torrent.status().num_seeds;
	}
	catch (...)
	{

	}
	return 0;
}
QIcon Torrent::GetMimeTypeIcon() const
{

	return icon;
}
QIcon Torrent::GetMimeTypeIcon()
{

	return icon;
}

void Torrent::announceRehash()
{
	try
	{
		cur_torrent.force_recheck();
	}
	catch (...)
	{

	}
}
bool Torrent::isSquential() const
{
	try
	{
		return cur_torrent.is_sequential_download();
	}
	catch (...)
	{

	}
	return false;
}
void Torrent::seqensialDownload()
{
	try
	{
		cur_torrent.set_sequential_download(!cur_torrent.is_sequential_download());
	}
	catch (...)
	{

	}

}

void Torrent::RemoveTorrent(TorrentManager *mgr,bool delfiles)
{
	mgr->RemoveTorrent(cur_torrent,delfiles);
}
void Torrent::pause()
{
	try
	{
		cur_torrent.auto_managed(false);
		cur_torrent.pause();
	}
	catch (...)
	{

	}
}
void Torrent::resume()
{
	
	try
	{
		cur_torrent.resume();
	}
	catch (...)
	{

	}
}
QString Torrent::GetSavePath()
{
	try
	{
		return QString::fromUtf8(cur_torrent.save_path().c_str())/*+GetName()*/;
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetTotalUploaded() const
{
	try
	{
		return StaticHelpers::toKbMbGb(cur_torrent.status().all_time_upload);
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetTotalDownloaded() const
{
	try
	{
		return StaticHelpers::toKbMbGb(cur_torrent.status().all_time_download);
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetActiveTime()
{
	try
	{
		return StaticHelpers::toTimeString(cur_torrent.status().active_time);
	}
	catch (...)
	{

	}
	return "";
}

QString Torrent::GetTotalSize() const
{
	try
	{
		return StaticHelpers::toKbMbGb(cur_torrent.get_torrent_info().total_size());
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetSeedString()
{
	try
	{
		return QObject::tr("CT_CONNECTED %1 CT_FROM %2").arg(cur_torrent.status().num_seeds).arg(cur_torrent.status().list_seeds);
	}
	catch (...)
	{

	}
	return "";
}
QString Torrent::GetPeerString()
{
	try
	{
		return QObject::tr("CT_CONNECTED %1 CT_FROM %2").arg(cur_torrent.status().num_peers).arg(cur_torrent.status().list_peers);
	}
	catch (...)
	{

	}
	return "";
}

QString Torrent::GetRemainingTime()
{
	QString res;
	try
	{
		if (isSeeding() || isPaused())
			{
				res.append(QChar(8734));
				return res;
			}
			if (cur_torrent.status().download_rate < 1024*10)
				res.append(QChar(8734));
			else
			{
				int time=(cur_torrent.get_torrent_info().total_size()-cur_torrent.status().all_time_download) / cur_torrent.status().download_rate;
				res = StaticHelpers::toTimeString(time);
			}
	}	
	catch (...)
	{

	}
	return res;
}

QList<file_info> Torrent::GetFileDownloadInfo()
{
	QList<file_info> res;
	try
	{
		torrent_info ti=cur_torrent.get_torrent_info();
		file_storage storage=ti.files();
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
		progresses.~vector();
	}
	catch (...)
	{

	}
	return res;
}
void Torrent::SetFilePriority(int index,int prioryty)
{
	try
	{
		std::vector<int> priorities=cur_torrent.file_priorities();
		priorities[index]=prioryty;
		cur_torrent.prioritize_files(priorities);
	}
	catch (...)
	{
		
	}
	
	
}

void Torrent::updateTrackers()
{
	cur_torrent.force_reannounce();
}
