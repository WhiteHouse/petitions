/* Right Rail open/close */
(function ($) {
Drupal.behaviors.accordian = {
  attach: function(context) {
  $(".toggle-container").hide();
  $("h2.trigger:first").addClass('top-trigger active');
  $("h2.trigger:last").addClass('bottom-trigger');
  $(".toggle-container:first").show();
  $('h2.trigger').unbind();
  $("h2.trigger:not(h2.trigger:last)").click(function() {
    $(this).toggleClass("active").next().slideToggle("fast", function() {
      if ($(this).parents('#qa-content').size() > 0) {
        if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }
      }
    });
    return false;
  });
  $("h2.trigger:last").click(function(){
    if($(this).hasClass("bottom-trigger")) {
      $(this).removeClass("bottom-trigger");
    }
    else {
      $(this).addClass("bottom-trigger");
    }
    $(this).toggleClass("active").next().slideToggle("fast", function() {
      if ($(this).parents('#qa-content').size() > 0) {
        if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }
      }
    });
    return false;
  });
  $("#right-rail h2.trigger a").click(function() { return false; });
  }
}
Drupal.behaviors.listHover = {
  attach: function(context) {
  $(".petition-list .find-out a, .petition-list .view a").hover(
    function() {
      $(this).parents('.entry').find('.title a').css('background-color','#fff');
    },
    function() {
      $(this).parents('.entry').find('.title a').css('background-color','#f6f6f6');
    }
  );
  $(".petition-list .title a").hover(
    function() {
      $(this).parents('.entry').find('.title a').css('background-color','#fff');
    },
    function() {
      $(this).parents('.entry').find('.title a').css('background-color','#f6f6f6');
    }
  );
  $(".petition-list .response-entry .action-bar").hover(
    function() {
      $(this).parents('.response-entry').find('.title a').css('background-color','#fff');
    },
    function() {
      $(this).parents('.response-entry').find('.title a').css('background-color','#f6f6f6');
    }
  );
  $(".petition-list .response-entry .title a").hover(
    function() {
      $(this).parents('.response-entry').find('.title a').css('background-color','#fff');
    },
    function() {
      $(this).parents('.response-entry').find('.title a').css('background-color','#f6f6f6');
    }
  );
  }
}
})(jQuery);

/* Attach the functionality to open and close the issues and search filter boxes */
(function ($) {
Drupal.behaviors.issuesFilter = {
  attach: function(context) {
    $(".filter-list .filter-by-issue").unbind();
    $(".filter-list .filter-by-issue").bind('click',
      function () {
        $('#search-drop').hide();
        $('#issues-filter-drop').toggle();
        return false;
      }
    );
    $("#issues-filter-drop .close-button").click(
      function () {
        $('#issues-filter-drop').hide();
      }
    );
  }
}
Drupal.behaviors.searchFilter = {
  attach: function(context) {
    $(".filter-list .search").unbind();
    $(".filter-list .search").click(
      function () {
        $('#search-drop').toggle();
        $('#issues-filter-drop').hide();
        return false;
      }
    );
    $("#search-drop .close-button").click(
      function () {
        $('#search-drop').hide();
      }
    );
  }
}
})(jQuery);

