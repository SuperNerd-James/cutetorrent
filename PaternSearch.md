# Introduction #

From now you were able to add torrent search resourses(like btdigg.org,mininova.org,thepiratebay etc.)



# Details #

You can add a Search item at Search tab of settings dialog by entering name and search pattern.

Search patern is a string like {some\_url}%1 where %1 will be replaced with  text you enterd in search field.

To add search in torrent client you should add such search pattern client:/search?q=%1

example for btdigg.org:
http://btdigg.org/search?q=%1