(function($) {
  Drupal.behaviors.wh_zipcodelookup = {
  attach: function(context, settings) {
    //hide the form elements and bind event handlers to elements in both forms
    var main_elements= getElementNames('user_register_form' || 'user_profile_form');

    hideFields('user_register_form');
    hideFields('user_profile_form');

    $(main_elements['zip_field']).blur(function(event) {processZipCode(Drupal.checkPlain($(main_elements['zip_field']).val()), this);});

  function showFields(form) {
    var elements = getElementNames(form);
    $(elements['city_wrapper']).css('display','');
    $(elements['state_wrapper']).css('display','');
  }

  function hideFields(form) {
    var elements = getElementNames(form);
    $(elements['city_wrapper']).css('display','none');
    $(elements['state_wrapper']).css('display','none');
  }

  function processZipCode(zip, callingElement) {
    //there has to be a better way to do this,
    //basically, it's getting the calling element, going up the DOM tree to find the form it's in
    //then coming back down the tree to find the id of that form
    var form = $("#" + $(callingElement).attr('id')).closest("form").find("input[name='form_id']").val();
    $.ajax({
      type: 'GET',
      url: '/wh_zipcodelookup/get_citystate/' + zip + '/' + form,
      dataType: 'json',
      success: displayCityState
    });
  }

  function displayCityState(data) {
    var form = data.form;
    var elements = getElementNames(form);
    showFields(form);

    if (data.data.length >0) {
      var pair = data.data[0];
      $(elements['city_field']).val(pair.city);
      $(elements['state_field'] + " > option[value=" + pair.state + "]").attr('selected', 'selected');
    }
    else {
      $(elements['city_field']).val('');
      $(elements['state_field'] + " > option").attr('selected', '');
    }
  }

  function getElementNames(form) {
    var elements = new Array();
    switch (form) {
      case 'user_register_form':
        elements['city_wrapper'] = '#edit-profile-main-field-city';
        elements['city_field'] = '#edit-profile-main-field-city-und-0-value';
        elements['state_wrapper'] = '#edit-profile-main-field-state';
        elements['state_field'] = '#edit-profile-main-field-state-und';
        elements['zip_field'] = '#edit-profile-main-field-zip-und-0-value';
        break;
      case 'user_profile_form':
        elements['city_wrapper'] = '#edit-profile-main-field-city';
        elements['city_field'] = '#edit-profile-main-field-city-und-0-value';
        elements['state_wrapper'] = '#edit-profile-main-field-state';
        elements['state_field'] = '#edit-profile-main-field-state-und';
        elements['zip_field'] = '#edit-profile-main-field-zip-und-0-value';
        break;
    }
    return elements;
  }
 }
}
})(jQuery);
