<?php

/**
 * Here we override the default HTML output of drupal.
 * refer to https://drupal.org/node/457740
 */

require "includes/petitions_responsive.favicon.inc";

// Auto-rebuild the theme registry during theme development.
if (theme_get_setting('clear_registry')) {
  // Rebuild .info data.
  system_rebuild_theme_data();
  // Rebuild theme registry.
  drupal_theme_rebuild();
}

// Add Zen Tabs styles
if (theme_get_setting('petitions_responsive_tabs')) {
  drupal_add_css( drupal_get_path('theme', 'petitions_responsive') .'/css/tabs.css');
}

function petitions_responsive_preprocess_html(&$vars) {
  global $user, $language;

  // Add role name classes (to allow css based show for admin/hidden from user)
  foreach ($user->roles as $role){
    $vars['classes_array'][] = 'role-' . petitions_responsive_id_safe($role);
  }

  // HTML Attributes
  // Use a proper attributes array for the html attributes.
  $vars['html_attributes'] = array();
  $vars['html_attributes']['lang'][] = $language->language;
  $vars['html_attributes']['dir'][] = $language->dir;

  // Convert RDF Namespaces into structured data using drupal_attributes.
  $vars['rdf_namespaces'] = array();
  if (function_exists('rdf_get_namespaces')) {
    foreach (rdf_get_namespaces() as $prefix => $uri) {
      $prefixes[] = $prefix . ': ' . $uri;
    }
    $vars['rdf_namespaces']['prefix'] = implode(' ', $prefixes);
  }

  // Flatten the HTML attributes and RDF namespaces arrays.
  $vars['html_attributes'] = drupal_attributes($vars['html_attributes']);
  $vars['rdf_namespaces'] = drupal_attributes($vars['rdf_namespaces']);

  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    list($section, ) = explode('/', $path, 2);
    $vars['classes_array'][] = 'with-subnav';
    $vars['classes_array'][] = petitions_responsive_id_safe('page-'. $path);
    $vars['classes_array'][] = petitions_responsive_id_safe('section-'. $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          // Remove 'section-node'
          array_pop( $vars['classes_array'] );
        }
        // Add 'section-node-add'
        $vars['classes_array'][] = 'section-node-add';
      }
      elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          // Remove 'section-node'
          array_pop( $vars['classes_array']);
        }
        // Add 'section-node-edit' or 'section-node-delete'
        $vars['classes_array'][] = 'section-node-'. arg(2);
      }
    }
  }
  //for normal un-themed edit pages
  if ((arg(0) == 'node') && (arg(2) == 'edit')) {
    $vars['template_files'][] =  'page';
  }

  // Add IE classes.
  if (theme_get_setting('petitions_responsive_ie_enabled')) {
    $petitions_responsive_ie_enabled_versions = theme_get_setting('petitions_responsive_ie_enabled_versions');
    if (in_array('ie8', $petitions_responsive_ie_enabled_versions, TRUE)) {
      drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
      drupal_add_js(path_to_theme() . '/js/build/selectivizr-min.js');
    }
    if (in_array('ie9', $petitions_responsive_ie_enabled_versions, TRUE)) {
      drupal_add_css(path_to_theme() . '/css/ie9.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 9', '!IE' => FALSE), 'preprocess' => FALSE));
    }
    if (in_array('ie10', $petitions_responsive_ie_enabled_versions, TRUE)) {
      drupal_add_css(path_to_theme() . '/css/ie10.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 10', '!IE' => FALSE), 'preprocess' => FALSE));
    }
  }

  $image_path = drupal_get_path('theme', 'petitions_responsive') . '/images/fb_share.jpg';
  $options = array('absolute' => TRUE, 'https' => TRUE);
  $path = url($image_path, $options);
  $meta_og_img = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image',
      'content' => $path,
    ),
  );
  drupal_add_html_head($meta_og_img, 'meta_og_img');

  $image_info = getimagesize($image_path);
  $image_width = $image_info['0'];
  $image_height = $image_info['1'];
  $meta_og_img_width = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image:width',
      'content' => $image_width,
    ),
  );
  drupal_add_html_head($meta_og_img_width, 'meta_og_img_width');
  $meta_og_img_height = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'property' => 'og:image:height',
      'content' => $image_height,
    ),
  );
  drupal_add_html_head($meta_og_img_height, 'meta_og_img_height');




}

