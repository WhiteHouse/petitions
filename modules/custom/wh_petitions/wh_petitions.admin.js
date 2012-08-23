(function ($) {
Drupal.behaviors.bookmarkPetition = {
  attach: function(context) {
    $(".bookmark").click(
      function () {
        var pid_arr = $(this).attr('id').split('-');
        var pid = pid_arr[1];
        
        $.post("/admin/bookmark/" + pid, { petition_id: pid },
          function(data) {
            if(data) {
              $("#bookmark-" + pid).html(data).show();
            }
          }
        );
      }
    );
  }
}

Drupal.behaviors.featurePetition = {
  attach: function(context) {
    $(".featured").click(
      function () {
        var pid_arr = $(this).attr('id').split('-');
        var pid = pid_arr[1];
        $.post("/admin/featured/" + pid, { petition_id: pid },
          function(data) {
            if(data) {
              $("#featured-" + pid).html(data).show();
            }
          }
        );
      }
    );
  }
}

Drupal.behaviors.hidePetition = {
  attach: function(context) {
    $(".remove-lists").click(
      function () {
        var pid_arr = $(this).attr('id').split('-');
        var pid = pid_arr[2];
        $.post("/admin/remove-lists/" + pid, { petition_id: pid },
          function(data) {
            if(data) {
              $("#remove-lists-" + pid).html(data['link']).show();
              $("#petition-status-" + pid).html(data['petition']['petition_status']).show();
            }
          }
        );
      }
    );
  }
}

Drupal.behaviors.overridePetition = {
  attach: function(context) {
    $(".override-status").click(
      function () {
        var pid_arr = $(this).attr('id').split('-');
        var pid = pid_arr[2];
        $.post("/admin/override-status/" + pid, { petition_id: pid },
          function(data) {
            if(data) {
              $("#override-status-" + pid).html(data).show();
            }
          }
        );
      }
    );
  }
}

Drupal.behaviors.privateTags = {
  attach: function(context) {
    $(".edit-private-tags").click(
      function () {
        $('#edit-private-tags').toggle();
      }
    );
  }
}
})(jQuery);
