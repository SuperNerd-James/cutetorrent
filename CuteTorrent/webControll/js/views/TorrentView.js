/**
 * Created with IntelliJ IDEA.
 * User: rfedos
 * Date: 7/24/13
 * Time: 1:54 PM
 * To change this template use File | Settings | File Templates.
 */
_.templateSettings = {
    evaluate: /\{\{(.+?)\}\}/g,
    interpolate: /\{\{=(.+?)\}\}/g
};
window.TorrentView = Backbone.View.extend({
    model: new window.Torrent,


    initialize: function () {
        //  _.bindAll(this, this.render, 'close');
        this.listenTo(this.model, 'change', this.render);
        this.model.view = this;
        this.template = $.templates("#torentTmpl");
    },
    updateFiles: function (response,view) {
		
        response.forEach(function (value, index) {
            files = view.model.get('files');
            if (files === undefined) {
                files = new Array();
                files[0] = value;

            } else {
                if (files.filter(function (e) {
                    return e.path == value.path;
                }).length == 0) {
                    files.push(value);
                }

            }
            view.model.set('files', files);
        });
    },

    render: function () {


        if (this.$el.find(".progress-bar").length) {
            var view = this;
            this.$el.find(".progress-bar").width(this.model.get('progress') + '%');
            setTimeout(function () {
                id = view.model.get('id');
                detailedInfo = $('#detailedTorrentInfo' + id);
                shouldShow = detailedInfo.hasClass('in');
                selection = $('#torrentData' + id);
                view.$el.html(view.template.render(view.model.attributes)).fadeIn(700);
                if (shouldShow) {
                    $('#detailedTorrentInfo' + id).addClass("in");
                    $('#torrentData' + id).html(selection.html());
                    $('#torrentData' + id + ' .active:last a').tab('show');
                    $('#torrentData' + id + ' a').click(function (e) {
                        e.preventDefault();
                        $(this).tab('show');
                       
                    });
                } else {
                    $('#torrentData' + id + ' a:first').tab('show');
                    $('#torrentData' + id + ' a').click(function (e) {
                        e.preventDefault();
                        $(this).tab('show');
                       
                    });
                }


            }, 1000);

            //this.$el			
        } else {
            id = this.model.get('id');
            detailedInfo = $('#detailedTorrentInfo' + id);
            shouldShow = detailedInfo.hasClass('in');
            selection = $('#torrentData' + id);
            this.$el.html(this.template.render(this.model.attributes)).fadeIn(700);
            if (shouldShow) {
                $('#detailedTorrentInfo' + id).addClass("in");
                $('#torrentData' + id).html(selection.html());
                $('#torrentData' + id + ' .active:last a').tab('show');
                $('#torrentData' + id + ' a').click(function (e) {
                    e.preventDefault();
                    $(this).tab('show');
                 });
            } else {
                $('#torrentData' + id + ' a:first').tab('show');
                $('#torrentData' + id + ' a').click(function (e) {
                    e.preventDefault();
                    $(this).tab('show');
                });
            }
        }




        return this;

    }




});