function petitions_responsive_preprocess_page(&$vars, $hook) {
  if (isset($vars['node'])) {
    $vars['title'] = $vars['node']->title;
  }

  $theme_path = '/' . drupal_get_path('theme', 'petitions_responsive');
  $vars['theme_path'] = $theme_path;

  global $base_url;
  $vars['base_url'] = $base_url;

  $user_menu_links = menu_navigation_links('main-menu');
  $user_menu = array(
    '#theme' => 'links',
    '#links' => $user_menu_links,
    '#attributes' => array(
      'id' => 'user-menu',
    ),
  );

  $vars['user_menu'] = $user_menu;

  // Expose the dashboard menu as a variable to the page template.
  $vars['dashboard_menu'] = array(
    '#theme_wrappers' => array('container'),
    '#attributes' => array(
      'class' => 'dashboard-menu',
    ),
    'label' => array(
      '#theme' => 'html_tag',
      '#tag' => 'span',
      '#attributes' => array(
        'class' => 'dashboard-menu-label',
      ),
      '#value' => t('Dashboard:'),
    ),
    'menu' => array(
      '#theme' => 'links',
      '#links' => menu_navigation_links('menu-petitions-dashboard'),
      '#attributes' => array(
        'id' => 'petitions-dashboard-menu',
      ),
    ),
  );

  // Add the content intro field to the intro region:
  if (isset($vars['node'])) {
    $node = $vars['node'];
    if (field_get_items('node', $node, 'field_page_intro')) {
      $intro_field = field_view_field('node', $node, 'field_page_intro', 'full');
      $vars['page']['content_top']['field_page_intro'] = $intro_field;
    }
  }

  // Adding classes whether #navigation is here or not.
  if (!empty($vars['main_menu']) or !empty($vars['sub_menu'])) {
    $vars['classes_array'][] = 'with-navigation';
  }
  if (!empty($vars['secondary_menu'])) {
    $vars['classes_array'][] = 'with-subnav';
  }

  // Add sidebar page classes.
  if (!empty($vars['page']['sidebar_top']) || !empty($vars['page']['sidebar_bottom'])) {
    $vars['classes_array'][] = 'has-sidebar';
  }
  if (!empty($vars['page']['sidebar_top'])) {
    $vars['classes_array'][] = 'has-sidebar-top';
  }
  if (!empty($vars['page']['sidebar_bottom'])) {
    $vars['classes_array'][] = 'has-sidebar-bottom';
  }

  // Add first/last classes to node listings about to be rendered.
  if (isset($vars['page']['content']['system_main']['nodes'])) {
    // All nids about to be loaded (without the #sorted attribute).
    $nids = element_children($vars['page']['content']['system_main']['nodes']);
    // Only add first/last classes if there is more than 1 node being rendered.
    if (count($nids) > 1) {
      $first_nid = reset($nids);
      $last_nid = end($nids);
      $first_node = $vars['page']['content']['system_main']['nodes'][$first_nid]['#node'];
      $first_node->classes_array = array('first');
      $last_node = $vars['page']['content']['system_main']['nodes'][$last_nid]['#node'];
      $last_node->classes_array = array('last');
    }
  }

  if (isset($vars['page']['content']['system_main']['#form_id'])) {
    $form_id = $vars['page']['content']['system_main']['#form_id'];

    if ($form_id == 'user_login' || $form_id == 'user_register_form') {
      switch ($form_id) {
        case 'user_login':
          $login_form = $vars['page']['content']['system_main'];
          $registration_form = drupal_get_form('user_register_form');
          break;
        case 'user_register_form':
          $registration_form = $vars['page']['content']['system_main'];
          $login_form = drupal_get_form('user_login');
          break;
        default:
          $login_form = drupal_get_form('user_login');
          $registration_form = drupal_get_form('user_register_form');
          break;
      }
      unset($vars['page']['content']['system_main']);
      $login_form['#weight'] = 0;
      $registration_form['#weight'] = 1;
      $vars['page']['content']['system_main']['login_form'] = $login_form;
      $vars['page']['content']['system_main']['registration_form'] = $registration_form;

      // Do not display tabs provided by core on the login page.
      unset($vars['tabs']);

      $title_suffix = array(
        '#theme' => 'html_tag',
        '#tag' => 'p',
        '#value' => t('In order to create a We the People petition, you must first create a user account and verify your email address is valid.'),
        '#attributes' => array(
          'id' => array('title_suffix'),
        )
      );

      $vars['title_suffix'] = $title_suffix;
    }
    if ($form_id == 'user_pass') {
      $vars['title'] = FALSE;
    }
  }

  // Allow page override template suggestions based on node content type.
  if (isset($vars['node']->type) && isset($vars['node']->nid)) {
    $vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
    $vars['theme_hook_suggestions'][] = "page__node__" . $vars['node']->nid;
  }
}

