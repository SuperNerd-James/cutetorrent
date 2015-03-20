# Introduction #

Web interface was builden with help of JS(JavaScript) frameworks (jsrender, backbone) so to wark with it you should enable executing of JS in your browser


# Features #

  * Asynckroniuos interface(no page reloading).
  * Paginted list of torrents.
  * Access vai IP wihite list.
  * Pausing,resuming,adding torrents.
  * Access to settings of torrent client.
# Some explanation of usage #
## Peforming Actions ##
To make some action on torrent you should toggle it. To do it you should click on it. After clicking detailed information about torrent will be displayed.
## IP fileter ##
On settings window of torrent client you could see tab named Remote Controll
On it you could enable remote controll start webInterface on specifick port.
And setup IP filter for access to Remote Web UI.

Possible IP filters discribed below:

  * IP\_FILTER = COMMAND IP\_ADDRESS\_MASK | IP\_ADDRES\_RANGE
  * COMMAND = allow | deny
  * IP\_ADDRES\_MASK = NUMBER < 256 | `*` . NUMBER < 256 |`*` . NUMBER < 256 | `*` . NUMBER < 256 | `*`
  * IP\_ADDRES\_RANGE = IP\_ADDRES - IP\_ADDRES
  * IP\_ADDRES = NUMBER <256 . NUMBER <256 . NUMBER <256 . NUMBER <256
> ### Examples: ###
    * allow 192.168.1.`*` (equals to allow 192.168.1.0-192.168.1.255 )
    * allow 10.10.109.0-10.10.109.128
    * deny 192.168.1.`*`
    * deny 10.10.109.0-10.10.109.128
# Screenshot #
![http://s23.postimg.org/wgxza4fhn/2013_08_15_181411.png](http://s23.postimg.org/wgxza4fhn/2013_08_15_181411.png)