//inner-shiv for ie
// http://bit.ly/ishiv | WTFPL License
window.innerShiv=function(){function h(c,e,b){return/^(?:area|br|col|embed|hr|img|input|link|meta|param)$/i.test(b)?c:e+"></"+b+">"}var c,e=document,j,g="abbr article aside audio canvas datalist details figcaption figure footer header hgroup mark meter nav output progress section summary time video".split(" ");return function(d,i){if(!c&&(c=e.createElement("div"),c.innerHTML="<nav></nav>",j=c.childNodes.length!==1)){for(var b=e.createDocumentFragment(),f=g.length;f--;)b.createElement(g[f]);b.appendChild(c)}d=d.replace(/^\s\s*/,"").replace(/\s\s*$/,"").replace(/<script\b[^<]*(?:(?!<\/script>)<[^<]*)*<\/script>/gi,"").replace(/(<([\w:]+)[^>]*?)\/>/g,h);c.innerHTML=(b=d.match(/^<(tbody|tr|td|col|colgroup|thead|tfoot)/i))?"<table>"+d+"</table>":d;b=b?c.getElementsByTagName(b[1])[0].parentNode:c;if(i===!1)return b.childNodes;for(var f=e.createDocumentFragment(),k=b.childNodes.length;k--;)f.appendChild(b.firstChild);return f}}();

(function($){
//var video = '<object width="610" height="371" style="visibility: visible" id="flashcontent-2825_" data="https://www.youtube-nocookie.com/v/MdcotOjqnVI&amp;hl=en&amp;fs=1&amp;showinfo=0&amp;showsearch=0&amp;autoplay=1" type="application/x-shockwave-flash"><param value="always" name="allowscriptaccess" /><param value="true" name="allowfullscreen" /><param value="transparent" name="wmode" /></object><a href= "#introvideo" class="close-link clearfix">Close Video</a>';

var videolink = '<a href = "#introvideo" class="video-link no-follow">Watch the Introductory Video</a><blockquote>?My administration is committed to creating an unprecedented level of openness in government. We will work together to ensure the public trust and establish a system of transparency, public participation and collaboration. Openness will strengthen our democracy and promote efficiency and efffectiveness in government.?</blockquote><div class="attribution">?President Barack Obama</div>';
var video = '<iframe width="610" height="371" src="https://www.youtube-nocookie.com/embed/MdcotOjqnVI?autoplay=1;showinfo=0;showsearch=0;fs=1;wmode=transparent" frameborder="0" allowfullscreen></iframe><a href= "#introvideo" class="close-link clearfix">Close Video</a>'
  $('a.video-link').live("click" , function(event){
  event.preventDefault();
  $('.video-region').animate({opacity: 0, height: 400, queue: false}, 500, function(){
    $('.video-region').empty();
    $('.video-region').animate({opacity: 1, queue: false}, 500);
    $('.video-region').append(video);
    $('#petition-tool-content').animate({opacity: 1, height: 1500, queue: false}, 500);
    });
  });

  $('a.close-link').live("click" , function(event){
    event.preventDefault();
    $('.video-region').animate({opacity: 0, height: 237, queue: false}, 500, function(){
      $('.video-region').empty();
      $('.video-region').append(videolink);
      $('.video-region').animate({opacity: 1, queue: false}, 500);
      $('#petition-tool-content').animate({opacity: 1, height: 1347, queue: false}, 500);
    });
  });
})(jQuery);

//tweets
(function($) {
    // Populate tweets
    var recentTweetParse = function(data) {
      if (data.results && data.results.length > 0) {
        var t = data.results[0],
            tweet = t.text,
            retweetstr = '<span id="twitter-retweet-button"><a target="_blank" class="retweet" href="https://twitter.com/intent/retweet?tweet_id='+t.id_str+'">Retweet</a></span>',
            replystr = '<span id="twitter-reply-button"><a target="_blank" class="reply" href="https://twitter.com/intent/tweet?in_reply_to='+t.id_str+'">Reply</a></span>',
            tweet = tweet.replace(/(https\:\/\/[A-Za-z0-9\/\.\?\=\-]+)/g,'<a href="$1" target="_blank">$1</a>'),
            tweet = tweet.replace(/@([A-Za-z0-9\/_]+)/g,'<a href="https://twitter.com/$1" target="_blank">@$1</a>'),
	    tweet = tweet.replace(/#([A-Za-z0-9\/\.]+)/g,'<a href="https://twitter.com/search?q=$1" target="_blank">#$1</a>'),
            rv = $('<div class="tweet" />')
            .append(
                    $('<div class="twitter-user" />')
                    .html('@Whitehouse')
                    )
            .append(
                  $('<div class="tweet-text" />')
                  .html(tweet)
                  )
            .append(
                  $('<div class="twitter-buttons" />')
                  .html(retweetstr+replystr)
                  ) ;
        return rv;
      }
    };
    var updateLatestTweet = function () {
      //var t_user = 'WeThePeople'
      var t_user = 'Whitehouse';
      var t_space = '#latest-tweet';
      $(t_space)
        .children('.tweet')
        .css({opacity: 0});
      $.ajax({
        url: 'https://search.twitter.com/search.json?q= from:' + t_user,
        dataType: 'jsonp',
        data: null,
        success: function (data) {
          var new_tweet = recentTweetParse(data).css({opacity: 0});
          $(t_space).html(new_tweet);
          new_tweet.animate({opacity: 1}, 1500);
          /**/
        }
      });
    };

    updateLatestTweet();
    setInterval(updateLatestTweet, 3*60*1000); // Update tweet every 3 minutes.
}
)(jQuery);
