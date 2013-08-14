window.TorrentCollection = Backbone.Collection.extend({
    model: Torrent,
    url:'/torrent/api',
    reset:true,
	search : function(letters){
		if(letters == "") return this;
		return _(this.filter(function(data) {
		  	return data.get("torrentName").toLowerCase().indexOf(letters.toLowerCase()) >= 0;
		}));
	}
});