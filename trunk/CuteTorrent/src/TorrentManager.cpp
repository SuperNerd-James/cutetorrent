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
#include "TorrentManager.h"

#include <QApplication>
#include  <QFile>
#include  <QDir>
#include  <QStringList>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include "MetaDataDownloadWaiter.h"
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include "messagebox.h"

using namespace libtorrent;


int load_file(std::string const& filename, std::vector<char>& v, libtorrent::error_code& ec, int limit = 8000000)
{
    ec.clear();
    FILE* f = fopen(filename.c_str(), "rb");
    if (f == NULL)
    {
        ec.assign(errno, boost::system::get_generic_category());
        return -1;
    }

    int r = fseek(f, 0, SEEK_END);
    if (r != 0)
    {
        ec.assign(errno, boost::system::get_generic_category());
        fclose(f);
        return -1;
    }
    long s = ftell(f);
    if (s < 0)
    {
        ec.assign(errno, boost::system::get_generic_category());
        fclose(f);
        return -1;
    }

    if (s > limit)
    {
        fclose(f);
        return -2;
    }

    r = fseek(f, 0, SEEK_SET);
    if (r != 0)
    {
        ec.assign(errno, boost::system::get_generic_category());
        fclose(f);
        return -1;
    }

    v.resize(s);
    if (s == 0)
    {
        fclose(f);
        return 0;
    }

    r = fread(&v[0], 1, v.size(), f);
    if (r < 0)
    {
        ec.assign(errno, boost::system::get_generic_category());
        fclose(f);
        return -1;
    }

    fclose(f);

    if (r != s) return -3;

    return 0;
}

TorrentManager::TorrentManager()
{

    try{
        torrents = TorrentStorrage::getInstance();
        //qDebug() << "QApplicationSettings::getInstance from TorrentManager::TorrentManager";
        torrentSettings = QApplicationSettings::getInstance();

        ses = new session(fingerprint("CuT", VERSION_MAJOR ,VERSION_MINOR,VERSION_REVISION ,VERSION_TAG)
                          , session::start_default_features
                          | session::add_default_plugins
						  , alert::error_notification );
        error_code ec;
        std::vector<char> in;
        if (load_file("CT_DATA/actual.state", in, ec) == 0)
        {
            lazy_entry e;
            if (lazy_bdecode(&in[0], &in[0] + in.size(), e, ec) == 0)
                ses->load_state(e);
        }
        session_settings s_settings=readSettings();
        // local perrs serach
        ses->start_lsd();
        // upnp
        ses->start_upnp();
        ses->start_natpmp();
        ses->add_extension(&libtorrent::create_ut_metadata_plugin);
        ses->add_extension(&libtorrent::create_ut_pex_plugin);
        ses->add_extension(&libtorrent::create_metadata_plugin);
        QString dataDir;
#ifdef Q_WS_MAC
        dataDir="/Library/CuteTorrent/";
#else
        dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
        create_directory(dataDir.toStdString()+"CT_DATA",ec);

        ses->listen_on(std::make_pair(listen_port, listen_port+20)
                       , ec);
        if (ec)
        {
            MyMessageBox::critical(NULL,"ERROR",tr("LISTENING ON PORT %1 FAILED").arg(listen_port));
            return;
        }




        s_settings.use_dht_as_fallback = false;

        ses->add_dht_router(std::make_pair(
                                std::string("router.bittorrent.com"), 6881));
        ses->add_dht_router(std::make_pair(
                                std::string("router.utorrent.com"), 6881));
        ses->add_dht_router(std::make_pair(
                                std::string("router.bitcomet.com"), 6881));
        ses->add_dht_router(std::make_pair(
                                std::string("dht.transmissionbt.com"), 6881));
        in.clear();
        if (load_file((dataDir).toStdString()+"CT_DATA/dht.state", in, ec) == 0)
        {
            entry e;

            e=bdecode(&in[0], &in[0] + in.size());
            ses->start_dht(e);
        }
        else
            ses->start_dht();

        ses->set_settings(s_settings);


    }
    catch(std::exception ex)
    {
        MyMessageBox::warning(0,"Error",QString("Error ocured in TorrentManager::TorrentManager()\n")+ex.what());
    }
    //QTimer::singleShot(10,this,SLOT(initSession()));
}

