<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * A QUICK OVERVIEW OF DRUPAL THEMING
 *
 *   The default HTML for all of Drupal's markup is specified by its modules.
 *   For example, the comment.module provides the default HTML markup and CSS
 *   styling that is wrapped around each comment. Fortunately, each piece of
 *   markup can optionally be overridden by the theme.
 *
 *   Drupal deals with each chunk of content using a "theme hook". The raw
 *   content is placed in PHP variables and passed through the theme hook, which
 *   can either be a template file (which you should already be familiary with)
 *   or a theme function. For example, the "comment" theme hook is implemented
 *   with a comment.tpl.php template file, but the "breadcrumb" theme hooks is
 *   implemented with a theme_breadcrumb() theme function. Regardless if the
 *   theme hook uses a template file or theme function, the template or function
 *   does the same kind of work; it takes the PHP variables passed to it and
 *   wraps the raw content with the desired HTML markup.
 *
 *   Most theme hooks are implemented with template files. Theme hooks that use
 *   theme functions do so for performance reasons - theme_field() is faster
 *   than a field.tpl.php - or for legacy reasons - theme_breadcrumb() has "been
 *   that way forever."
 *
 *   The variables used by theme functions or template files come from a handful
 *   of sources:
 *   - the contents of other theme hooks that have already been rendered into
 *     HTML. For example, the HTML from theme_breadcrumb() is put into the
 *     $breadcrumb variable of the page.tpl.php template file.
 *   - raw data provided directly by a module (often pulled from a database)
 *   - a "render element" provided directly by a module. A render element is a
 *     nested PHP array which contains both content and meta data with hints on
 *     how the content should be rendered. If a variable in a template file is a
 *     render element, it needs to be rendered with the render() function and
 *     then printed using:
 *       <?php print render($variable); ?>
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. With this file you can do three things:
 *   - Modify any theme hooks variables or add your own variables, using
 *     preprocess or process functions.
 *   - Override any theme function. That is, replace a module's default theme
 *     function with one you write.
 *   - Call hook_*_alter() functions which allow you to alter various parts of
 *     Drupal's internals, including the render elements in forms. The most
 *     useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 *     and hook_page_alter(). See api.drupal.org for more information about
 *     _alter functions.
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   If a theme hook uses a theme function, Drupal will use the default theme
 *   function unless your theme overrides it. To override a theme function, you
 *   have to first find the theme function that generates the output. (The
 *   api.drupal.org website is a good place to find which file contains which
 *   function.) Then you can copy the original function in its entirety and
 *   paste it in this template.php file, changing the prefix from theme_ to
 *   petitions44_. For example:
 *
 *     original, found in modules/field/field.module: theme_field()
 *     theme override, found in template.php: petitions44_field()
 *
 *   where petitions44 is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_field() function.
 *
 *   Note that base themes can also override theme functions. And those
 *   overrides will be used by sub-themes unless the sub-theme chooses to
 *   override again.
 *
 *   Zen core only overrides one theme function. If you wish to override it, you
 *   should first look at how Zen core implements this function:
 *     theme_breadcrumbs()      in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called theme hook suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node--forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and theme hook suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */

/**
 * Preprocesses the wrapping HTML.
 *
 * @param array $vars
 *   Template variables.
 */
function petitions44_preprocess_html(&$vars) {
  // Set insecure facebook open graph image tag. There is a corresponding
  // .htaccess rule that allows this image to be accessed via http. It is
  // necessary to provide an http alternative of this image, due to FB bug.
  // @see https://developers.facebook.com/bugs/405588419493203
  $options = array('absolute' => TRUE, 'https' => FALSE);
  $path = url(drupal_get_path('theme', 'petitions44') . '/img/fb_share_we_the_people.png', $options);
  $path = str_replace('https://', 'http://', $path);
  $meta_og_img = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image',
      'content' => $path,
    ),
  );
  drupal_add_html_head($meta_og_img, 'meta_og_img');

  // Set secure facebook open graph image tag.
  $options = array('absolute' => TRUE, 'https' => TRUE);
  $path = url(drupal_get_path('theme', 'petitions44') . '/img/fb_share_we_the_people.png', $options);
  $meta_og_img_secure = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image:secure_url',
      'content' => $path,
    ),
  );
  drupal_add_html_head($meta_og_img_secure, 'meta_og_img_secure');

}

/**
 * Override or insert variables into the page template.
 */
function petitions44_process_page(&$variables) {
  /*
  Load template asset variables from theme_settings if locally modified,
  otherwise from variable_get / strongarm export
  */

  $variables['petitions44_help_text'] = _petitions44_help_text();
}

/**
 * Override or insert variables into the forward template.
 */
function petitions44_preprocess_forward(&$variables) {

  $email_text = '';

  if (arg(0) == 'petition') {
    $email_text = variable_get('wh_petitions_email_forward_text', 'Dear Friends,

    I wanted to let you know about an official petition I have signed at WhiteHouse.gov. Will you add your name to mine?  If this petition gets !signatures_needed signatures by !date_needed, the White House will review it and respond!

    You can view and sign the petition here: !shorturl

    Here\'s some more information about this petition:
    !petition_description');

  }
  else {

    $email_text = variable_get('wh_petitions_email_forward_response_text', 'Dear Friends,

    I wanted to let you know about an official petition I have signed at WhiteHouse.gov. Will you add your name to mine?  If this petition gets !signatures_needed signatures by !date_needed, the White House will review it and respond!

    You can view and sign the petition here: !shorturl

    Here\'s some more information about this petition:
    !petition_description');
  }

  $variables['email_text'] = $email_text;
}

/**
 * Override theme_menu_local_tasks().
 */
function petitions44_menu_local_tasks(&$variables) {
  $output = '';
  global $user;
  $arg = arg();

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#suffix'] = '</ul>';
    if ($arg[0] == 'user' && $arg[1] == $user->uid && $arg[2] == 'edit' ) {
      $variables['secondary']['#prefix'] .= '<ul class="tabs petitions44-user-edit">';
    }
    else {
      $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    }
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}


/**
 * Help text can be set as a theme setting.
 *
 * If no setting is set by the end user, a module can provide default help text.
 * If help text is not provided by the user or a module, a default
 * is defined here.
 *
 * @return String
 *   Help text.
 */
function _petitions44_help_text() {
  // Check for user-defined header.
  if (!$petitions44_help_text = theme_get_setting('petitions44_help_text')) {
    // If no user-defined header exists, check to see if an included module is
    // providing defaults (this is useful for implmentations like petitions).
    $text = t('Help make We the People even better. Share your feedback on how this new platform can improve.');
    $petitions44_help_text = variable_get('petitions44_help_text', $text);
  }
  return $petitions44_help_text;
}

/**
 * Preprocess services_documentation_version template.
 */
function template_preprocess_services_documentation_version(&$vars) {
  $vars['left_nav'] = wh_petition_tool_left_nav();
}
