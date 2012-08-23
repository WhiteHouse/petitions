<?php

/**
 * Implementation of hook_menu_default_menu_custom().
 */
function wh_petition_page_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-how-why
  $menus['menu-how-why'] = array(
    'menu_name' => 'menu-how-why',
    'title' => 'How & Why',
    'description' => '',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('How & Why');


  return $menus;
}