void TorrentManager::initSession()
{
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    QDir dir(dataDir+"CT_DATA");
    QStringList filter;

    filter <<"*.torrent";

    QStringList torrentFiles=dir.entryList(filter);
    //QMessageBox::warning(NULL,QApplication::applicationDirPath(),QApplication::applicationDirPath());
	QMap<QString,QPair<QString, QString>> save_path_data;
	QString pathResumeFile = dataDir + "CT_DATA/path.resume";
	QFile path_infohashFile(pathResumeFile);
	if (path_infohashFile.open(QFile::ReadOnly))
	{
		QTextStream strm(&path_infohashFile);
		strm.setCodec("UTF-8");
		QString line;
		while (!strm.atEnd())
		{
			line = strm.readLine();
			QStringList parts = line.split("|");
			if (parts.count() > 3)
				continue;
			if (parts.count() < 3)
				save_path_data.insert(parts.at(0), qMakePair(parts.at(1), QString("")));
			else
				save_path_data.insert(parts.at(0), qMakePair(parts.at(1), parts.at(2)));
		}
		path_infohashFile.close();
		QFile::remove(pathResumeFile);
	}
  
	for (QStringList::iterator i = torrentFiles.begin(); i != torrentFiles.end(); ++i)
	{
		error_code ec;
		QString filePath = dir.filePath(*i);
		torrent_info ti(filePath.toStdString(), ec);
		QString infoHash = QString::fromStdString(to_hex(ti.info_hash().to_string()));
		QString savePath = "";
		QString group = "";
		if (!save_path_data.isEmpty() && save_path_data.contains(infoHash))
		{
			QPair<QString, QString> info = save_path_data[infoHash];
			savePath = info.first;
			group = info.second;
		}
		AddTorrent(dir.filePath(*i), savePath, "", ec, QMap<QString, qint8>(), group);
		if (ec)
		{
			MyMessageBox::critical(NULL, tr("Error in TorrentManager::AddTorrent"), QString::fromLocal8Bit(ec.message().c_str()));
		}

	}
    std::deque<alert*> alerts;
    ses->pop_alerts(&alerts);
//    std::string now = time_now_string();
    for (std::deque<alert*>::iterator i = alerts.begin()
         , end(alerts.end()); i != end; ++i)
    {
		delete *i;
    }
    alerts.clear();
    emit initCompleted();
	ses->set_alert_mask(alert::error_notification | alert::storage_notification | alert::status_notification);
}
bool yes(libtorrent::torrent_status const&)
{
    return true;
}
std::vector<torrent_status> TorrentManager::GetTorrents()
{
    std::vector<torrent_status> result;


    ses->get_torrent_status(&result,yes);

    return result;
}
void TorrentManager::handle_alert(alert* a)
{
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    switch(a->type())
    {
    case torrent_finished_alert::alert_type:
    {
        torrent_finished_alert* p = alert_cast<torrent_finished_alert>(a);
        p->handle.set_max_connections(max_connections_per_torrent / 2);

        torrent_handle h = p->handle;
        emit TorrentCompleted(h.name().c_str(),h.save_path().c_str());
        h.save_resume_data();
        break;
    }
    case save_resume_data_alert::alert_type:
    {
        save_resume_data_alert* p = alert_cast<save_resume_data_alert>(a);
        torrent_handle h = p->handle;
        if (p->resume_data)
        {
            std::vector<char> out;
			entry e = *p->resume_data;
			std::string info_hash = to_hex(h.info_hash().to_string());
			Torrent * torrent = torrents->getTorrent(QString::fromStdString(info_hash));
			if (torrent != NULL)
			{
				e["torrent_group"] = torrent->GetGroup().toStdString();
				e["torrent_name"] = h.status(torrent_handle::query_name).name;
			}
			qDebug() << "Saving resume data" << QString::fromStdString(e.to_string());
			bencode(std::back_inserter(out), e);
            save_file( combine_path((dataDir+"CT_DATA").toStdString(), to_hex(h.info_hash().to_string()) + ".resume"), out);
        }
        break;
    }
    case tracker_error_alert::alert_type:
    {
        tracker_error_alert* p=alert_cast<tracker_error_alert>(a);
		std::string message = p->message();
        if (strstr(message.c_str(),"-")==NULL && strstr(message.c_str(),"200")==NULL )
        {
            emit TorrentError("",message.c_str());
        }
        break;
    }
    case storage_moved_alert::alert_type:
    {
        storage_moved_alert *p=alert_cast<storage_moved_alert>(a);
        torrent_handle h = p->handle;
		if (h.is_valid())
		{
			h.save_resume_data();
			h.resume();
		}
        emit TorrentInfo(QString::fromStdString(h.name()),tr("MOVE_STORRAGE_COMPLETED_TO:\n%1").arg(QString::fromStdString(h.save_path())));
        break;
    }
    case fastresume_rejected_alert::alert_type:
    {
        fastresume_rejected_alert *p=alert_cast<fastresume_rejected_alert>(a);

        torrent_handle h = p->handle;
        h.auto_managed(false);
        h.pause();
        emit TorrentError("",p->message().c_str());

        break;
    }
    case metadata_received_alert::alert_type:
    {
        metadata_received_alert* p = alert_cast<metadata_received_alert>(a);
        torrent_handle h = p->handle;
        if (h.is_valid()) {
            try
            {
                torrent_info const& ti = h.get_torrent_info();
                create_torrent ct(ti);
                std::ofstream out(complete(combine_path((dataDir+"CT_DATA").toStdString(),to_hex(ti.info_hash().to_string()) + ".torrent")).c_str(), std::ios_base::binary);
                bencode(std::ostream_iterator<char>(out), ct.generate());
            }
            catch (...)
            {
                qDebug() << "Exception in metadata_received_alert";
            }



        }
        break;
    }
    case portmap_alert::alert_type:
    {
        portmap_alert* alert = alert_cast<portmap_alert>(a);
        emit TorrentInfo("",QString::fromStdString(alert->message()));
    }
    case performance_alert::alert_type:
        /*{
            performance_alert* p=alert_cast<performance_alert>(a);
            session_settings settings = ses->settings();
            switch (p->warning_code)
            {
                case performance_alert::outstanding_disk_buffer_limit_reached:
                {
                    //qDebug() << "max_queued_disk_bytes" << settings.max_queued_disk_bytes;
                    settings.max_queued_disk_bytes+=settings.max_queued_disk_bytes/4;
                    break;
                }
                case performance_alert::outstanding_request_limit_reached:
                {
                    //qDebug() << "max_out_request_queue" << settings.max_out_request_queue;
                    settings.max_out_request_queue+=settings.max_out_request_queue/4;
                    break;
                }
                case performance_alert::upload_limit_too_low:
                {
                    //qDebug() << "upload_rate_limit" << settings.upload_rate_limit;
                    settings.upload_rate_limit+=settings.upload_rate_limit*5/100;
                    break;
                }
                case performance_alert::download_limit_too_low:
                {
                    //qDebug() << "download_rate_limit" << settings.download_rate_limit;
                    settings.download_rate_limit+=settings.download_rate_limit*5/100;
                    break;
                }
                case performance_alert::send_buffer_watermark_too_low:
                {
                    //qDebug() << "send_buffer_watermark" << settings.send_buffer_watermark;
                    settings.send_buffer_watermark+=settings.send_buffer_watermark/4;
                    break;
                }
                case performance_alert::too_many_optimistic_unchoke_slots:
                {
                    //qDebug() << "unchoke_slots_limit" << settings.unchoke_slots_limit;
                    settings.unchoke_slots_limit+= settings.unchoke_slots_limit/4;
                    break;
                }
                case performance_alert::too_high_disk_queue_limit:
                {
                    //qDebug() << "max_queued_disk_bytes" << settings.max_queued_disk_bytes;
                    //qDebug() << "cache_size" << settings.cache_size;
                    if (settings.max_queued_disk_bytes-settings.max_queued_disk_bytes/4 < 16*1024)
                    {
                        settings.max_queued_disk_bytes=settings.max_queued_disk_bytes-settings.max_queued_disk_bytes/4;
                    }
                    else
                    {
                        settings.cache_size+=32;
                    }
                    break;
                }
            }
            ses->set_settings(settings);
        }*/
	case add_torrent_alert::alert_type:
    case listen_succeeded_alert::alert_type:
    case state_changed_alert::alert_type:
    case state_update_alert::alert_type:
    case torrent_added_alert::alert_type:
    case torrent_checked_alert::alert_type:
    case torrent_resumed_alert::alert_type:
    case torrent_paused_alert::alert_type:
    case torrent_removed_alert::alert_type:
    case cache_flushed_alert::alert_type:
    case torrent_deleted_alert::alert_type:
    case external_ip_alert::alert_type:
    case udp_error_alert::alert_type:
        break;
    default:
    {
        QString information = QString::fromStdString(a->message());
		if (information.startsWith("added torrent")) {
			qDebug() << "passed alert" << information << QString::fromStdString(a->what());
		}
        if ((a->category() & alert::error_notification)==alert::error_notification)
        {
            emit TorrentError("",information);
        }
        else
        {


            emit TorrentInfo("",information);
        }
        break;
    }
    }


}
void TorrentManager::PostTorrentUpdate()
{
    std::deque<alert*> alerts;
    ses->pop_alerts(&alerts);

    for (std::deque<alert*>::iterator i = alerts.begin()
         , end(alerts.end()); i != end; ++i)
    {

        TORRENT_TRY
        {
            handle_alert(*i);

        } TORRENT_CATCH(std::exception& e) {
			qDebug() << e.what();
		}
        delete *i;
    }
    alerts.clear();
    ses->post_torrent_updates();
}
bool TorrentManager::AddTorrent(QString path, QString save_path, QString name, error_code& ec, QMap< QString, qint8 >& filePriorities, QString group, bool sequntial)

