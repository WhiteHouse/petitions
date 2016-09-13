(function ($) {
  Drupal.behaviors.petition = {
    attach: function (context, settings) {
      $('.block-signature-count').once('signatures-progress-bar-refresh', function(){
        context = $(this);
        petition_ajax_refresh();

        setInterval(function(){
          petition_ajax_refresh()
        }, settings.petition.ajax_delay * 1000);
      });

      function petition_ajax_refresh() {
        $.ajax({
          url: settings.petition.api_base + '/petitions/' + settings.petition.petition_id + '.json',
          method: 'GET',
          cache: false,
          success: function(data) {
            var response = data.results[0];
            var width = 1;
            if (response.signatureCount < response.signatureThreshold) {
              var calc = (Math.round((response.signatureCount / response.signatureThreshold * 100) + "e+1") + "e-1");
              width = Math.max(calc, 1);
            }
            else {
              width = 100;
            }

            $('.signatures-needed', context).text(response.signaturesNeeded.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ","));
            $('.signature-meter > span', context).width(width + '%');
            $('.signatures-number', context).text(response.signatureCount.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ","));
          }
        });

      }
    }
  };
}(jQuery));
