(function ($) {
  Drupal.behaviors.popularPetitions = {
    attach: function (context) {
      $('.view-display-id-popular_petitions', context).once('popular-petitions', function () {
        // Refresh the cached popular petitions.
        popular_petition_ajax_refresh();

        // Setup interval refresh for popular petitions.
        if (Drupal.settings.popularPetitions.ajax_delay) {
          setInterval(function () {
            popular_petition_ajax_refresh();
          }, Drupal.settings.popularPetitions.ajax_delay * 1000);
        }
      });
    }
  };

  function popular_petition_ajax_refresh() {
    $.ajax({
      url: '/views-pager-history/petitions_listing/popular_petitions/0',
      method: 'GET',
      cache: true,
      success: function(data) {
        $('.view-display-id-popular_petitions').html(data.content);
      }
    });
  }

}(jQuery));
