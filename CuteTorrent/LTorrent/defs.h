#ifndef _DEFS_H
#define _DEFS_H


#define BOOST_ASIO_SEPARATE_COMPILATION
#include "libtorrent/config.hpp"
#include "libtorrent/file_storage.hpp"
#include "libtorrent/torrent_handle.hpp"
using namespace libtorrent;
struct opentorrent_info
{
	QString name,describtion,base_suffix;
	libtorrent::file_storage files;
	libtorrent::size_type size;
};
struct openmagnet_info
{
	QString name,describtion,base_suffix,link;
	libtorrent::torrent_handle handle;
	libtorrent::file_storage files;
	libtorrent::size_type size;
};


enum FilterType{
	ACTIVE,
	NOT_ACTIVE,
	SEEDING,
	DWONLOADING,
	COMPLETED,
	NONE
};

struct torrent_creation_info
{
	std::string path,filter,describtion;
	std::vector<std::string> web_seeds;
	std::vector<std::string> trackers;
	quint64 pice_size;
	bool _private, startSeed, saveFileOrder;


};

struct files_info
{
	QString infoHash;
    file_storage storrage;
    std::vector<float> progresses;
    std::vector<int> priorities;
};

#endif _DEFS_H