/* Issues and Search filter submit handlers */
(function ($) {
Drupal.behaviors.issuesSubmit = {
  attach: function(context) {
    $("#filter-list #issues-filter-drop #form-item-issues-filter").unbind();
    $('#filter-list #issues-filter-drop #form-item-issues-filter').submit(function() {
      var issues = retrieveIssues();
      var cols = retrieveCols();
      var sort = retrieveSort();
      if (sort == 'trending') {
        sort = 'all';
        $('#filter-list #sort-trending').removeClass('active');
      }

      // Construct URL
      var begin = '/petitions/more/'+sort+'/';
      var type = 'petitions';
      if($(this).hasClass('response-issue-form')) {
        var begin = '/responses/more/featured/';
        var type = 'responses';
      }
      var url = begin + "0/"+cols+"/"+issues+"/%20/";
        var params = {};
        $.getJSON(url, params, function(data) {
          $('#form-item-search-drop input').val('');
          $('#petition-bars').remove();
          $('.loading-more-petitions-bar').addClass('display-none');
          $('.show-more-petitions-bar').removeClass('display-none');
          if(issues != '0') {
            $('#filter-list #sort-all').removeClass('active');
          }
          else {
            $('#filter-list #sort-all').addClass('active');
          }

          $('#issues-filter-drop').hide();
          if(type == 'responses') {
            $(".petition-list .responses").html(data.markup);
            Drupal.attachBehaviors('.petition-list .show-more-responses-bar')
          }
          else {
            $(".petition-list .petitions").html(data.markup).show();
            Drupal.attachBehaviors('.petition-list .show-more-petitions-bar');
          }
          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }

          if (typeof wh_petition_page_update_links == 'function') {
            wh_petition_page_update_links();
          }
        });
        return false;
    });
  }
}
Drupal.behaviors.searchSubmit = {
  attach: function(context) {
    $('#filter-list #search-drop #form-item-search-drop').unbind();
    $('#filter-list #search-drop #form-item-search-drop').submit(function() {
      var search = retrieveSearch();
      var cols = retrieveCols();

      // Construct URL
      var begin = '/petitions/more/all/';
      var type = 'petitions';
      if($(this).hasClass('response-search-form')) {
        var begin = '/responses/more/featured/';
        var type = 'responses';
      }
      else {
        $('#filter-list #sort-trending').removeClass('active');
        $('#filter-list #sort-popular').removeClass('active');
      }
      var url = begin + "0/"+cols+"/0/"+search+"/";
        var params = {};
        $.getJSON(url, params, function(data) {
          $('#form-item-issues-filter input:checked').attr('checked', false);
          $('#petition-bars').remove();
          $('.loading-more-petitions-bar').addClass('display-none');
          $('.show-more-petitions-bar').removeClass('display-none');

          if(search != '%20') {
            $('#filter-list #sort-all').removeClass('active');
          }
          else {
            $('#filter-list #sort-all').addClass('active');
          }

          $('#search-drop').hide();
          if(type == 'responses') {
            $(".petition-list .responses").html(data.markup);
            Drupal.attachBehaviors('.petition-list .show-more-responses-bar')
          }
          else {
            $(".petition-list .petitions").html(data.markup).show();
            Drupal.attachBehaviors('.petition-list .show-more-petitions-bar');
          }
          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }

          if (typeof wh_petition_page_update_links == 'function') {
            wh_petition_page_update_links();
          }
        });
        return false;
    });
  }
}
Drupal.behaviors.oneColToggle = {
  attach: function(context) {
    $('.filter-list li.view-one-column').unbind();
    $('.filter-list li.view-one-column').click(function() {
      var issues = retrieveIssues();
      var search = retrieveSearch();
      var sort = retrieveSort();

      // Construct URL
      var type = 'petitions';
      if ($(this).parents('.response-filter').size() > 0) {
        var type = 'responses';
        var url = '/responses/more/'+sort+"/1/1/"+issues+"/"+search+"/";
      }
      else {
        var url = '/petitions/more/'+sort+"/1/1/"+issues+"/"+search+"/";
      }
        var params = {};
        $.getJSON(url, params, function(data) {
          $('#petition-bars').remove();
          if (type == 'responses') {
            $(".petition-list .responses").html(data.markup).show();
            Drupal.attachBehaviors('.petition-list .show-more-responses-bar');
          }
          else {
            $(".petition-list .petitions").html(data.markup).show();
            Drupal.attachBehaviors('.petition-list .show-more-petitions-bar');
          }
          $('.filter-list li.view-one-column a').addClass('active');
          $('.filter-list li.view-two-column a').removeClass('active');
          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }
          if (typeof wh_petition_page_update_links == 'function') {
            wh_petition_page_update_links();
          }
        });
        return false;
    });
  }
}
Drupal.behaviors.twoColToggle = {
  attach: function(context) {
    $('.filter-list li.view-two-column').unbind();
    $('.filter-list li.view-two-column').click(function() {
      var issues = retrieveIssues();
      var search = retrieveSearch();
      var sort = retrieveSort();

      // Construct URL
      var type = 'petitions';
      if ($(this).parents('.response-filter').size() > 0) {
        var type = 'responses';
        var url = '/responses/more/'+sort+"/1/2/"+issues+"/"+search+"/";
      }
      else {
        var url = '/petitions/more/'+sort+"/1/2/"+issues+"/"+search+"/";
      }
        var params = {};
        $.getJSON(url, params, function(data) {
          $('#petition-bars').remove();
          if (type == 'responses') {
            $(".petition-list .responses").html(data.markup).show();
            Drupal.attachBehaviors('.petition-list .show-more-responses-bar');
          }
          else {
            $(".petition-list .petitions").html(data.markup).show();
            Drupal.attachBehaviors('.petition-list .show-more-petitions-bar');
          }
          $('.filter-list li.view-two-column a').addClass('active');
          $('.filter-list li.view-one-column a').removeClass('active');
          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }
          if (typeof wh_petition_page_update_links == 'function') {
            wh_petition_page_update_links();
          }
        });
        return false;
    });
  }
}
})(jQuery);

