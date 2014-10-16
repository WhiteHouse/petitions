/**
 * @file
 * JavaScript for wh_core.
 */

(function ($) {

  Drupal.behaviors.wh_core_origin_populate = {
    attach: function (context) {
      start = location.search.indexOf('destination=');
      end = location.search.indexOf('&', start);
      newVal = 'petitions';
      if (start != -1) {
        // Add 12 to compensate for destination= part of the string.
        start += 12;
        if (end == -1) {
          newVal = location.search.substr(start);
        }
        else {
          newVal = location.search.substr(start, end - 1);
        }
      }
      $('input[name="field_origin[und][0][value]"]').val(newVal);
    }
  };

  /**
   * Display 'thank you' or error message after filling out
   * user registration form.
   */
  Drupal.behaviors.wh_user_registration_thanks = {
    attach: function(context) {
      $('#userreg-thanks').css('display' , 'none');
      $('.congreet_status').css('display' , 'none');
      if (window.location.href.match(/thank\-you/)) {
        $('.messages').css('display' , 'none');
      }
      var message;
      var thanks;
      var congreet;
      thanks = $('#userreg-thanks').html();
      congreet = $('.congreet_status').html();
      congreetmsg = '<div class ="congreet_status">' + congreet + '</div>'
      thanksMsg = '<div id = "userreg-thanks">' + thanks + '</div>';
      message = $('.messages').html();
      if(congreet != null){$('.user-message').append(congreetmsg)};
      if(message != null){$('.user-message').append(message)};
      if(thanks != null) { $('.user-message').append(thanksMsg); }
    }
  }

})(jQuery);
