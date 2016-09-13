(function ($) {
  Drupal.behaviors.createPetition = {
    attach: function (context) {

      // Send the Step 1 function to Analytics when people land on the page.
      $('.page-petition-create', context).once('petition-create', function () {
        ga('send', 'pageview', '/petition/create/step1');
      });

      $('#petition-create-formwrapper').once('form-navigation', function () {
        $('.input-button-go-back').on('mousedown', function () {
          currentStep = $('form[action="/petition/create"]').attr('class').slice(-1);
          sendAnalyticsEvent('previous', currentStep);

        });
        $('.input-button-edit').on('mousedown', function () {
          currentStep = $('form[action="/petition/create"]').attr('class').slice(-1);
          sendAnalyticsEvent('previous', currentStep);

        });

        $('.input-button-next-step').on('mousedown', function () {
          currentStep = $('form[action="/petition/create"]').attr('class').slice(-1);
          sendAnalyticsEvent('next', currentStep);
        });

        $('.input-button-publish').on('mousedown', function() {
          ga('send', 'event', 'Petition Create', 'publish button click', 'Publish Petition');
        });
      });

      $('form.petition-create-form-step-6').once('form-thankyou', function() {
        ga('send', 'pageview', '/petition/create/complete');
      });

      var char_limit = 120;
      var textarea_dom_id = $('.form-item-petition-title textarea');
      var petition_help_dom_id = $('.petition-title-help');
      var character_count_dom_id = $('.petition-title-help .character-count');

      textarea_dom_id.keyup(function() {
        var char_limit = 120;
        var character_count_dom_id = $('.petition-title-help .character-count');
        var petition_help_dom_id = $('.petition-title-help');

        character_count_dom_id.html(char_limit - $(this).val().length);
        if($(this).val().length > char_limit) {
          $(this).val(($(this).val().substring(0, char_limit)));
          character_count_dom_id.html($(this).val().length);
        }
        if($(this).val().length == char_limit) {
          petition_help_dom_id.addClass('char-limit-reached');
        }
        else {
          petition_help_dom_id.removeClass('char-limit-reached');
        }
      });

      if(textarea_dom_id.val() != undefined && textarea_dom_id.val().length > 0) {
        character_count_dom_id.html(char_limit - textarea_dom_id.val().length);
        if(textarea_dom_id.val().length == char_limit) {
          petition_help_dom_id.addClass('char-limit-reached');
        }
        else {
          petition_help_dom_id.removeClass('char-limit-reached');
        }
      }

      char_limit = 800;
      textarea_dom_id = $('.form-item-petition-description textarea');
      petition_help_dom_id = $('.petition-description-help');
      character_count_dom_id = $('.petition-description-help .character-count');

      textarea_dom_id.keyup(function() {
        var char_limit = 800;
        var character_count_dom_id = $('.petition-description-help .character-count');
        var petition_help_dom_id = $('.petition-description-help');

        character_count_dom_id.html(char_limit - $(this).val().length);
        if($(this).val().length > char_limit) {
          $(this).val(($(this).val().substring(0, char_limit)));
          character_count_dom_id.html($(this).val().length);

        }

        if($(this).val().length == char_limit) {
          petition_help_dom_id.addClass('char-limit-reached');
        }
        else {
          petition_help_dom_id.removeClass('char-limit-reached');
        }
      });

      if(textarea_dom_id.val() != undefined && textarea_dom_id.val().length > 0) {
        character_count_dom_id.html(char_limit - textarea_dom_id.val().length);
        if(textarea_dom_id.val().length == char_limit) {
          petition_help_dom_id.addClass('char-limit-reached');
        }
        else {
          petition_help_dom_id.removeClass('char-limit-reached');
        }
      }
    }
  }

  function sendAnalyticsEvent(direction, currentStep) {
    targetStep = (direction == 'next') ? parseInt(currentStep) + 1 : parseInt(currentStep) - 1;
    targetPath = '/petition/create/step' + targetStep;
    eventAction = direction + ' button click';
    eventLabel = 'Step ' + currentStep + ' -> Step ' + targetStep;
    ga('send', 'event', 'Petition Create', eventAction, eventLabel);
    ga('send', 'pageview', targetPath);
  }

  Drupal.ajax.prototype.commands.petitionCreateScroll =  function(ajax, response, status) {
    if ($("#petition-create-formwrapper").length) {
      var top = jQuery("#petition-create-formwrapper").offset().top;
      $("html, body").animate({scrollTop: top}, "slow")
    }
  }
})(jQuery);
