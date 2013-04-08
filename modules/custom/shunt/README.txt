README
======

About
-----

  "In electronics, a shunt is a device which allows electric current to pass
  around another point in the circuit by creating a low resistance path."
  source: http://en.wikipedia.org/wiki/Shunt_(electrical)

  In an emergency situation you can "trip the shunt" to instruct Drupal to
  degrade gracefully.

  For example, if your site is the target of a denial of service (DOS) attack, you
  may want to disable forms to prevent legitimate users
  from trying to write to the database during the attack (to prevent them from
  getting white screens or getting frustrated when their data doesn't save).

  By itself, this module doesn't do anything. It just sets a
  variable, shunt_is_enabled to TRUE or FALSE. This lets other
  modules check if the shunt is enabled, and modify their
  behavior accordingly.

  Note: the primary function of this module is to degrade features gracefully
  without requiring any cache clears.


Usage
------

  Enable the shunt like this:

    A. Via Drush:

      drush vset shunt_is_enabled TRUE


    B. Via admin GUI

      Go here:
        admin/settings/shunt

      Enable shunt check box. Save.


Developers
----------

  Module developers can implement shunts for their own
  modules like this:

  <?php
    // Check to see if Shunt module is installed and the
    // shunt is enabled.
    $shunt_is_enabled = variable_get('shunt_is_enabled', FALSE);

    // Anywhere you want your module to be able to fail gracefully, do this:
    if ($shunt_is_enabled) {
      // Shunt is enabled. Fail gracefully.
    }
    else {
      // Shunt is not enabled. Proceed.
    }
  ?>


  If you want more granular control, your module can have its own checkbox on
  admin/settings/shunt. All you have to do is implement hook_shunt and
  return an array keyed by your own module's variable name.

  Please prefix your variable with your module's name.

  <?php
    /**
     * Implements hook_shunt().
     *
     * @return
     *  Array
     *   key = shunt name
     *   value = description
     */
    function example_shunt() {
      return array(
        // Make a custom shunt like this.
        // Then you can check to see if your own shunt has been flipped
        // by doing this:
        //
        // $example_shunt_is_enabled = variable_get('example_shunt_is_enabled', FALSE);
        //
        'example_shunt_is_enabled' => 'This is a shunt for Example module. It disables something specific in Example module.',
        // You can implement as many shunts as you want. Here's a second
        // shunt provided by example module.
        'example_shunt_2_is_enabled' => 'Lorem ipsum.',
      );
    }
  ?>


  /**
   * @TODO Implement hook_shunt_enable and hook_shunt_disable as documented below.
   * @TODO Add example/test implementations of hook_shunt_enable and hook_shunt_disable to shuntexample module.
   *
   *  [ ]  Add a custom form submission handler to fire when shunt config is updated
   *      - When shunt(s) are enabled, call module_invoke_all('shunt_enable', $values)
   *      - When shunt(s) are disabled, call module_invoke_all('shunt_disable', $values)
   *      - $values = array keyed by shunt name, value = shunt status (TRUE/FALSE for On/Off)
   *
   *  [ ] Provide a shunt_enable($name) function.
   *      - Call variable_set($name, TRUE)
   *      - Call module_invoke_all('shunt_enable', array($name => TRUE))
   *  [ ] Provide a shunt_disable($name) function.
   *      - Call variable_set($name, TRUE)
   *      - Call module_invoke_all('shunt_disable', array($name => FALSE))
   */

  If your module needs to take a one-time action when the shunt is enabled
  or disabled, implement hook_shunt_enable and hook_shunt_disable.

  <?php

    /**
     * Implements hook_shunt_enable().
     *
     * @param $shunts
     *  Array, keyed by shunt name, indicating the status of each
     *  shunt.
     */
     function example_shunt_enable($shunts) {
       // This function is called when shunts are enabled.
     }

    /**
     * Implements hook_shunt_disable().
     *
     * @param $shunts
     *  Array, keyed by shunt name, indicating the status of each
     *  shunt.
     */
     function example_shunt_disable($shunts) {
       // This function is called when shunts are disabled.
     }
  ?>
