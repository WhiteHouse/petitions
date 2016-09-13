<?php
/**
 * @file
 * wh_petition_page.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function wh_petition_page_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: main-menu.
  $menus['main-menu'] = array(
    'menu_name' => 'main-menu',
    'title' => 'Main menu',
    'description' => 'The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.',
  );
  // Exported menu: menu-how-why.
  $menus['menu-how-why'] = array(
    'menu_name' => 'menu-how-why',
    'title' => 'How & Why',
    'description' => '',
  );
  // Exported menu: petitions-menu.
  $menus['petitions-menu'] = array(
    'menu_name' => 'petitions-menu',
    'title' => 'Petitions Menu',
    'description' => 'Default Main Menu for Petitions install profile.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Default Main Menu for Petitions install profile.');
  t('How & Why');
  t('Main menu');
  t('Petitions Menu');
  t('The <em>Main</em> menu is used on many sites to show the major sections of the site, often in a top navigation bar.');


  return $menus;
}
