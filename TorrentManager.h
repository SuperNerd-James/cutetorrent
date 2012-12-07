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
#ifndef TOR_MANAGER_H
#define TOR_MANAGER_H
#include "versionInfo.h"

#include <iterator>
#include <vector>
#include <QString>
#include <QStringList>
#include <QMap>

#define BOOST_ASIO_SEPARATE_COMPILATION
#include "libtorrent/config.hpp"
#include <QSettings>
#ifdef _MSC_VER
#pragma warning(push, 1)
#endif

#include <boost/bind.hpp>
#include <boost/unordered_set.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "libtorrent/extensions/metadata_transfer.hpp"
#include "libtorrent/extensions/ut_metadata.hpp"
#include "libtorrent/extensions/ut_pex.hpp"
#include "libtorrent/extensions/smart_ban.hpp"

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/identify_client.hpp"
#include "libtorrent/alert_types.hpp"
#include "libtorrent/ip_filter.hpp"
#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/bitfield.hpp"
#include "libtorrent/file.hpp"
#include "libtorrent/peer_info.hpp"
#include "libtorrent/socket_io.hpp" // print_address
#include "libtorrent/time.hpp"
#include "libtorrent/create_torrent.hpp"
#include "libtorrent/file_pool.hpp"
using boost::bind;

#ifdef _WIN32

#if defined(_MSC_VER)
#	define for if (false) {} else for
#endif

#include <windows.h>
#include <conio.h>


#else

#include <stdlib.h>
#include <stdio.h>

#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>

#endif

struct opentorrent_info
{
	QString name,describtion,base_suffix;
	libtorrent::file_storage files;
	libtorrent::size_type size;
};
using namespace libtorrent;

#include "QApplicationSettings.h"
#include "Torrent.h"
class QTorrentDisplayModel;
class Torrent;

enum torrent_filter
{
	active=0,stopped=1,compled=2,seeding=3

};



struct torrent_creation_info
{
	std::string path,filter,describtion;
	std::vector<std::string> web_seeds;
	std::vector<std::string> trackers;
	quint64 pice_size;
	bool _private, startSeed, saveFileOrder;


};

class TorrentManager : public QObject
{
	Q_OBJECT
signals:
	void AddTorrentGui(Torrent*);
	void TorrentError(const QString&,const QString&);
	void TorrentCompleted(const QString&);
protected:
	TorrentManager();
	~TorrentManager();
	static TorrentManager* _instance;
	static int _instanceCount;
private:
	void handle_alert(alert*);
	void writeSettings();
	QMap<QString,QString> save_path_data;
	libtorrent::session* ses;
	QApplicationSettings* torrentSettings;
	int save_file(std::string const& filename, std::vector<char>& v);
	
//settingsData TODO implement a container
	QString DTInstallPath;
	int max_connections_per_torrent;
	int listen_port;
	QString ipFilterFileName;
	bool useProxy;
	libtorrent::proxy_settings ps;
//settingsData end
	void onClose();
public:
	libtorrent::session_settings readSettings();
	void updateSettings(libtorrent::session_settings settings);
	QString GetSessionDownloadSpeed();
	QString GetSessionUploadSpeed();
	QString GetSessionDownloaded();
	QString GetSessionUploaded();
	void initSession();
	static TorrentManager* getInstance();
	static void freeInstance();
	std::vector<torrent_status> GetTorrents();
	opentorrent_info* GetTorrentInfo(QString filename);
	void RemoveTorrent(QString InfoHash);
	bool AddMagnet(QString link);
	bool AddTorrent(QString path, QString save_path,QMap<QString,int> filepriorities = QMap<QString,int>());
	void PostTorrentUpdate();
	void RemoveTorrent(torrent_handle h,bool dellfiles=false);
protected:
};


#endif