/* Retrieve more petitions and/or responses */
(function ($) {
Drupal.behaviors.morePetitions = {
  attach: function(context) {
    $('.petition-list .show-more-petitions-bar').unbind();
    $(".petition-list .show-more-petitions-bar").bind('click',
      function () {
        $('.show-more-petitions-bar').addClass('display-none');
        $('.loading-more-petitions-bar').removeClass('display-none');

        var petition_sort = retrieveSort();
        var petition_cols = retrieveCols();
        var petition_issues = retrieveIssues();
        var petition_search = retrieveSearch();
        var petition_page = parseInt($('#page-num').html());

        var url = "/petitions/more/"+petition_sort+"/"+(petition_page + 1)+"/"+petition_cols+"/"+petition_issues+"/"+petition_search+"/";
        var params = {};
        $.getJSON(url, params, function(data) {
          $('#petition-bars').remove();
          $('.loading-more-petitions-bar').addClass('display-none');
          $('.show-more-petitions-bar').removeClass('display-none');
          $(".petition-list .petitions").append(data.markup).show();

          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }

          Drupal.attachBehaviors('.petition-list .show-more-petitions-bar');
          if (typeof wh_petition_page_update_links == 'function') {
            wh_petition_page_update_links();
          }
        });

        return false;
      }
    );
  }
}
Drupal.behaviors.moreResponses = {
  attach: function(context) {
    $('.petition-list .show-more-responses-bar').unbind();
    $(".petition-list .show-more-responses-bar").bind('click',
      function () {
        $('.show-more-responses-bar').addClass('display-none');
        $('.loading-more-petitions-bar').removeClass('display-none');

        var response_sort = retrieveSort();
        var response_cols = retrieveCols();
        var response_issues = retrieveIssues();
        var response_search = retrieveSearch();
        var response_page = parseInt($('#page-num').html());

        var url = "/responses/more/"+response_sort+"/"+(response_page + 1)+"/"+response_cols+"/"+response_issues+"/"+response_search+"/";
        var params = {};
        $.getJSON(url, params, function(data) {
          $('#petition-bars').remove();
          $('.loading-more-petitions-bar').addClass('display-none');
          $('.show-more-responses-bar').removeClass('display-none');
          $(".petition-list .responses").append(data.markup).show();

          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }

          Drupal.attachBehaviors('.petition-list .show-more-responses-bar');
          if (typeof wh_petition_page_update_links == 'function') {
            wh_petition_page_update_links();
          }
        });

        return false;
      }
    );
  }
}
})(jQuery);