{

    boost::intrusive_ptr<torrent_info> t;



    t = new torrent_info(path.toUtf8().data(), ec);

    if (ec)
    {
        //	QMessageBox::warning(0,"Adding a torrent",QString::fromStdString(ec.message()));
        return false;
    }



    add_torrent_params p;

     QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    std::string filename = combine_path((dataDir+"CT_DATA").toStdString(), to_hex(t->info_hash().to_string()) + ".resume");
	std::vector<char> buf;
    if (load_file(filename.c_str(), buf, ec) == 0)
    {
       // p.flags |= add_torrent_params::flag_seed_mode;
        p.resume_data = buf;
		entry e = libtorrent::bdecode(buf.begin(),buf.end());
		if (entry* i = e.find_key("torrent_name"))
		{
			name = QString::fromStdString(i->string());
		}
		if (entry* i = e.find_key("torrent_group"))
		{
			group = QString::fromStdString(i->string());
		}
    }
    if (!filePriorities.isEmpty())
    {
        std::vector<boost::uint8_t> filepriorities;
		file_storage storrage = t->files();
		int filesCount = storrage.num_files();
		for (int i = 0; i < filesCount; i++)
        {
			file_entry file = storrage.at(i);
			QString filePath = QDir::toNativeSeparators(file.path.c_str());
			if (filePriorities.contains(filePath))
			{
				filepriorities.push_back(filePriorities[filePath]);
			}
            else
            {
				filepriorities.push_back(1);
            }
        }
		p.file_priorities = filepriorities;
    }
	if (!name.isEmpty())
	{
		p.name = name.toStdString();
		file_storage fs = t->files();
		fs.set_name(p.name);
		t->remap_files(fs);
	}
    p.ti = t;
	
	p.save_path = std::string(save_path.toUtf8().data());
    p.storage_mode = storage_mode_sparse;
    p.flags |= add_torrent_params::flag_paused;
    p.flags |= add_torrent_params::flag_duplicate_is_error;
    p.userdata = (void*)strdup(path.toLatin1().data());
    torrent_handle h=ses->add_torrent(p,ec);
	


    if (ec)
    {
        //	QMessageBox::warning(0,"Error",ec.message().c_str());
        return false;
    }
    Torrent* current=new Torrent(h,group);
    torrents->append(current);
    
    emit AddTorrentGui(current);

    if (sequntial)
        current->seqensialDownload();
    h.set_max_connections(max_connections_per_torrent);

    QFile::copy(path,combine_path((dataDir).toLatin1().data(),combine_path("CT_DATA",(to_hex(t->info_hash().to_string())+".torrent").c_str()).c_str()).c_str());
    if (QFile::exists(combine_path((dataDir+"CT_DATA").toLatin1().data(),((t->name())+".torrent").c_str()).c_str()))
    {
        QFile::remove(combine_path((dataDir+"CT_DATA").toLatin1().data(),((t->name())+".torrent").c_str()).c_str());
    }
	/*
    if (save_path_data.contains(to_hex(h.info_hash().to_string()).c_str()))
    {
        save_path_data[to_hex(h.info_hash().to_string()).c_str()]=qMakePair(save_path,group);
    }
    else
    {
        save_path_data.insert(to_hex(h.info_hash().to_string()).c_str(),qMakePair(save_path,group));
    }
    UpdatePathResumeAndLinks();
	*/
    return true;
}
session_settings TorrentManager::readSettings()
{

    session_settings s_settings=ses->settings();
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    QString baseDir=torrentSettings->valueString("System","BaseDir");
    if (baseDir.isEmpty())
    {
        baseDir=dataDir;
        torrentSettings->setValue("System","BaseDir",baseDir);
    }
    QDir::setCurrent(QDir::toNativeSeparators (baseDir));
    //s_settings.p
    s_settings.half_open_limit = torrentSettings->valueInt("Torrent","half_open_limit",0x7fffffff);
    s_settings.allow_multiple_connections_per_ip = torrentSettings->valueBool("Torrent","allow_multiple_connections_per_ip",true);
    listen_port=torrentSettings->valueInt("Torrent","listen_port",6103);
    s_settings.use_disk_read_ahead = torrentSettings->valueBool("Torrent","use_disk_read_ahead",true);
    s_settings.disable_hash_checks = torrentSettings->valueBool("Torrent","disable_hash_checks",false);
    s_settings.peer_timeout = torrentSettings->valueInt("Torrent","peer_timeout",120);
    s_settings.announce_to_all_tiers = torrentSettings->valueBool("Torrent","announce_to_all_tiers",true);
    s_settings.download_rate_limit = torrentSettings->valueInt("Torrent","download_rate_limit",0);
    s_settings.upload_rate_limit = torrentSettings->valueInt("Torrent","upload_rate_limit",0);
	s_settings.torrent_connect_boost = 15;
    s_settings.unchoke_slots_limit = torrentSettings->valueInt("Torrent","unchoke_slots_limit",8);
    s_settings.urlseed_wait_retry = torrentSettings->valueInt("Torrent","urlseed_wait_retry",30);
    s_settings.listen_queue_size = torrentSettings->valueInt("Torrent","listen_queue_size",30);
    s_settings.mixed_mode_algorithm = session_settings::peer_proportional;
    s_settings.max_peerlist_size = torrentSettings->valueInt("Torrent","max_peerlist_size",4000);
    s_settings.max_paused_peerlist_size = torrentSettings->valueInt("Torrent","max_paused_peerlist_size",4000);
    ipFilterFileName = torrentSettings->valueString("Torrent","ip_filter_filename","");
    FILE* filter = fopen(ipFilterFileName.toLatin1().data(), "r");
    if (filter)
    {
        ip_filter fil;
        unsigned int a,b,c,d,e,f,g,h, flags;
        while (fscanf(filter, "%u.%u.%u.%u - %u.%u.%u.%u %u\n", &a, &b, &c, &d, &e, &f, &g, &h, &flags) == 9)
        {
            address_v4 start((a << 24) + (b << 16) + (c << 8) + d);
            address_v4 last((e << 24) + (f << 16) + (g << 8) + h);
            if (flags <= 127) flags = ip_filter::blocked;
            else flags = 0;
            fil.add_rule(start, last, flags);
        }
        ses->set_ip_filter(fil);
        fclose(filter);
    }
    max_connections_per_torrent = torrentSettings->valueInt("Torrent","max_connections_per_torrent",50);
    useProxy = torrentSettings->valueBool("Torrent","useProxy",false);
    if (useProxy)
    {
        ps.hostname = torrentSettings->valueString("Torrent","proxy_hostname").toUtf8().constData();
        ps.port = torrentSettings->valueInt("Torrent","proxy_port");
        ps.type = (proxy_settings::proxy_type)torrentSettings->valueInt("Torrent","proxy_type");
        ps.username = torrentSettings->valueString("Torrent","proxy_username").toUtf8().constData();
        ps.password = torrentSettings->valueString("Torrent","proxy_password").toUtf8().constData();
        ses->set_proxy(ps);
    }
    s_settings.lock_files = torrentSettings->valueBool("Torrent","lock_files",false);
    s_settings.disk_io_read_mode = torrentSettings->valueInt("Torrent","disk_io_read_mode",0);
    s_settings.disk_io_write_mode = torrentSettings->valueInt("Torrent","disk_io_write_mode",0);
    s_settings.low_prio_disk = torrentSettings->valueBool("Torrent","low_prio_disk",false);
    s_settings.cache_size = torrentSettings->valueInt("Torrent","cache_size",2048);
    s_settings.use_read_cache =  torrentSettings->valueInt("Torrent","use_read_cache",s_settings.cache_size > 0);
    s_settings.cache_buffer_chunk_size = torrentSettings->valueInt("Torrent","cache_buffer_chunk_size",s_settings.cache_size /100);
    s_settings.allowed_fast_set_size = torrentSettings->valueInt("Torrent","allowed_fast_set_size",10);
    s_settings.read_cache_line_size = torrentSettings->valueInt("Torrent","read_cache_line_size",40);
    s_settings.allow_reordered_disk_operations = torrentSettings->valueBool("Torrent","allow_reordered_disk_operations",true);
    s_settings.active_downloads = torrentSettings->valueInt("Torrent","active_downloads",-1);
    s_settings.active_limit = torrentSettings->valueInt("Torrent","active_limit",-1);
    s_settings.active_seeds = torrentSettings->valueInt("Torrent","active_seeds",-1);
    s_settings.choking_algorithm = session_settings::auto_expand_choker;
    s_settings.disk_cache_algorithm = session_settings::avoid_readback;
    s_settings.user_agent = "CuteTorrent "CT_VERSION;
    DTInstallPath = torrentSettings->valueString("DT","DTInstallPath");
    s_settings.announce_double_nat=true;


    return s_settings;
}

