<?php

/**
 * Provides a settings form for sticky elements.
 */
function sticky_element_admin() {
  $form = array();

  $form['sticky_elements'] = array(
    '#type' => 'textarea',
    '#title' => t('Sticky Element Settings'),
    '#description' => t('Target sticky elements, one per line, with parameters separated by a vertical pipe.  4 parameters are accepted: <ul><li>Sticky Element Selector, any valid css selector</li><li>(true|false, default false) Sticky Bottom. If false, the element will stick to viewport top instead of viewport bottom</li><li>(true|false, default true) Initially Sticky. When false, the element only becomes sticky after its natural dom position reaches the targeted viewport edge</li><li>(optional, css selector) End element. The sticky element will resume scrolling once meeting the edge of this element</li></ul>  e.g. <code>.sidebar #my-block | true | true | #footer</code>'),
    '#default_value' => _sticky_element_admin_convert_array_to_text(variable_get('sticky_element_sticky_elements', array())),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

function sticky_element_admin_validate($form, &$form_state) {
  $settings_array = _sticky_element_admin_convert_text_to_array($form_state['values']['sticky_elements']);

  foreach ($settings_array as $setting_array) {
    if (count($setting_array) > 4) {
      form_set_error('sticky_elements', t('Too many arguments supplied. Each line must of the format: <code>selector|bool|bool|selector</code>'));
    }
    foreach ($setting_array as $i => $setting) {
      if ($i == 1 || $i == 2) {
        if (strtolower($setting) != 'true' && strtolower($setting) != 'false') {
          form_set_error('sticky_elements', t('Argument @i should be either "true" or "false".', array('@i' => ($i + 1))));
        }
      }
    }
  }
}

/**
 * Submit handler for floating_block admin form.
 *
 * Saves sticky_elements variable.
 */
function sticky_element_admin_submit($form, &$form_state) {
  variable_set('sticky_element_sticky_elements', _sticky_element_admin_convert_text_to_array($form_state['values']['sticky_elements']));
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Converts a string representation of floating block settings to an array.
 *
 * @param $sticky_elements
 *   A string representation of floating block settings.
 *
 * @return
 *   An array representation of floating block settings.
 */
function _sticky_element_admin_convert_text_to_array($sticky_elements) {
  $sticky_elements = preg_split("/(\r\n|\n)/", $sticky_elements, NULL, PREG_SPLIT_NO_EMPTY);
  $settings = array();

  foreach ($sticky_elements as $sticky_element) {
    $settings[] = array_map('trim', explode('|', $sticky_element));
  }

  return $settings;
}

/**
 * Converts an array representation of floating block settings to a string.
 *
 * @param $sticky_elements
 *   An array representation of floating block settings.
 *
 * @return
 *   A string representation of floating block settings.
 */
function _sticky_element_admin_convert_array_to_text($sticky_elements) {
  $output = array();
  foreach ($sticky_elements as $sticky_element) {
    $output_line = '';
    foreach ($sticky_element as $i => $setting) {
      $output_line .= $setting;
      if ($i + 1 < count($sticky_element)) {
        $output_line .= ' | ';
      }
    }
    $output[] = $output_line;
  }
  return implode("\n", $output);
}
