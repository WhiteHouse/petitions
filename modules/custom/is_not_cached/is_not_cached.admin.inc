<?php
/**
 * @file is_not_cached.admin.inc
 */

/**
 * Menu callback for configuration form, (admin/config/system/is_not_cached).
 *
 * @see system_settings_form().  // TODO maybe easier to use a custom form.
 */
function is_not_cached_configure() {
  $form['is_not_cached_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Is not cached paths'),
    '#description' => t('Paths where $is_not_cached should be set to true in page.tpl.php. One path per line.'),
    '#default_value' => variable_get('is_not_cached_paths', ''),
  );

  if ($is_not_cached_paths = module_invoke_all('is_not_cached_paths')) {
    $form['is_not_cached_paths_from_other_modules'] = array(
      '#type' => 'item',
      '#title' => t("Is not cached paths (set by modules)"),
      '#description' => t('Paths where $is_not_cached should be set to true in page.tpl.php (these cannot be modified).'),
      '#markup' => theme('is_not_cached_display_paths', array('is_not_cached_paths' => $is_not_cached_paths)),
    );
  }

  if ($is_not_cached_rules = module_invoke_all('is_not_cached_rules')) {
    $form['is_not_cached_rules_from_other_modules'] = array(
      '#type' => 'item',
      '#title' => t('Is not cached rules'),
      '#description' => t('Check modules for rules'),
      '#markup' => theme('is_not_cached_display_rules', array('is_not_cached_rules' => $is_not_cached_rules)),
    );
  }

  $form['is_not_cached_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override $is_not_cached variable. Always set to TRUE.'),
    '#description' => t('This is useful for debugging, development, etc.'),
    '#default_value' => variable_get('is_not_cached_override', FALSE),
  );

  return system_settings_form($form);
}