libtorrent::pe_settings TorrentManager::readEncSettings()
{
    return ses->get_pe_settings();
}

void TorrentManager::updateEncSettings(const libtorrent::pe_settings& settings) {
    ses->set_pe_settings(settings);
}

void TorrentManager::updateSettings(const libtorrent::session_settings& settings)
{
    ses->set_settings(settings);



}
void TorrentManager::writeSettings()
{
    session_settings s_settings=ses->settings();

    torrentSettings->setValue("Torrent","half_open_limit",s_settings.half_open_limit);
    torrentSettings->setValue("Torrent","allow_multiple_connections_per_ip",s_settings.allow_multiple_connections_per_ip);

    torrentSettings->setValue("Torrent","use_disk_read_ahead",s_settings.use_disk_read_ahead);
    torrentSettings->setValue("Torrent","disable_hash_checks",s_settings.disable_hash_checks);;
    torrentSettings->setValue("Torrent","peer_timeout",s_settings.peer_timeout);
    torrentSettings->setValue("Torrent","announce_to_all_tiers",s_settings.announce_to_all_tiers);
    torrentSettings->setValue("Torrent","download_rate_limit",s_settings.download_rate_limit);
    torrentSettings->setValue("Torrent","upload_rate_limit",s_settings.upload_rate_limit);

    torrentSettings->setValue("Torrent","unchoke_slots_limit",s_settings.unchoke_slots_limit);
    torrentSettings->setValue("Torrent","urlseed_wait_retry",s_settings.urlseed_wait_retry);
    torrentSettings->setValue("Torrent","listen_queue_size",s_settings.listen_queue_size);

    torrentSettings->setValue("Torrent","max_peerlist_size",s_settings.max_peerlist_size);
    torrentSettings->setValue("Torrent","max_paused_peerlist_size",s_settings.max_paused_peerlist_size);
    torrentSettings->setValue("Torrent","max_connections_per_torrent",max_connections_per_torrent);

    torrentSettings->setValue("Torrent","ip_filter_filename",ipFilterFileName);

    torrentSettings->setValue("Torrent","useProxy",useProxy);
    torrentSettings->setValue("Torrent","proxy_hostname",ps.hostname.c_str());
    torrentSettings->setValue("Torrent","proxy_port",ps.port);
    torrentSettings->setValue("Torrent","proxy_type",ps.type);
    torrentSettings->setValue("Torrent","proxy_username",ps.username.c_str());
    torrentSettings->setValue("Torrent","proxy_password",ps.password.c_str());


    torrentSettings->setValue("Torrent","lock_files",s_settings.lock_files);
    torrentSettings->setValue("Torrent","disk_io_read_mode",s_settings.disk_io_read_mode);
    torrentSettings->setValue("Torrent","disk_io_write_mode",s_settings.disk_io_write_mode);
    torrentSettings->setValue("Torrent","low_prio_disk",s_settings.low_prio_disk );
    torrentSettings->setValue("Torrent","cache_size",s_settings.cache_size);
    torrentSettings->setValue("Torrent","use_read_cache",s_settings.use_read_cache);
    torrentSettings->setValue("Torrent","allow_reordered_disk_operations",s_settings.allow_reordered_disk_operations);

    torrentSettings->setValue("Torrent","cache_buffer_chunk_size",s_settings.cache_buffer_chunk_size);
    torrentSettings->setValue("Torrent","allowed_fast_set_size",s_settings.allowed_fast_set_size);
    torrentSettings->setValue("Torrent","read_cache_line_size",s_settings.read_cache_line_size);

    torrentSettings->setValue("Torrent","active_downloads",s_settings.active_downloads);
    torrentSettings->setValue("Torrent","active_limit",s_settings.active_limit);
    torrentSettings->setValue("Torrent","active_seeds",s_settings.active_seeds);

    pe_settings enc_settings = ses->get_pe_settings();
    torrentSettings->setValue("Torrent","in_enc_policy",enc_settings.in_enc_policy);
    torrentSettings->setValue("Torrent","out_enc_policy",enc_settings.out_enc_policy);
    torrentSettings->setValue("Torrent","allowed_enc_level",enc_settings.allowed_enc_level - 1);
    torrentSettings->setValue("Torrent","prefer_rc4",enc_settings.prefer_rc4);
}
void TorrentManager::onClose()
{

    writeSettings();
    int num_outstanding_resume_data = 0;
    std::vector<torrent_status> temp;
    ses->get_torrent_status(&temp, &yes, 0);
    for (std::vector<torrent_status>::iterator i = temp.begin();
         i != temp.end(); ++i)
    {
        torrent_status& st = *i;
        if (!st.handle.is_valid())
        {
            continue;
        }
        if (!st.has_metadata)
        {
            continue;
        }
        if (!st.need_save_resume)
        {
            continue;
        }

        st.handle.save_resume_data();
        ++num_outstanding_resume_data;
        printf("\r%d  ", num_outstanding_resume_data);

    }
    //qDebug() << "waiting for resume data " << num_outstanding_resume_data << "\n";
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    while (num_outstanding_resume_data > 0)
    {
        alert const* a = ses->wait_for_alert(seconds(10));
        if (a == 0) continue;

        std::deque<alert*> alerts;
        ses->pop_alerts(&alerts);

        for (std::deque<alert*>::iterator i = alerts.begin()
             , end(alerts.end()); i != end; ++i)
        {
            std::auto_ptr<alert> a(*i);

            torrent_paused_alert const* tp = alert_cast<torrent_paused_alert>(*i);
            if (tp)
            {
                continue;
            }

            if (alert_cast<save_resume_data_failed_alert>(*i))
            {
                --num_outstanding_resume_data;
                continue;
            }

            save_resume_data_alert const* rd = alert_cast<save_resume_data_alert>(*i);
            if (!rd) continue;
            --num_outstanding_resume_data;


            if (!rd->resume_data) continue;

            torrent_handle h = rd->handle;
            std::vector<char> out;
			libtorrent::entry  e = *rd->resume_data;
			std::string info_hash = to_hex(h.info_hash().to_string());
			Torrent * torrent = torrents->getTorrent(QString::fromStdString(info_hash));
			if (torrent != NULL)
			{
				e["torrent_group"] = torrent->GetGroup().toStdString();
				e["torrent_name"] = h.status(torrent_handle::query_name).name;
			}
			bencode(std::back_inserter(out), e);
			save_file(combine_path((dataDir + "CT_DATA").toStdString(), info_hash + ".resume"), out);
        }
    }


    {
        entry session_state;
        ses->save_state(session_state);

        std::vector<char> out;
        bencode(std::back_inserter(out), session_state);
        save_file((dataDir+"CT_DATA").toStdString()+"/actual.state", out);
    }
    /*UpdatePathResumeAndLinks();*/
    ses->abort();

}
int TorrentManager::save_file(std::string const& filename, std::vector<char>& v)
{
    using namespace libtorrent;

    file f;
    error_code ec;
    if (!f.open(filename, file::write_only, ec)) return -1;
    if (ec) return -1;
    file::iovec_t b = {&v[0], v.size()};
    size_type written = f.writev(0, &b, 1, ec);
    if (written != int(v.size())) return -3;
    if (ec) return -3;
    return 0;
}

