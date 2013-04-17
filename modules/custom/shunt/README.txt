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
  variable to keep track of whether the shunt is enabled, lets modules check if
  the shunt is enabled, and provides hooks to let modules react to shunts being enabled/disabled.

  Note: the primary function of this module is to degrade features gracefully
  without requiring any cache clears.


Usage
------

  To see all available drush commands, do this:

    drush --filter=shunt


  Enable ("trip") the shunt to disable targeted site functionality like this:

    A. Via Drush:

      drush shunt-enable


    B. Via admin GUI

      Go here:
        admin/config/system/shunt

        Enable shunt check box. Save.

  
  Disable the shunt to renable site functionality like this:

    A. Via Drush:

      drush shunt-disable


    B. Via admin GUI

      Go here:
        admin/config/system/shunt

        Disable shunt check box. Save.
