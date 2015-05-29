<?php
/**
 * @file
 * wh_response_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wh_response_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function wh_response_feature_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function wh_response_feature_image_default_styles() {
  $styles = array();

  // Exported image style: agency_seal.
  $styles['agency_seal'] = array(
    'name' => 'agency_seal',
    'effects' => array(
      1 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '50',
          'height' => '50',
        ),
        'weight' => '1',
      ),
    ),
    'label' => 'agency_seal',
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function wh_response_feature_node_info() {
  $items = array(
    'wh_response' => array(
      'name' => t('Response'),
      'base' => 'node_content',
      'description' => t('Response to a petition'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
