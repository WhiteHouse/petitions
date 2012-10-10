(function ($) {
Drupal.behaviors.loginlogout = {
  attach: function(context) {
    // determine which of the View My Petitions, login/logout, etc links should be visible
   var cookies = {};
   var authenticated = 0;
   if (document.cookie && document.cookie != '') {
     var split = document.cookie.split(';');
     for (var i = 0; i < split.length; i++) {
       var cookie = split[i].split('=');
       if (jQuery.trim(cookie[0]) == 'authenticated' && jQuery.trim(cookie[1]) == '1') {
         authenticated = 1;
         break;
       }
     }
   }

   $('ul.user-state-links li a').each(function(index, value){
     var _url = $(this).attr('href');

     if ((_url.indexOf("register") > 0) || (_url.indexOf("login") > 0)) {
       $(this).addClass('anonymous');
     }
     else if ((_url.indexOf("logout") > 0) || (_url.indexOf("dashboard") > 0) ) {
       $(this).addClass('authenticated');
     }

   });

   // hide irrelevant links based on authenticated status
   if (authenticated) {
     $('a.anonymous').hide();
   }
   else {
     $('a.authenticated').hide();
   }


  }
}
})(jQuery);