// Retrieve more signatures for a petition and activate the comment hover
(function ($) {
Drupal.behaviors.moreSignatures = {
  attach: function(context) {
    signature_page = $('#signature-list').data('signature_page');
    signature_page = signature_page || 1;
    $("#signature-list .load-next").unbind();
    $("#signature-list .load-next").click(
      function () {
        var petition_id = $(this).attr('rel');
        var last_id = $('#last-signature-id').html();
        var previously_found_creator = $('#found-creator').html();

        var url = "/signatures/more/"+petition_id+"/"+(signature_page + 1)+"/"+last_id+"/"+previously_found_creator;
        var params = {};
        $.getJSON(url, params, function(data) {
          signature_page++;
          $('#signature-list').data('signature_page', signature_page);
          $(".load-next").remove();
          $("#last-signature-id").remove();
          $("#found-creator").remove();
          $("#signature-list").append(data.markup);
          var newsignatures = $(data.markup).filter('.entry').length;
          var current = parseInt($('#signature-list .count').html());
          var newcount = current + newsignatures;
          $('#signature-list .count').html(newcount);

          if (typeof wh_petition_adjustHeight == 'function') {
            wh_petition_adjustHeight();
          }

          Drupal.attachBehaviors('#signatures');
        });
        return false;
      }
    );
  }
}

var railpos = '';
Drupal.behaviors.railScroll = {
  attach: function(context) {
    if($('#petition-inner .right-rail').length > 0) {
      if (railpos == '') {
        railpos = $('#petition-inner .right-rail').offset();
      }
      $(window).unbind('resize');
      $(window).resize(function(railpos) {
        var pos = $('#petition-inner .right-rail').offset();
        if(pos != null) {
          railScroll(pos);
        }
      });
      $(window).unbind('scroll');
      $(window).scroll(function() {
        railpos = $('#petition-inner .right-rail').offset();
        railScroll(railpos);
      });
    }
  }
}

function railScroll(pos) {
  var windowScrollTop = $(window).scrollTop();
  var petitionHeight = $('#petition-inner .main-content').height();
  var sidebarHeight = $('#petition-inner .right-rail .container').height();
  if(pos != null) {
    if (windowScrollTop >= pos.top && petitionHeight >= (windowScrollTop + sidebarHeight)) {
      $('#petition-inner').css('position', 'relative');
      $('#petition-inner .right-rail .container').removeClass('right-rail-fixed-bottom');
      $('#petition-inner .right-rail .container').addClass('right-rail-fixed');
      $('#petition-inner .right-rail .container').css({top: ''});
      $('#button-back-to-top').removeClass('hide');
    }
    else if (windowScrollTop >= pos.top && petitionHeight < (windowScrollTop + sidebarHeight)) {
      $('#petition-inner').css('position', 'relative');
      $('#petition-inner .right-rail .container').removeClass('right-rail-fixed');
      $('#petition-inner .right-rail .container').addClass('right-rail-fixed-bottom');
      $('#petition-inner .right-rail .container').css({top: (petitionHeight - sidebarHeight)});
      $('#button-back-to-top').removeClass('hide');
    }
    else if (windowScrollTop <= pos.top) {
      $('#petition-inner').css('position', 'static');
      $('#petition-inner .right-rail .container').removeClass('right-rail-fixed');
      $('#petition-inner .right-rail .container').removeClass('right-rail-fixed-bottom');
      $('#petition-inner .right-rail .container').css({top: ''});
      $('#button-back-to-top').addClass('hide');
    }
  }
}

Drupal.behaviors.back_to_top = {
  attach: function(context) {
   $('#button-back-to-top').unbind();
   $('#button-back-to-top').click(function(e) {
     e.preventDefault();
     var pos = $("#petition-inner").position().top;
     pos = pos + 124;
     $("html,body").animate({scrollTop:pos}, "slow");
   });
  }
}

Drupal.behaviors.moreSignaturesAuto = {
  attach: function(context) {
    if($('#signature-list .load-next').length > 0) {
      var pos = $('#signature-list').offset();
      var include_rail = 0;
      if($('#petition-inner .right-rail').length > 0) {
        if (railpos == '') {
          railpos = $('#petition-inner .right-rail').offset();
        }
        include_rail = 1;
      }
      $(window).unbind('resize');
      $(window).resize(function(railpos) {
        var pos = $('#petition-inner .right-rail').offset();
        if(pos != null) {
          $('#petition-inner .right-rail .container').css('left', pos.left + 'px');
          railScroll(pos);
        }
      });
      $(window).unbind('scroll');
      $(window).scroll(function() {
        autoSignatures();
        if (include_rail) {
          railpos = $('#petition-inner .right-rail').offset();
          railScroll(railpos);
        }
      });
    }
  }
}

function autoSignatures() {
  var pos = $('#signature-list').offset();
  if (pos == null) {
    return;
  }
  var height = $('#signature-list').height();
  var toppos = pos.top + height - 117;
  if ($(window).scrollTop() > Math.floor(toppos - $(window).height())) {
    $(window).unbind('scroll');
    $('#signature-list .load-next').html('Retrieving more signatures...');
    var petition_id = $('#signature-list .load-next').attr('rel');
    signature_page = $('#signature-list').data('signature_page');
    signature_page = signature_page || 1;
    var last_id = $('#last-signature-id').html();
    var previously_found_creator = $('#found-creator').html();

    var url = "/signatures/more/"+petition_id+"/"+(signature_page + 1)+"/"+last_id+"/"+previously_found_creator;
    var params = {};
    $.getJSON(url, params, function(data) {
      signature_page++;
      $('#signature-list').data('signature_page', signature_page);
      $(".load-next").remove();
      $("#last-signature-id").remove();
      $("#found-creator").remove();
      $("#signature-list").append(data.markup);
      var newsignatures = $(data.markup).filter('.entry').length;
      var current = parseInt($('#signature-list .count').html());
      var newcount = current + newsignatures;
      $('#signature-list .count').html(newcount);

      if (typeof wh_petition_adjustHeight == 'function') {
         wh_petition_adjustHeight();
      }

      Drupal.attachBehaviors('#signatures');
    });
  }
}
Drupal.behaviors.commentHover = {
  attach: function(context) {
    $("#signature-list .read-comment").hover(
      function () {
        $(this).parent().find(".comment-popup").show();
        $(this).parent().next('.entry').css('position','static');
      },
      function () {
        $(this).parent().find(".comment-popup").hide();
        $(this).parent().next('.entry').css('position','relative');
      }
    );
  }
}
})(jQuery);

