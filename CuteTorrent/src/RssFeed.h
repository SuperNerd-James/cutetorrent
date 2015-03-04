#ifndef _RSS_ITEM_H_
#define _RSS_ITEM_H_

#include <libtorrent/config.hpp>
#include <libtorrent/rss.hpp>

class RssFeed
{
public:
	RssFeed(libtorrent::feed_handle rssFeed);
	void Update();
	libtorrent::feed_status Status();
	libtorrent::feed_settings GetSettings();
	void SetSettinfs(libtorrent::feed_settings);
private:
	libtorrent::feed_handle m_hRssFeed;
};

#endif // !_RSS_ITEM_H_
