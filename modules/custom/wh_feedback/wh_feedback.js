(function($) {

Drupal.behaviors.wh_feedback = {
attach: function (context,settings) {

  if ($.browser.msie) { use_recaptcha = 0; } 

  function wh_feedback_valid_email(email) { return /^.+@.+\..+$/.test(email); }

  function wh_feedback_success() { 
	$('#wh-feedback-submit').remove();
	$('#wh-feedback-modal').html('<div id="wh-feedback-success"><div class="message">Thank you for your feedback!</div></div>');
	$('html, body').animate({
		scrollTop: $(".wh-feedback-link").offset().top
	}, 1000);
	setTimeout(function() { $('#wh-feedback-container').slideToggle('fast'); }, 6000);
  }

  function wh_feedback_validate() {

	var error_flag = 1;
	$("span[class^=wh-feedback-error-]").remove();

	if (!$("#wh-feedback-webform textarea[id=edit-submitted-comments]").val()) {
		$("#wh-feedback-webform textarea[id=edit-submitted-comments]")
			.before("<span class='wh-feedback-error-comments'>Please provide comments before submitting</span>")
			.css('border','2px solid red');
		error_flag = 0;
	} 

        if ($("#wh-feedback-webform input[id=edit-submitted-email-address]").val()) {
		var email = $("#wh-feedback-webform input[id=edit-submitted-email-address]").val();
		if (!wh_feedback_valid_email(email)) { 
	                $("#wh-feedback-webform input[id=edit-submitted-email-address]")
        	                .before("<span class='wh-feedback-error-email'>Please provide a valid email address</span>")
                	        .css('border','2px solid red');
	                error_flag = 0;
		}
	}
	return error_flag;

  }
  $("#wh-feedback-node-form #edit-submit").click(function() {
	if (!(wh_feedback_validate())) {
		return false; 
	}
  }); 

  $(".wh-feedback-link", context)
	.after('<div id="wh-feedback-container"><div id="wh-feedback-top"></div><div id="wh-feedback-modal"></div><div id="wh-feedback-bottom"></div></div>')
		
	.click(function () {
	
		var wh_feedback_submitted = false;

			$("#wh-feedback-modal").load("https://wwws.whitehouse.gov/splash/feedback-petitions/modal?key="+new Date().getTime(), function() {

				$("#wh-feedback-modal").prepend('<a href="#" id="wh-feedback-close">Close</a>');
				$("#wh-feedback-modal").append('<div><br /><div id="wh-feedback-submit"></div><br /></div>');
				$("#wh-feedback-comments").css("display","none");

				$("#wh-feedback-webform input[id=edit-submitted-email-address]").focus(function() { $(this).css("border","1px solid gainsboro"); $(".wh-feedback-error-email").remove(); });
                                $("#wh-feedback-webform textarea[id=edit-submitted-comments]").focus(function() { $(this).css("border","1px solid gainsboro"); $(".wh-feedback-error-comments").remove(); });

                                $("#wh-feedback-container").slideToggle('fast');
			        $('#wh-feedback-close').click(function () { $('#wh-feedback-container').slideToggle('fast'); return false; });

				$("#wh-feedback-submit").click(function () { 
					$("input[id=edit-submitted-browser-string]").val(navigator.userAgent);
					$("input[id=edit-submitted-origin-url]").val($(location).attr('pathname'));
				
					if (!wh_feedback_submitted) {
					   if ($("#wh-feedback-webform textarea[id=wh-feedback-comments]").val()) { wh_feedback_success(); wh_feedback_submitted = true; } else {
						$.ajax({
							type: "POST",
							url: 'https://wwws.whitehouse.gov/feedback-petitions',
							data: $('#wh-feedback-modal form[id^=webform-client-form-]').serialize(),
							beforeSend: function() {
								if (wh_feedback_validate()) { 
									$('#wh-feedback-submit').remove();
									$('#wh-feedback-modal').html('<div id="wh-feedback-success"><div class="message">Thank you for your feedback!</div></div>');
									$('html, body').animate({
										scrollTop: $(".wh-feedback-link").offset().top
									}, 1000); 
									setTimeout(function() { $('#wh-feedback-container').slideToggle('fast'); }, 6000);
									wh_feedback_submitted = true;
								} else {
									return false;
								}
							},
							success: function() { 
							}
						});
					   }
					}

					return false; 
				});
			});
	
		return false;
	  });

}
};
 
})(jQuery);