/* Attach the functionality to open and close the issues and search filter boxes */
(function ($) {
Drupal.behaviors.reportModal = {
  attach: function(context) {
    $("#report-inappropriate-link").unbind();
    $("#report-inappropriate-link").click(
      function () {
        $('#report-petition-overlay').show();
        $('.report-petition').append('<div id="mask"></div>');
        var maskHeight = $(document).height() + 300;
        var maskWidth = $(window).width();
        var pos = $('.report-petition').position();
        var left_margin = (maskWidth - 976) / 2;
        var left = pos.left + left_margin;
        var top = pos.top + 269;
        $('#mask').css({'width':maskWidth,'height':maskHeight,'top':'-'+top+'px','left':'-'+left+'px'});
        $('#mask').show();
        return false;
      }
    );
    $("#report-petition-overlay .close-button").click(
      function () {
        $('#mask').hide();
        $('#report-petition-overlay').hide();
      }
    );
  }
}
Drupal.behaviors.deleteModal = {
  attach: function(context) {
    $(".delete-petition").unbind();
    $(".delete-petition").each(function() {
      var id = $(this).attr('rel');
      $('#delete-petition-link-'+id).click(
      function () {
        var id = $(this).attr('rel');
        $('#delete-petition-'+id).show();
        $('.delete-insert-'+id).append('<div id="mask"></div>');
        var maskHeight = $(document).height();
        var maskWidth = $(window).width();
        var pos = $('.delete-insert-'+id).position();
        var left_margin = (maskWidth - 976) / 2;
        var left = pos.left + left_margin;
        var top = pos.top + 269;
        $('#mask').css({'width':maskWidth,'height':maskHeight,'top':'-'+top+'px','left':'-'+left+'px'});
        $('#mask').show();
        return false;
      }
    );
  });
  $(".delete-petition-overlay a.close-button").unbind();
    $(".delete-petition-overlay a.close-button").click(
      function () {
        var id = $(this).attr('rel');
        $('#mask').hide();
        $('#delete-petition-'+id).hide();
        return false;
      }
    );
    $(".delete-petition-overlay #button-no-thanks-keep").click(
      function () {
        var id = $(this).attr('rel');
        $('#mask').hide();
        $('#delete-petition-'+id).hide();
        return false;
      }
    );

    $(".delete-petition-form").unbind();
    $(".delete-petition-form").click(function() {
      var id = $(this).attr('rel');
      $('#delete-petition-'+id).show();
      return false;
    });
  }
}
sign_return = '';
sign_id = '';
Drupal.behaviors.signModal = {
  attach: function(context) {
    $("#button-sign-this-petition").unbind();
    $("#button-sign-this-petition").bind('click',
      function () {
        var wpos = $(window).scrollTop();
        wpos = wpos + (($(window).height() - 390) / 2);
        $('#thank-you-modal').css({'top':wpos+'px'});
        $('#footer').css('position', 'static');
        $('#wh-footer').css('position', 'static');
        $('#thank-you-modal').show();
        $('#sign-this-petition').append('<div id="comment-mask"></div>');
        var maskHeight = $(document).height() + 300;
        var maskWidth = $(window).width();
        var pos = $('#sign-this-petition').position();
        var left_margin = (maskWidth - 976) / 2;
        $('#comment-mask').css({'width':maskWidth,'height':maskHeight,'top':'-'+pos.top+'px','left':'0px'});
        $('#comment-mask').show();

        var id = $(this).attr('rel');
        sign_id = id;
        $.post("/petition/sign/" + id, { petition_id: id, js: true },
          function(data) {
            sign_return = data;
          }
        );

        return false;
      }
    );
    $("#thank-you-modal .close-button").unbind();
    $("#thank-you-modal .close-button").click(
      function () {
        $('#comment-mask').hide();
        $('#thank-you-modal').hide();
        $('#footer').css('position', 'relative');
        $('#wh-footer').css('position', 'relative');
        if(sign_return != 0) {
          $('#sign-this-petition').replaceWith(sign_return);
          var url = "/signatures/more/"+sign_id+"/1/0/0/";
          var params = {};
          $.getJSON(url, params, function(data) {
            signature_page++;
            $(".load-next").remove();
            $("#signature-list .entry").remove();
            $("#signature-list").append(data.markup);
            var newsignatures = $(data.markup).filter('.entry').length;
            var newcount = newsignatures;
            $('#signature-list .count').html(newcount);
            var prevtotal = parseInt($('#signature-list .total-count').html());
            $('#signature-list .total-count').html((prevtotal + 1));

            if (typeof wh_petition_adjustHeight == 'function') {
              wh_petition_adjustHeight();
            }

            Drupal.attachBehaviors('#signatures');
          });
        }
      }
    );
    $("#wh-petitions-comment-petition-form").unbind();
    $("#wh-petitions-comment-petition-form").submit(
      function () {
        var id = $("input[name=petition_id]").val();
        var comment = $("textarea[name=signature_comment]").val();
        var uid = $("input[name=user_id]").val();
        $.post("/petition/comment/" + id, { petition_id: id, js: true, signature_comment: comment, user_id: uid },
          function(data) {
           $('#thank-you-modal #comment-form-wrapper').html(data);
          }
        );
        return false;
      }
    );
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.issuesValidate = {
  attach: function(context) {
    $("#create-petition-form-step-1 #edit-petition-issues input").change(
      function () {
        var checked = $('#create-petition-form-step-1 #edit-petition-issues input').filter(':checked').length;
        if (checked == 3) {
          $('#create-petition-form-step-1 #edit-petition-issues input:not(:checked)').attr('disabled', true);
        }
        else if(checked == 2) {
          $('#create-petition-form-step-1 #edit-petition-issues input:not(:checked)').attr('disabled', false);
        }
        else if(checked > 3) {
          $(this).attr('checked', false);
          $('#create-petition-form-step-1 #edit-petition-issues input:not(:checked)').attr('disabled', true);
        }
      }
    );
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.tooltipThresholdHover = {
  attach: function(context) {
    $("#petition-inner .view-threshold-tooltip").hover(
      function () {
        $('#threshold-tooltip').show();
      },
      function () {
        $('#threshold-tooltip').hide();
      }
    );
    $("#petition-inner .view-threshold-tooltip a").click(
      function () {
        return false;
      }
    );
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.tooltipHover = {
  attach: function(context) {
    $("#petition-inner .view-tooltip").hover(
      function () {
        $('#description-tooltip').show();
      },
      function () {
        $('#description-tooltip').hide();
      }
    );
  }
}
Drupal.behaviors.morePetitionsScroll = {
  attach: function(context) {
    $(".petition-response #more-petitions").click(
      function () {
        var pos = $(".petition-list .full-page-list").position();
        if (pos !== null) {
          pos = pos.top + 225;
          $("html,body").animate({scrollTop:pos}, "slow");
        }
        return false;
      }
    );
  }
}
Drupal.behaviors.gotoResponse = {
  attach: function(context) {
    $(".top-msg-bar-blue span").click(function () {
        var pos = $(".petition-response").position();
        if (pos !== null) {
          pos = pos.top;
          $("html,body").animate({scrollTop:pos}, "slow");
        }
        return false;
    });
  }
}


})(jQuery);

(function ($) {
Drupal.behaviors.tooltip2Hover = {
  attach: function(context) {
    $("#petition-inner .view-tooltip-tag").hover(
      function () {
        $('#tags-tooltip').show();
      },
      function () {
        $('#tags-tooltip').hide();
      }
    );
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.tooltip3Hover = {
  attach: function(context) {
    $("#petition-inner #form-item-create-petition-title .view-some").hover(
      function () {
        $('#title-tooltip').show();
      },
      function () {
        $('#title-tooltip').hide();
      }
    );
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.whyOverlay = {
  attach: function(context) {
    $("#why-overlay-text").unbind();
    $("#why-overlay-text").hover(
      function () {
        $('#why-overlay').show();
      },
      function () {
        $('#why-overlay').hide();
      }
    );
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.titleCount = {
  attach: function(context) {
    var char_limit = 120;
    $('#form-item-create-petition-title textarea').keyup(function() {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      // Displays count
      $('#form-item-create-petition-title .character-limit').html(chars_left + ' characters');
      if($(this).val().length > char_limit) {
        $(this).val(($(this).val().substring(0, char_limit)));
        $('#form-item-create-petition-title .character-limit').html(chars_left + ' characters');
      }
    });

    if($('#form-item-create-petition-title').length > 0) {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      $('#form-item-create-petition-title .character-limit').html(chars_left + ' characters');
    }
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.tagsCount = {
  attach: function(context) {
    var char_limit = 120;
    $('.form-item-petition-tags input').keyup(function() {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      // Displays count
      $('.form-item-petition-tags .character-count').html(chars_left + ' characters remaining.');
      if($(this).val().length > char_limit) {
        $(this).val(($(this).val().substring(0, char_limit)));
        $('.form-item-petition-tags .character-count').html(chars_left + ' characters remaining.');
      }
    });

    if($('.form-item-petition-tags').length > 0) {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      $('.form-item-petition-tags .character-count').html(chars_left + ' characters remaining.');
    }
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.descCount = {
  attach: function(context) {
    var char_limit = 800;
    $('.form-item-petition-description textarea').keyup(function() {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      // Displays count
      $('.form-item-petition-description .character-count').html(chars_left + ' characters remaining.');
      if($(this).val().length > char_limit) {
        $(this).val(($(this).val().substring(0, char_limit)));
        $('.form-item-petition-description .character-count').html(chars_left + ' characters remaining.');
      }
    });

    if($('.form-item-petition-description').length > 0) {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      $('.form-item-petition-description .character-count').html(chars_left + ' characters remaining.');
    }
  }
}
})(jQuery);

(function ($) {
Drupal.behaviors.commentCount = {
  attach: function(context) {
    var char_limit = 240;
    $('#wh-petitions-comment-petition-form textarea').keyup(function() {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      // Displays count
      $('#wh-petitions-comment-petition-form .help-text').html('<strong>'+chars_left+'</strong>' + ' characters remaining.');
      if($(this).val().length > char_limit) {
        $(this).val(($(this).val().substring(0, char_limit)));
        $('#wh-petitions-comment-petition-form .help-text').html('<strong>'+chars_left+'</strong>' + ' characters remaining.');
      }
    });

    if($('#wh-petitions-comment-petition-form').length > 0) {
      var chars_left = getCharsLeft($(this).val().length, char_limit);
      $('#wh-petitions-comment-petition-form .help-text').html('<strong>'+chars_left+'</strong>' + ' characters remaining.');
    }
  }
}
})(jQuery);

function getCharsLeft(length, limit) {
  var chars_left = limit - length;
  if (chars_left < 0) { chars_left = 0; }
  return chars_left;
}

/* Get the Issues that have been selected */
function retrieveIssues() {
  var issues = '';
  (jQuery)("#form-item-issues-filter input:checked").each(function() {
    if (issues == '') {
      issues = (jQuery)(this).val();
    }
    else {
      issues += '+' + (jQuery)(this).val();
    }
  });
  if (issues == '') { issues = '0'; }
  return issues;
}

/* Get the search terms */
function retrieveSearch() {
  var search = (jQuery)("#form-item-search-drop input#edit-filter-search").val();
  if (search == 'Enter your search terms') { search = ' '; }
  if (search == '') { search = ' '; }
  return escape(search);
}

function retrieveCols() {
  var cols = '';
  if((jQuery)(".filter-list li.view-one-column a").hasClass('active')) { cols = 1; }
  else { cols = 2; }
  return cols;
}

function retrieveSort() {
  var sort = '';
  var sort_text = (jQuery)("ul.select-view .active a").html();
  if (sort_text == 'All Petitions') { sort = 'all'; }
  else if (sort_text == 'Trending') { sort = 'trending'; }
  else if (sort_text == 'Popular') { sort = 'popular'; }
  else { sort = 'all'; }
  return sort;
}

(function ($) {
Drupal.behaviors.petitions_user_login_links = {
  attach: function(context) {
    // Catch special cases when D7 displays a Sign This Petition button
    // because it thinks you're logged in but really not. In this case,
    // display the login form. That form won't exist if you're logged in.
    $('#button-sign-this-petition-signin').not('.petition-processed').click(function(context) {
      top.utility_bar_signin_form_show();
      return false;
    }).addClass('petition-processed');

    // From the petition details page
    $('#sign-this-petition #button-sign-in').not('.petition-processed').click(function(context) {
      utility_bar_signin_form_show();
      return false;
    }).addClass('petition-processed');
    // From the petition details page
    $('#sign-this-petition #button-create-an-account').not('.petition-processed').click(function(context) {
      utility_bar_signin_form_show(true);
      return false;
    }).addClass('petition-processed');

    // From within iframe of the create petition page
    $('#sign-in-or-create-account #button-sign-in').not('.petition-processed').click(function(context) {
      top.utility_bar_signin_form_show();
      return false;
    }).addClass('petition-processed');
    // From within iframe of the create petition page
    $('#sign-in-or-create-account #button-create-an-account').not('.petition-processed').click(function(context) {
      top.utility_bar_signin_form_show(true);
      return false;
    }).addClass('petition-processed');


    // This disables the action of the sign this petition button
    // for anonymous users to ensure it doesn't take them away from
    // the current page.
    $('#button-sign-this-petition-inactive').not('.petition-processed').click(function(context) {
      return false;
    }).addClass('petition-processed');


    // TODO: Figure out if this can be deleted. What does it do?
    $('#utility-bar #button-sign-in').not('.petition-processed').click(function(context) {
      utility_bar_signin_form_show();
      return false;
    }).addClass('petition-processed');
  }
}
})(jQuery);

/**
 * Apply yellow thank-you banner to the top of pages as needed.
 */
var thanksmessagedisplayed = false;
(function ($) {
  Drupal.behaviors.petitions_yellow_bar = {
    attach: function(context) {
      // Find the appropriate thank-you and display it on the page at the appropriate spot
      if (!thanksmessagedisplayed) {
        var thanksmsg = '<div id="userreg-thanks">' + 'IMPORTANT: You have not finished creating your account. You will receive an email from us within a few minutes. You must click on the link in that email to continue.' +'</div>';
        thanksmessagedisplayed = true;
        if (location.href.indexOf('thank-you=p') != -1) {
          setTimeout(function() {
            $('#petition-outer').prepend(thanksmsg);
              setTimeout(function() {
                $('#userreg-thanks').slideUp();
              }, 300000);
            }, 500);
          $('#control .status').hide();
          $('.congreet_status').hide();
        }
      }
    }
  }
}) (jQuery);
