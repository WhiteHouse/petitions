About
-----

  The is_profane module allows users to:

  - Define a default list of profanity terms
  - Add or remove profanity terms from the administartor GUI
  - Check strings for profanity terms, and return TRUE/FALSE


Installation
------------

  1. Extract the module into your modules directory

  2. Go to "admin/modules" and select the "Enabled" checkbox next to Is Profane


Usage
-----

  On it's own, this module does not do anything.  It simply adds the ability for
  administrators to add/remove profanity terms, and allows modules to use the
  is_profane() function to check strings for terms.

  To add/remove profanity terms go to "admin/config/system/is_profane".


Developers
----------

  The following is a list of available functions this module provides:

    is_profane($string, $terms = array())

    This is the module's main function.  You can implement this in your module
    to check if a specific string contains any of the identified terms.  If you
    do not pass this function any terms, it will validate against the sites
    default terms.  This function returns TRUE/FALSE.

    is_profane_terms()

    This function returns an array containing all of the identified profanity
    terms.

    is_profane_terms_default()

    This function returns an array containing the default list of profanity
    terms.


Road-Map / To-Do
----------------

  NOTE-1:
  -------

    This module could easily be extended to use a third party service rather
    than our homegrown profanity filter. To implement this, the following check
    could be run against a service like webpurify.com
    (e.g. http://www.webpurify.com/documentation/samples/) rather than our own
    privately maintained list.

  TO-DO:
  ------

    1. To impement NOTE-1, update the $form returned by is_profane_form() to
       make third party service vs. site-admin-defined profanity terms
       determined by radio buttons. If the user chooses third party service,
       display supported third party services. Then, in is_profane(), check
       which strategy the site administrator has enabled. Then either use
       the terms defined in is_profane_terms() or call a third party service to
       determine whether the string is profane.

Credits
-------

  N/A
