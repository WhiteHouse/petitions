/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors
Drupal.behaviors.petitionsresponsive = {
  attach: function(context, settings) {

    $(window).ready(function() {
      // Execute code once the window is ready.
    });

    $(window).load(function() {
      // Execute code once the window is fully loaded.
    });

    $(window).resize(function() {
      // Execute code when the window is resized.
      scrollTopLinkDisplay();
    });

    $(window).scroll(function () {
      // Execute code when the window scrolls.
    });

    $(document).ready(function() {
      // Execute code once the DOM is ready.
      scrollTopLinkDisplay();
      $('.return-top', context).once('returntotop', function(){
        $(this).click(function(event){
          event.preventDefault();
          href = $(this).attr('href');
          $('html,body').animate({scrollTop: $(href).offset().top}, 600);
        });
      });
    });
  }
};

// Add a class to show/hide the "Return to Top" link based on document height.
var scrollTopLinkDisplay = function() {
  var min_desktop_height = 1050;
  var el = $('.return-top');
  if ($(document).height() > min_desktop_height) {
    el.addClass('desktop-visible');
  }
  else {
    el.removeClass('desktop-visible');
  }
}

})(jQuery, Drupal, this, this.document);

