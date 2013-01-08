(function ($) {

  Drupal.behaviors.gatekeeper = {
    attach: function (context, settings) {

      $(document).bind('click', function(e) {
        if( (!$.browser.msie && e.button === 0) || ($.browser.msie) ) {
          $redirect_flag = true;
          var host = window.location.host.toLowerCase();

          $event = e;
          $parent_event = e.target.parentNode;

          var nodename = '';
          $href = '';
          var href_title = '';
          var target = '';

          if (!$parent_event) { return; }

          if( $parent_event.nodeName == 'A' && e.target.nodeName == 'IMG') {
            nodename = 'A';
            $href = $parent_event.href;
            href_title = $parent_event.href;
            target = $parent_event.target;
          }
          else {
            nodename = e.target.nodeName;
            $href = e.target.href;
            href_title = e.target.text;
            target = e.target.target;
          }

          if (target == '_blank') {
            return true;
          }

          if (nodename == 'A' && $("#TB_ajaxContent").html() ) {
            return true;
          }
          if (nodename == 'A') {
            var link_address = $href;

            if ( !link_address.match(host) && !link_address.match('petitions.whitehouse.gov') && !link_address.match('whitehouse.gov') && !link_address.match('wh.gov') ) {
              var prompt_text = '<div id="tb_external">';
              prompt_text += '<h2>You are exiting the White House Web Server</h2>';
              prompt_text += '<p>Thank you for visiting our site.</p>';
              prompt_text += '<div id="exit-now-access-bg"><div id="exit-now-access">';
              prompt_text += '<p>You will now access<br>';
              prompt_text += '<a href="' + $href + '" title="' + href_title + '">' + $href + '</a></p>';
              prompt_text += '</div></div>';
              prompt_text += '<p><strong>We hope your visit was informative and enjoyable.</strong></p>';
              prompt_text += '<p>To comment on this site, <a href="https://whitehouse.gov/contact">send feedback to the web development team.</a></p></div>';

              e.preventDefault();
              $.colorbox({html:prompt_text, width:"484px", height:"275px"});
              setTimeout(function() {
                window.location = link_address;
              }, 5000);
            }

            else {
              return true;
            }
          }
        }
      });
    }
  };

})(jQuery);
