# Introduction #
DAEMON Tools was originally a successor of Generic SafeDisc emulator and incorporated all of its features. The program claims to be able to defeat most copy protection schemes such as SafeDisc and SecuROM. It is currently compatible with Windows XP, Windows Vista, Windows 7 and Windows 8. DAEMON Tools has a special mode for proper operation of copies of discs with advanced protection (SafeDisc, SecuRom and LaserLock, CDCOPS, StarForce and Protect CD), which are used on some discs with games.

# How to know command line parametrs for DaemonTools? #
First of all we should know command line parametrs to Daemon Tools for mounting images. To find out them we should go to folder where Daemon Tools were installed. Find DTLite.exe or DTAget.exe and launch it with parametr -help. Than we will see this window:
![http://s10.postimg.org/o3y4mrpu1/screenshot_5.png](http://s10.postimg.org/o3y4mrpu1/screenshot_5.png)

As we see to mount image we should launch DTLite.exe with parametrs  -mount` <type>,<n>,<path>`
type - type of drive. Avaliable values: "dt" and "scsi".
n - divece count
path - path to disk image file

I decided to told about this beacuse developers of Daemon Tools can change this parametrs.

# CuteTorrent setup #

1. Open settings of CuteTorrent and go to Daemon Tools Tab
![http://s10.postimg.org/5cw7clv9l/screenshot_6.png](http://s10.postimg.org/5cw7clv9l/screenshot_6.png)

Press Browse and chose path to DTLite.exe or DTAget.exe