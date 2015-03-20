# Introduction #

Torrent tracker with suport of no\_peer\_id and compact extentions.


# Details #

To use torrent tracker you shoukd add tracker url to torrent's tracker list.

Tracker url is constructed according to this template:

http://[IP-Address or domian]: [[port of tracker](.md)]/announce

[[port of tracker](.md)] - can be changed in settings. default port is 6996.

This port is forvarded with UPnP. If UPnP is switched off for some reasons on your routing devices than you shold manualy forvard this port.