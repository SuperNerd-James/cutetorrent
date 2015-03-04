
#include "RssFeed.h"

RssFeed::RssFeed(libtorrent::feed_handle rssFeed)
{
	m_hRssFeed = rssFeed;
}

void RssFeed::Update()
{
	m_hRssFeed.update_feed();
}

libtorrent::feed_status RssFeed::Status()
{
	return m_hRssFeed.get_feed_status();
}

libtorrent::feed_settings RssFeed::GetSettings()
{
	return m_hRssFeed.settings();
}

void RssFeed::SetSettinfs(libtorrent::feed_settings settings)
{
	m_hRssFeed.set_settings(settings);
}