/*
void TorrentManager::UpdatePathResumeAndLinks()
{
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    QFile pathDataFile(dataDir+"CT_DATA/path.resume");
    if (pathDataFile.open(QFile::WriteOnly))
    {
        for (QMap<QString,QPair<QString,QString> >::iterator i=save_path_data.begin();i!=save_path_data.end();++i)
        {
            pathDataFile.write((i.key()+"|"+i.value().first+"|"+i.value().second+"\n").toUtf8());
        }
        pathDataFile.close();
    }
    else
    {
        MyMessageBox::critical(0,"Error","CT_DATA/path.resume couldn't be opened");
    }

    
}
*/

TorrentManager::~TorrentManager()
{
    //qDebug() << "TorrentManager::~TorrentManager";
    onClose();
    TorrentStorrage::freeInstance();
    //qDebug() << "free from TorrentManager::~TorrentManager";
    QApplicationSettings::FreeInstance();
}

TorrentManager* TorrentManager::_instance=NULL;
int TorrentManager::_instanceCount=0;
TorrentManager* TorrentManager::getInstance()
{
    //qDebug() << "TorrentManager::getInstance()";
    if (_instance==NULL)
        _instance = new TorrentManager();
    _instanceCount++;
    return _instance;
}
void TorrentManager::freeInstance()
{


    _instanceCount--;
    //qDebug() << "TorrentManager::freeInstance()" << _instanceCount ;
    if (!_instanceCount)
    {
        _instance->~TorrentManager();
        _instance=NULL;
    }
}
opentorrent_info* TorrentManager::GetTorrentInfo(QString filename)
{
    error_code ec;

    torrent_info* ti=new torrent_info(filename.toStdString(), ec);
    if (ec)
    {
        MyMessageBox::warning(NULL,"Warning",QObject::tr("ERROR_OPENING_FILE\n%1").arg(filename+"\n"+QString::fromLocal8Bit(ec.message().c_str())));
        return NULL;
    }

    opentorrent_info* info=new opentorrent_info;
    info->size=ti->total_size();
    info->name=QString::fromUtf8(ti->name().c_str());
    info->describtion = QString::fromUtf8(ti->comment().c_str());
    info->files = ti->files();
    info->base_suffix=StaticHelpers::GetBaseSuffix(info->files);
    return info;
}

