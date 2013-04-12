<?php
/**
 * Module developers can implement shunts for their own modules like this.
 */
function mymodule_does_something() {
  // Check to see if Shunt module is installed and the shunt is enabled.
  $shunt_is_enabled = (module_exists('shunt') && shunt_is_enabled()) ? TRUE : FALSE;

  // Or check to see if a specific shunt enabled.
  $shunt_is_enabled = (shunt_is_enabled('my_special_shunt')) ? TRUE : FALSE;

  // Anywhere you want your module to be able to fail gracefully, do this:
  if ($shunt_is_enabled) {
    // Shunt is enabled. Fail gracefully.
  }
  else {
    // Shunt is not enabled. Proceed.
  }
}

/**
 * Implements hook_shunt_enable().
 *
 * If your module needs to take a one-time action when the shunt is enabled
 * or disabled, implement hook_shunt_enable and hook_shunt_disable.
 *
 * @param string $name
 *  Name of shunt that was just enabled.
 */
 function example_shunt_enable($name) {
   // This function is called when shunts are enabled.
 }

/**
 * Implements hook_shunt_disable().
 *
 * @param string $name
 *  Name of shunt that was just disabled.
 */
 function example_shunt_disable($name) {
   // This function is called when shunts are disabled.
 }

/**
 * Implements hook_shunt().
 *
 *  If you want more granular control, your module can have its own checkbox on
 *  admin/settings/shunt. All you have to do is implement hook_shunt and
 *  return an array keyed by your own module's variable name.
 *
 *  Please prefix your variable with your module's name.
 *
 * @return
 *  Array. array('shunt_name' => 'description goes here');
 */
 function example_shunt() {
  return array(
    'example' => 'This is a shunt for Example module. It disables something specific in Example module.',
    // If your module needs two special shunt trips, you can implement as many as you need...
    'example2' => 'Lorem ipsum.',
  );
}
