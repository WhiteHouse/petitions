(function ($) {
/* Display the correct media field based on the status of the dropdown */
Drupal.behaviors.mediaChange = {
  attach: function(context) {	
	$('#edit-field-response-media select').change(function() {
	  tid = $(this).val();
	  $('.column-wrapper .media-type').each(function() {
	    if($(this).hasClass('media-id-' + tid)) {
	      $(this).removeClass('display-none');
	      $(this).addClass('display-block');
	    }
	    else {
	      $(this).removeClass('display-block');
	      $(this).addClass('display-none');
	    }
	  });
	});
  }
}

/* Slide open the intro field on the response edit form */
Drupal.behaviors.openIntro = {
  attach: function(context) {	
	$('#wh-response-node-form .petitions .open-intro').click(function() {
	  var id_arr = $(this).attr('id').split('-');
	  id = id_arr[2];	  
	  $('#wh-response-node-form .petitions .form-item-petitions-petition-'+id+'-intro').slideToggle();
	});
  }
}
})(jQuery);