openmagnet_info* TorrentManager::GetTorrentInfo(const torrent_handle& handle )
{
    error_code ec;

    torrent_info ti=handle.get_torrent_info();

    openmagnet_info* info=new openmagnet_info;
    info->handle=handle;
    info->size=ti.total_size();
    info->name=QString::fromUtf8(ti.name().c_str());
    info->describtion = QString::fromUtf8(ti.comment().c_str());
    info->files = ti.files();
    info->base_suffix=StaticHelpers::GetBaseSuffix(info->files);
    return info;
}

void TorrentManager::RemoveTorrent(torrent_handle h,bool delFiles)
{

    std::string info_hash=to_hex(h.info_hash().to_string());
    QString infoHash=QString::fromStdString(info_hash);
    emit TorrentRemove(infoHash);

    /*	for (int i=0;i<torrents->count();i++)
    {
        if (torrents->at(i)->GetInfoHash()==infoHash)
        {
            torrents->remove(infoHash);
            break;
        }
    }*/
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    QString resume_path=dataDir+QString::fromStdString(combine_path("CT_DATA",info_hash+".resume"));
    QString torrent_path=dataDir+QString::fromStdString(combine_path("CT_DATA",h.name()+".torrent"));
    QString magnet_path = dataDir+QString::fromStdString(combine_path("CT_DATA",info_hash+".torrent"));

    if (QFile::exists(magnet_path))
        QFile::remove(magnet_path);

    if (QFile::exists(torrent_path))
        QFile::remove(torrent_path);

    if (QFile::exists(resume_path))
        QFile::remove(resume_path);

    /*if (save_path_data.contains(infoHash))
        save_path_data.remove(infoHash);
    UpdatePathResumeAndLinks();*/
    //qDebug() << "before ses->remove_torrent(h); h.is_valid()=" << h.is_valid();
    try
    {
        ses->remove_torrent(h,delFiles ? session::delete_files : 0);
    }
    catch (libtorrent::libtorrent_exception e)
    {
        qDebug() << e.what();
    }
    catch(...)
    {
        qDebug() << "Not a libtorrent exception caught";
    }


    //qDebug() << "after ses->remove_torrent(h)";
}
QString TorrentManager::GetSessionDownloadSpeed()
{
    try
    {
		return StaticHelpers::toKbMbGb(ses->status().download_rate)+"/s";
    }
    catch (...)
    {
        return "";
    }

}
QString TorrentManager::GetSessionUploadSpeed()
{
    try
    {
        return StaticHelpers::toKbMbGb(ses->status().upload_rate)+"/s";
    }
    catch (...)
    {
        return "";
    }

}
QString TorrentManager::GetSessionDownloaded()
{
    try
    {
        return StaticHelpers::toKbMbGb(ses->status().total_download);
    }
    catch (...)
    {
        return "";
    }

}
QString TorrentManager::GetSessionUploaded()
{

    try
    {
        return StaticHelpers::toKbMbGb(ses->status().total_upload);
    }
    catch (...)
    {
        return "";
    }

}
torrent_handle TorrentManager::ProcessMagnetLink(QString link,error_code& ec)
{
    add_torrent_params add_info;
    torrent_handle h;
    parse_magnet_uri(link.toStdString(),add_info,ec);
    if (ec!=0)
    {
        MyMessageBox::critical(0,"Error",QString::fromStdString(ec.message()));
        return h;
    }
    lazy_entry resume_data;
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    std::string filename = (dataDir).toStdString()+combine_path("CT_DATA", to_hex(add_info.info_hash.to_string()) + ".resume");
    std::vector<char> buf;

    if (load_file(filename.c_str(), buf, ec) == 0)
    {
        add_info.resume_data = buf;
      /*  if (save_path_data.contains(to_hex(add_info.info_hash.to_string()).c_str()))
        {
            add_info.save_path = save_path_data[to_hex(add_info.info_hash.to_string()).c_str()].first.toStdString();
        }*/
    }

    add_info.flags |= add_torrent_params::flag_paused;
    add_info.flags |= add_torrent_params::flag_duplicate_is_error;
    h=ses->add_torrent(add_info);
    
    while (!h.has_metadata())
    {
        sleep(1000);
    }
	h.pause();




    return h;

}

