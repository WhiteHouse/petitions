(function ($) {

  Drupal.behaviors.wh_core_origin_populate = {
    attach: function (context) {
      start = location.search.indexOf('destination=');
      end = location.search.indexOf('&', start);
      newVal = 'petitions';
      if (start != -1) {
        start += 12; // Add 12 to compensate for destination= part of the string.
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

})(jQuery);
;