function petitions_responsive_preprocess_node(&$vars) {
  // Add a striping class.
  $vars['classes_array'][] = 'node-' . $vars['zebra'];

  // Add $unpublished variable.
  $vars['unpublished'] = (!$vars['status']) ? TRUE : FALSE;

  // Merge first/last class (from petitions_responsive_preprocess_page) into classes array of current node object.
  $node = $vars['node'];
  $is_teaser = (isset($vars['view_mode']) && $vars['view_mode'] == 'teaser');

  if (!empty($node->classes_array)) {
    $vars['classes_array'] = array_merge($vars['classes_array'], $node->classes_array);
  }

  // Make "node--NODETYPE--VIEWMODE.tpl.php" templates available for nodes
  $vars['theme_hook_suggestions'][] = 'node__' . $vars['type'] . '__' . $vars['view_mode'];


  // Make sure petition issues appear before the social links.
  if (isset($vars['content']['field_petition_issues'])) {
    $weight = $is_teaser ? -10 : 10;
    $vars['content']['field_petition_issues']['#weight'] = $weight;
  }

  // Add share links to the content array after issues.
  if (isset($vars['content']['share_links'])) {
    if ($is_teaser) {
      unset($vars['content']['share_links']);
    }
    else {
      $vars['content']['share_links']['#weight'] = 20;
    }
  }

  if (isset($vars['content']['field_response_id'])) {
    $response_prefix = array(
      '#theme' => 'html_tag',
      '#tag' => 'h2',
      '#value' => t('Response to Petition'),
      '#attributes' => array(
        'class' => array('element-invisible'),
      ),
    );
    $vars['content']['field_response_id']['#prefix'] = drupal_render($response_prefix);
    // Make sure the response appears after both issues and social links.
    $vars['content']['field_response_id']['#weight'] = 25;
  }
}

function petitions_responsive_preprocess_block(&$vars, $hook) {
  // Add the block delta to the classes array for custom blocks.
  if (!empty($vars['block']->delta)) {
    $vars['classes_array'][] = 'block-' . drupal_html_class($vars['block']->delta);
  }

  // Add a striping class.
  $vars['classes_array'][] = 'block-' . $vars['block_zebra'];

  // Add first/last block classes.
  $first_last = "";
  // If block id (count) is 1, it's first in region.
  if ($vars['block_id'] == '1') {
    $first_last = "first";
    $vars['classes_array'][] = $first_last;
  }
  // Count amount of blocks about to be rendered in that region.
  $block_count = count(block_list($vars['elements']['#block']->region));
  if ($vars['block_id'] == $block_count) {
    $first_last = "last";
    $vars['classes_array'][] = $first_last;
  }
  if ($vars['elements']['#block']->bid == 'views-0760e70c35322fc3c653f411c63f2660'){
    $vars['classes_array'][] = 'open-petitions';
  }
  elseif ($vars['elements']['#block']->bid == 'views-751ef66fe65c3cf71c599d3c6c5c7cd4') {
    $vars['classes_array'][] = 'block-popular-petitions';
  }
}

