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
bool Torrent::hasError() const
{
	QString errorString="";
	bool hasErr=false;
	if (cur_torrent.error.length()>0)
	{
		hasErr=true;
	}
	const std::vector<announce_entry> trackers=cur_torrent.handle.trackers();
	for (int i=0;i<trackers.size();i++)
		if (trackers[i].message.length()>0)
		{	
			hasErr=true;
		}
		return hasErr;
}
std::vector<peer_info> Torrent::GetPeerInfo()
{


	std::vector<peer_info> pInfo;

	//	if (!isSeeding())
	{
		//		MessageBoxA(0,"cur_torrent.handle.get_peer_info(pInfo);","cur_torrent.handle.get_peer_info(pInfo);",0);
		if (cur_torrent.handle.is_valid())
			cur_torrent.handle.get_peer_info(pInfo);
		//		MessageBoxA(0,"cur_torrent.handle.get_peer_info(pInfo);1","cur_torrent.handle.get_peer_info(pInfo);1",0);
	}
	return pInfo;
}
std::vector<announce_entry> Torrent::GetTrackerInfo()
{
	return cur_torrent.handle.trackers();
}
QString Torrent::GetErrorMessage() const
{

	QString errorString="";
	if (cur_torrent.error.length()>0)
	{

		errorString.append(QString::fromLocal8Bit(cur_torrent.error.c_str())); 
		errorString.append("\n"); 
	}
	const std::vector<announce_entry> trackers=cur_torrent.handle.trackers();
	for (int i=0;i<trackers.size();i++)
		if (trackers[i].message.length()>0)
		{
			errorString.append(QString::fromLocal8Bit(trackers[i].message.c_str())); 
			errorString.append("\n"); 
		}

		return errorString;
}
bool Torrent::hasMetadata() const
{
	return cur_torrent.has_metadata;
}
bool Torrent::isDownloading() const
{
	return cur_torrent.state>=2 && cur_torrent.state<=3;
}
bool Torrent::isPaused() const
{
	return cur_torrent.handle.is_paused();
}
bool Torrent::isSeeding() const
{
	return cur_torrent.handle.is_seed();
}
int Torrent::GetProgress() const
{


	return cur_torrent.progress_ppm / 10000.f;

}
QString Torrent::GetSuffix()
{
	return base_suffix;
}
QStringList Torrent::GetImageFiles()
{
	return imageFiles;
}
Torrent::Torrent(libtorrent::torrent_status torrentStatus)
{

	mountable=false;
	cur_torrent=torrentStatus;
	file_storage storrgae=cur_torrent.handle.get_torrent_info().files();
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
			imageFiles << QString::fromUtf8(cur_torrent.handle.save_path().c_str())+QString::fromUtf8(storrgae.file_path(*i).c_str());
		}
	}
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
	return QString::number(cur_torrent.progress_ppm / 10000.f ,'f',2)+"%";
}
QString Torrent::GetStatusString() const
{
	QStringList state_str;
	state_str << QString::fromLocal8Bit("Проверка файлов (q)") << QString::fromLocal8Bit("Проверка файлов") << QString::fromLocal8Bit("Загрузка") << QString::fromLocal8Bit("Загрузка") << QString::fromLocal8Bit("Законченно") << QString::fromLocal8Bit("Раздача") << QString::fromLocal8Bit("Подготовка") << QString::fromLocal8Bit("Проверка файлов (r)");

	return state_str.at(cur_torrent.state);

}
QString Torrent::GetHashString() const
{

	return to_hex(cur_torrent.info_hash.to_string()).c_str();
}

QString Torrent::GetName() const
{
	if (cur_torrent.handle.is_valid())
		return QString::fromUtf8(cur_torrent.handle.name().c_str());
	return "";
}
QString Torrent::GetDwonloadSpeed()
{
	return (cur_torrent.download_rate != 0 ) ? StaticHelpers::toKbMbGb(cur_torrent.download_rate)+"\\s" : "";
}
QString Torrent::GetDwonloadSpeed() const
{
	return (cur_torrent.download_rate != 0 ) ? StaticHelpers::toKbMbGb(cur_torrent.download_rate)+"\\s" : "";
}
QString Torrent::GetUploadSpeed()
{
	return (cur_torrent.upload_rate != 0) ? StaticHelpers::toKbMbGb(cur_torrent.upload_rate)+"\\s" : "";
}

QString Torrent::GetUploadSpeed() const
{
	return (cur_torrent.upload_rate != 0) ? StaticHelpers::toKbMbGb(cur_torrent.upload_rate)+"\\s" : "";
}
int Torrent::GetPeerCount()
{
	return cur_torrent.num_peers;
}
int Torrent::GetActivePeerCount()
{
	return cur_torrent.num_seeds;
}
QIcon Torrent::GetMimeTypeIcon() const
{

	return icon;
}
QIcon Torrent::GetMimeTypeIcon()
{

	return icon;
}
void Torrent::updateTorrent(torrent_status data)
{
	ErrorString="";
	if (cur_torrent.is_finished!=data.is_finished)
		emit TorrentCompleted(GetName());
	cur_torrent=data;
	if (hasError())
	{
		emit TorrentError(GetName());
	}



}
void Torrent::announceRehash()
{
	cur_torrent.handle.force_recheck();
}
bool Torrent::isSquential() const
{
	return cur_torrent.handle.is_sequential_download();
}
void Torrent::seqensialDownload()
{
	cur_torrent.handle.set_sequential_download(!cur_torrent.handle.is_sequential_download());
}

void Torrent::RemoveTorrent(TorrentManager *mgr,bool delfiles)
{
	mgr->RemoveTorrent(cur_torrent.handle,delfiles);
}
void Torrent::pause()
{
	cur_torrent.handle.auto_managed(false);
	cur_torrent.handle.pause();
}
void Torrent::resume()
{
	cur_torrent.handle.resume();
}
QString Torrent::GetSavePath()
{
	return QString::fromUtf8(cur_torrent.handle.save_path().c_str())+GetName();
}
QString Torrent::GetTotalUploaded() const
{
	return StaticHelpers::toKbMbGb(cur_torrent.all_time_upload);
}
QString Torrent::GetTotalDownloaded() const
{
	return StaticHelpers::toKbMbGb(cur_torrent.all_time_download);
}
QString Torrent::GetActiveTime()
{
	return StaticHelpers::toTimeString(cur_torrent.active_time);
}

QString Torrent::GetTotalSize() const
{
	return StaticHelpers::toKbMbGb(cur_torrent.handle.get_torrent_info().total_size());
}
QString Torrent::GetSeedString()
{
	return QString::fromLocal8Bit("Подключено %1 из %2").arg(cur_torrent.num_seeds).arg(cur_torrent.list_seeds);
}
QString Torrent::GetPeerString()
{
	return QString::fromLocal8Bit("Подключено %1 из %2").arg(cur_torrent.num_peers).arg(cur_torrent.list_peers);
}

QString Torrent::GetRemainingTime()
{
	QString res;
	if (isSeeding() || isPaused())
	{
		res.append(QChar(8734));
		return res;
	}
	if (cur_torrent.download_rate < 1024*10)
		res.append(QChar(8734));
	else
	{
		int time=(cur_torrent.handle.get_torrent_info().total_size()-cur_torrent.all_time_download) / cur_torrent.download_rate;
		res = StaticHelpers::toTimeString(time);
	}
	return res;
}
