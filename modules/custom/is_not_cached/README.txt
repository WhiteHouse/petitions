Is Not Cached (7.x-1.x)
------------------------

  This module is for Drupal sites running behind a content delivery
  network (CDN). The use case is this: Your site mostly serves
  anonymous users cached content from your CDN. But you want to be able
  to make exceptions. For example, you don't want your 
  CDN to cache things like drupal_set_message() messages, but you don't want
  to remove $messages from your theme everywhere forever. But you also don't
  want to deal with edge side includes for some dynamic parts of your
  site (e.g. the pages where you want to make an exception, tell your 
  CDN not to cache the page, and let Drupal print $messages).

  This module does three simple things:

    1. Enables site administrators to tell Drupal a list of paths that
       are NOT cached by the CDN (e.g. pages that SHOULD display $messages).

    2. Enables modules to add to the list mentioned in #1 by adding paths
       and/or conditions in which pages are not cached.

    3. Surfaces a TRUE/FALSE $is_not_cached variable in page.tpl.php, to
       be used to do things like this:

           <?php if ($is_not_cached): ?>
             <?php print $messages; ?>
           <?php end; ?>

Usage
-----

  1. Go to admin/config/system/is_not_cached.

  2. Enter paths where $is_not_cached should be true in page.tpl.php.

  3. Now themers can add something like this to page.tpl.php: 

       /**
        * Variables from contrib modules:
        *  - $is_not_cached - TRUE/FALSE
        */

        ...

        <?php if ($is_not_cached): ?>
          <?php print $example; ?>
        <?php endif; ?>

Developers
----------
 
  Example hook implementations:

  /**
   * Implements hook_is_not_cached_paths().
   */
  function example_is_not_cached_paths() {
    return array(
      'path/to/example/1',  // <--- This is a path where $is_not_cached should be TRUE
      'path/to/example/2',  // <--- This is a path where $is_not_cached should be TRUE
      'path/to/example/3',  // <--- This is a path where $is_not_cached should be TRUE
    );  
  }

  /**
   * Implements hook_is_not_cached_rules().
   */
  function example_is_not_cached_rules() {
    // return array('callback_name_goes_here' => t('Human readable description goes here'));
    return array('user_is_logged_in' => t('User is logged in'));
  }