void TorrentManager::CancelMagnetLink(QString link)
{
    add_torrent_params add_info;
    error_code ec;
    parse_magnet_uri(link.toStdString(),add_info,ec);
    
    torrent_handle h = ses->find_torrent(add_info.info_hash);
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    QString magnet_path = dataDir+QString::fromStdString(combine_path("CT_DATA",to_hex(h.info_hash().to_string())+".torrent"));

    if (QFile::exists(magnet_path))
        QFile::remove(magnet_path);

    if (h.is_valid())
    {
        ses->remove_torrent(h,session::delete_files);
    }
}

bool TorrentManager::AddMagnet( torrent_handle h,QString SavePath,QString group,QMap<QString,qint8>& filePriorities )
{

    if (!filePriorities.isEmpty())
    {
        std::vector<int> filepriorities;
        file_storage storrage = h.get_torrent_info().files();
        for (file_storage::iterator i=storrage.begin();i!=storrage.end();++i)
        {

            if (filePriorities.contains(QDir::toNativeSeparators(storrage.file_path(*i).c_str())))
                filepriorities.push_back(filePriorities[storrage.file_path(*i).c_str()]);
            else
            {
                filepriorities.push_back(7);
            }
        }
        h.prioritize_files(filepriorities);
        filePriorities.~QMap();

    }
    if (SavePath!=QString::fromStdString(h.save_path()))
    {
        h.move_storage(SavePath.toStdString());
    }
	QString info_hash = QString::fromStdString(to_hex(h.info_hash().to_string()));
   /* if (save_path_data.contains(info_hash))
    {
        group=save_path_data[info_hash].second;
    }*/
    if (h.is_valid())
    {

        emit AddTorrentGui(new Torrent(h,group));
    }
  /*  if (save_path_data.contains(info_hash))
    {
        save_path_data[info_hash]=qMakePair(SavePath,group);
    }
    else
    {
        save_path_data.insert(info_hash,qMakePair(SavePath,group));
    }*/
    Torrent* current = new Torrent(h,group);
    torrents->append(current);
   /* UpdatePathResumeAndLinks();*/
    return true;

}

