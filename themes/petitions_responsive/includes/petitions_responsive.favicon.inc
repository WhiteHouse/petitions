<?php

function petitions_responsive_html_head_alter(&$head_elements) {
  $favicon_path = $GLOBALS['base_url'] . '/' . path_to_theme() . '/' . 'images/favicons';

  $favicon_apple_touch_precomposed = array(
    'apple_touch_icon_precomposed_default' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'href' => $favicon_path . '/apple-touch-icon-60x60.png',
    ),
    'apple_touch_icon_precomposed_60' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '60x60',
      'href' => $favicon_path . '/apple-touch-icon-60x60.png',
    ),
    'apple_touch_icon_precomposed_72' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '72x72',
      'href' => $favicon_path . '/apple-touch-icon-72x72.png',
    ),
    'apple_touch_icon_precomposed_76' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '76x76',
      'href' => $favicon_path . '/apple-touch-icon-76x76.png',
    ),
    'apple_touch_icon_precomposed_114' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '114x114',
      'href' => $favicon_path . '/apple-touch-icon-114x114.png',
    ),
    'apple_touch_icon_precomposed_120' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '120x120',
      'href' => $favicon_path . '/apple-touch-icon-120x120.png',
    ),
    'apple_touch_icon_precomposed_144' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '144x144',
      'href' => $favicon_path . '/apple-touch-icon-144x144.png',
    ),
    'apple_touch_icon_precomposed_152' => array(
      'rel' => 'apple-touch-icon-precomposed',
      'sizes' => '152x152',
      'href' => $favicon_path . '/apple-touch-icon-152x152.png',
    ),
  );

  $favicon_apple_touch = array(
    'apple-touch-icon_default' => array(
      'rel' => 'apple-touch-icon',
      'href' => $favicon_path . '/iOS_76x76.png',
    ),
    'apple_touch_icon_76' => array(
      'rel' => 'apple-touch-icon',
      'sizes' => '76x76',
      'href' => $favicon_path . '/iOS_76x76.png',
    ),
    'apple_touch_icon_120' => array(
      'rel' => 'apple-touch-icon',
      'sizes' => '120x120',
      'href' => $favicon_path . '/iOS_120x120.png',
    ),
    'apple_touch_icon_152' => array(
      'rel' => 'apple-touch-icon',
      'sizes' => '152x152',
      'href' => $favicon_path . '/iOS_152x152.png',
    ),
  );

  $favicon_favicon = array(
    'favicon_16' => array(
      'rel' => 'icon',
      'type' => 'image/png',
      'sizes' => '16x16',
      'href' => $favicon_path . '/favicon-16x16.png',
    ),
    'favicon_32' => array(
      'rel' => 'icon',
      'type' => 'image/png',
      'sizes' => '32x32',
      'href' => $favicon_path . '/favicon-32x32.png',
    ),
    'favicon_96' => array(
      'rel' => 'icon',
      'type' => 'image/png',
      'sizes' => '96x96',
      'href' => $favicon_path . '/favicon-96x96.png',
    ),
    'favicon_128' => array(
      'rel' => 'icon',
      'type' => 'image/png',
      'sizes' => '128x128',
      'href' => $favicon_path . '/favicon-128.png',
    ),
    'favicon_196' => array(
      'rel' => 'icon',
      'type' => 'image/png',
      'sizes' => '196x196',
      'href' => $favicon_path . '/favicon-196x196.png',
    ),
  );

  $favicon_items = array_merge($favicon_apple_touch_precomposed, $favicon_apple_touch, $favicon_favicon);

  foreach ($favicon_items AS $key => $attributes) {
    $head_elements[$key] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => $attributes,
    );
    drupal_add_html_head($head_elements, $key);
  }

  // Register msapplication xml path
  $head_elements['msapplication_config'] = array(
    'msapplication_config' => array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'msapplication-config',
        'content' => $GLOBALS['base_url'] . '/' . path_to_theme() . '/browserconfig.xml',
      ),
    ),
  );

  drupal_add_html_head($head_elements, 'msapplication_config');

  // append Internet Explorer 11 Items
  $favicon_internet_explorer = array(
    'application-name' => array(
      'name' => 'application-name',
      'content' => ' Petitions'
    ),
    'msapplication-TileColor' => array(
      'name' => 'msapplication-TileColor',
      'content' => '#ffffff',
    ),
    'msapplication-square70x70logo' => array(
      'name' => 'msapplication-square70x70logo',
      'content' => $favicon_path . '/mstile-70x70.png',
    ),
    'msapplication-square150x150logo' => array(
      'name' => 'msapplication-square150x150logo',
      'content' => $favicon_path . '/mstile-150x150.png',
    ),
    'msapplication-square310x310logo' => array(
      'name' => 'msapplication-square310x310logo',
      'content' => $favicon_path . '/mstile-310x310.png',
    ),
    'msapplication-wide310x150logo' => array(
      'name' => 'msapplication-wide310x150logo',
      'content' => $favicon_path . '/mstile-310x150.png',
    ),
  );

  foreach ($favicon_internet_explorer AS $key => $attributes) {
    $head_elements[$key] = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => $attributes,
    );
    drupal_add_html_head($head_elements, $key);
  }

  // restoring short cut icon
    $head_elements['short-cut'] = array(
      '#type' => 'html_tag',
      '#tag' => 'link',
      '#attributes' => array(
        'rel' => 'shortcut icon',
        'href' => $GLOBALS['base_url'] . '/' . path_to_theme() . '/favicon.ico',
        'type' => 'image/vnd.microsoft.icon',
      ),
    );
    drupal_add_html_head($head_elements, 'short-cut');
}