function petitions_responsive_preprocess_field(&$vars) {
  $element = $vars['element'];
  if ($element['#field_name'] == 'field_petition_issues') {
    foreach ($element['#items'] as $delta => $item) {
      $vars['items'][$delta]['#prefix'] = '<h6>';
      $vars['items'][$delta]['#suffix'] = '</h6>';
    }
    $vars['classes_array'][] = 'tags';
  }
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return
 *   A string containing the breadcrumb output.
 */
function petitions_responsive_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('petitions_responsive_breadcrumb');
  if ($show_breadcrumb == 'yes' || $show_breadcrumb == 'admin' && arg(0) == 'admin') {

    // Optionally get rid of the homepage link.
    $show_breadcrumb_home = theme_get_setting('petitions_responsive_breadcrumb_home');
    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      $breadcrumb_separator = theme_get_setting('petitions_responsive_breadcrumb_separator');
      $trailing_separator = $title = '';
      if (theme_get_setting('petitions_responsive_breadcrumb_title')) {
        $item = menu_get_item();
        if (!empty($item['tab_parent'])) {
          // If we are on a non-default tab, use the tab's title.
          $title = check_plain($item['title']);
        }
        else {
          $title = drupal_get_title();
        }
        if ($title) {
          $trailing_separator = $breadcrumb_separator;
        }
      }
      elseif (theme_get_setting('petitions_responsive_breadcrumb_trailing')) {
        $trailing_separator = $breadcrumb_separator;
      }

      // Provide a navigational heading to give context for breadcrumb links to
      // screen-reader users. Make the heading invisible with .element-invisible.
      $heading = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

      return $heading . '<div class="breadcrumb">' . implode($breadcrumb_separator, $breadcrumb) . $trailing_separator . $title . '</div>';
    }
  }
  // Otherwise, return an empty string.
  return '';
}

/**
 * Converts a string to a suitable html ID attribute.
 *
 * http://www.w3.org/TR/html4/struct/global.html#h-7.5.2 specifies what makes a
 * valid ID attribute in HTML. This function:
 *
 * - Ensure an ID starts with an alpha character by optionally adding an 'n'.
 * - Replaces any character except A-Z, numbers, and underscores with dashes.
 * - Converts entire string to lowercase.
 *
 * @param $string
 *  The string
 * @return
 *  The converted string
 */
function petitions_responsive_id_safe($string) {
  // Replace with dashes anything that isn't A-Z, numbers, dashes, or underscores.
  $string = strtolower(preg_replace('/[^a-zA-Z0-9_-]+/', '-', $string));
  // If the first character is not a-z, add 'n' in front.
  if (!ctype_lower($string{0})) { // Don't use ctype_alpha since its locale aware.
    $string = 'id'. $string;
  }
  return $string;
}

/**
 * Generate the HTML output for a menu link and submenu.
 *
 * @param $variables
 *  An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return
 *  A themed HTML string.
 *
 * @ingroup themeable
 *
 */
function petitions_responsive_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  // Adding a class depending on the TITLE of the link (not constant)
  $element['#attributes']['class'][] = petitions_responsive_id_safe($element['#title']);
  // Adding a class depending on the ID of the link (constant)
  if (isset($element['#original_link']['mlid']) && !empty($element['#original_link']['mlid'])) {
    $element['#attributes']['class'][] = 'mid-' . $element['#original_link']['mlid'];
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Override or insert variables into theme_menu_local_task().
 */
function petitions_responsive_preprocess_menu_local_task(&$variables) {
  $link =& $variables['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }
  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab ' . drupal_html_class('task-' . $link['title']) . '">' . $link['title'] . '</span>';
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function petitions_responsive_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  return $output;
}

function petitions_responsive_preprocess_form_element(&$variables) {
  // Adds empty label to checkboxes to ensure font awesome icon for checkbox
  // appears.
  $element = &$variables['element'];
  if ($element['#type'] == 'checkbox') {
    if (!isset($element['#title'])) {
      $element['#title'] = ' ';
    }
  }
}

function petitions_responsive_preprocess_checkbox(&$variables) {
  $element = &$variables['element'];
  // Add css class to distinguish between checkboxes with true labels and
  // checkboxes with empty labels.
  if (!isset($element['#title'])) {
    _form_set_class($element, array('checkbox-dummy-label'));
  }
}