void TorrentManager::StartAllTorrents()
{
    
        std::vector<torrent_handle> torrent_handels=ses->get_torrents();
        for (int i = 0; i<torrent_handels.size();i++)
        {
			torrent_handle hTor = torrent_handels.at(i);
            if (hTor.is_valid() && hTor.is_paused())
                hTor.resume();
        }
}

void TorrentManager::PauseAllTorrents()
{

   
        std::vector<torrent_handle> torrent_handels=ses->get_torrents();
        for (int i = 0; i<torrent_handels.size();i++)
        {
			torrent_handle tHandle = torrent_handels.at(i);
            if (tHandle.is_valid() && !tHandle.is_paused())
            {
                tHandle.auto_managed(false);
                tHandle.pause();
            }
        }
   
}

QString TorrentManager::GetSessionDHTstate()
{
    return "";
}

void TorrentManager::SetUlLimit( int val )
{
    //qDebug() << "TorrentManager::SetUlLimit" << val;
    ses->set_upload_rate_limit(val);
}

void TorrentManager::SetDlLimit( int val )
{
    //qDebug() << "TorrentManager::SetDlLimit" << val;
    ses->set_download_rate_limit(val);

}

int TorrentManager::GetDownloadLimit()
{
    return ses->download_rate_limit();
}

int TorrentManager::GetUploadLimit()
{
    return ses->upload_rate_limit();
}

Torrent* TorrentManager::GetTorrentByInfoHash(const sha1_hash& hash )
{
    QString infoHash =QString::fromStdString(to_hex(hash.to_string()));

    if (torrents->hasTorrent(infoHash))
        return	(*torrents)[infoHash];
    return NULL;

}
Torrent* TorrentManager::GetTorrentByInfoHash( QString infoHash )
{
    if (torrents->hasTorrent(infoHash))
        return	(*torrents)[infoHash];
    return NULL;
}

void TorrentManager::AddPortMapping(session::protocol_type type, ushort external_port, ushort local_port)
{
	ses->add_port_mapping(type, external_port, local_port);
}

