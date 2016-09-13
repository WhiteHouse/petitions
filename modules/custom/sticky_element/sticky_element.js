/**
 * @file
 * Javascript behaviors for the Achievement Page.
 */

(function ($) {

  "use strict";

  var scrollPosition;
  var windowHeight;
  var toolbarHeight = 0; // Store the admin toolbar height (0 if not present).

  Drupal.behaviors.sticky_element = {
    attach: function (context, settings) {
      var stickyElements = [];
      settings.sticky_element.settings.forEach(function(setting){
        if ($(setting[0], context).length) {
          stickyElements.push(new StickyElement(setting, context));
        }
      });

      if (stickyElements.length) {
        $(document).ready(function(){
          Drupal.stickyWindowResizeEvents(stickyElements);
          Drupal.stickyWindowScrollEvents(stickyElements);
        });

        $(window).load(function() {
          Drupal.stickyWindowResizeEvents(stickyElements);
          Drupal.stickyWindowScrollEvents(stickyElements);
        });

        $(window).resize(function(){
          Drupal.stickyWindowResizeEvents(stickyElements);
        });

        $(document).scroll(function() {
          Drupal.stickyWindowScrollEvents(stickyElements);
        });
      }
    }
  };

  /**
   * Recalculate variables related to height and vertical offset.
   */
  Drupal.stickyWindowResizeEvents = function(stickyElements) {
    toolbarHeight = parseFloat($('body').css('padding-top'));
    windowHeight = window.innerHeight;
    stickyElements.forEach(function(stickyElement){
      stickyElement.resize;
    });
  }

  /**
   * Update sticky classes and css.
   */
  Drupal.stickyWindowScrollEvents = function(stickyElements) {
    scrollPosition = $(document).scrollTop();
    stickyElements.forEach(function(stickyElement){
      stickyElement.scroll(scrollPosition, toolbarHeight);
    });
  }

  /**
   * Prototype for a StickyElement object.
   *
   * @param array setting
   *   An indexed array of the following settings:
   *   - Css selector for the target element
   *   - (bool) Attach to viewport bottom or top
   *   - (bool) Initial fixed position
   *   - Css selector for the element triggering the end of the sticky behavior
   *
   * @param context
   *   The context for used to select the target element
   *
   * @constructor
   */
  var StickyElement = function(setting, context) {
    this.settings = {
      selector: setting[0],
      stickyBottom: (setting[1] ? setting[1] == "true" : false),
      stickyInitial: (setting[2] ? setting[2] == "true" : true),
      stickyEndSelector: (setting[3] || false)
    };
    this.element = $(setting[0], context);
    this.endElement = this.settings.stickyEndSelector ? $(this.settings.stickyEndSelector) : false;
    this.offsetTop = this.element.offset().top;
    this.height = parseFloat(this.element.css('height'));
  }

  /**
   * Recalculate properties related to height, width, and vertical offset.
   */
  StickyElement.prototype.resize = function(){
    var self = this;
    self.offsetTop = self.element.offset().top;
    self.height = parseFloat(self.element.css('height'));
  };

  /**
   * Calls the top or bottom scroll function based on the element's settings.
   *
   * @param scrollPosition
   * @param padding
   */
  StickyElement.prototype.scroll = function(scrollPosition, padding){
    var self = this;
    padding = (padding == null) ? 0 : padding;
    if (self.settings.stickyBottom) {
      self.scrollAnchorBottom(scrollPosition, 0)
    }
    else {
      self.scrollAnchorTop(scrollPosition, padding);
    }
  };

  /**
   * Apply sticky classes and css to a top-anchored element.
   *
   * @param int scrollPosition
   *   the calculated page offset
   * @param padding
   *   Additional margin to apply to the anchored edge.
   */
  StickyElement.prototype.scrollAnchorTop = function(scrollPosition, padding) {
    var self = this;
    if (scrollPosition >= self.offsetTop) {
      // Add the sticky selector
      self.element.addClass('sticky');

      // Calculate and apply negative margin if StickyEndSelector is valid.
      if (self.endElement && self.endElement.length == 1) {
        var stickyScrollEnd = self.endElement.offset().top - self.height;
        var margin = stickyScrollEnd - scrollPosition;
        self.element.css('margin-top', Math.min(margin, padding));
      }
    }
    else if (self.settings.stickyInitial) {
      self.element.addClass('sticky');
      self.element.css('margin-top', padding);
    }
    else {
      self.element.removeClass('sticky');
      self.element.css('margin-top', padding);
    }
  };

  /**
   * Apply sticky classes and css to a bottom-anchored element.
   *
   * @param int scrollPosition
   *   the calculated page offset
   * @param padding
   *   Additional margin to apply to the anchored edge.
   */
  StickyElement.prototype.scrollAnchorBottom = function(scrollPosition, padding) {
    var self = this;
    var bottomOffset = self.offsetTop + self.height;

    if (scrollPosition + windowHeight >= bottomOffset) {
      // Add the sticky selector.
      self.element.addClass('sticky-bottom');

      // Calculate and apply negative margin if StickyEndSelector is valid.
      if (self.endElement && self.endElement.length == 1) {
        var stickyScrollEnd = self.endElement.offset().top - windowHeight;
        var margin = scrollPosition - stickyScrollEnd;
        self.element.css('margin-bottom', Math.max(margin, padding));
      }
    }
    else if (self.settings.stickyInitial) {
      self.element.addClass('sticky-bottom');
      self.element.css('margin-bottom', padding);
    }
    else {
      self.element.removeClass('sticky-bottom');
      self.element.css('margin-bottom', padding);
    }
  }

})(jQuery);


