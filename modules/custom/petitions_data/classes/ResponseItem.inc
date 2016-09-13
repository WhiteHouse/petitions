<?php

/**
 * @file
 * Contains ResponseItem.
 */

/**
 * Class ResponseItem.
 */

class ResponseItem {
  private $node = NULL;

  /**
   * ResponseItem constructor.
   *
   * @param object $node
   *   Node object to build item from.
   */
  public function __construct($node) {
    $this->node = $node;
  }

  /**
   * Check is node is a valid, public wh_response node.
   *
   * @param object $node
   *   Node to validate.
   *
   * @return bool
   *   TRUE if valid, FALSE otherwise.
   */
  public static function checkValid($node) {
    if (empty($node) || $node->type != 'wh_response' || $node->status != 1) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Get URL for response node.
   *
   * @param bool $absolute
   *   Returns absolute URL if TRUE, relative otherwise.
   *
   * @return string
   *   URL of response node.
   */
  public function getUrl($absolute = FALSE) {
    $base_url = variable_get('petitions_data_petitions_website_url', $GLOBALS['base_url']);
    $node_url = url('node/' . $this->node->nid);
    $node_url = ltrim($node_url, '/');

    $alias = drupal_get_path_alias($node_url);
    $base = $absolute ? $base_url . '/' : base_path();
    return $base . $alias;
  }

  /**
   * Gets the response in the form of a REST response item array.
   *
   * @return array
   *   An array as used by the REST API.
   */
  public function toRestResponseItemArray() {

    if ($this->node == NULL) {
      return NULL;
    }

    $issues = NULL;
    $petitions = NULL;
    $wrapper = entity_metadata_wrapper('node', $this->node);

    $body = $wrapper->body->value->raw();
    $body_plain = strip_tags($body);

    $agency = $wrapper->field_agency_name->raw();

    foreach ($wrapper->field_issues->value() as $index => $issue) {
      $issues[] = array('id' => $issue->tid, 'name' => $issue->name);
    }

    foreach ($wrapper->field_petition_id->value() as $index => $petition) {
      $petition_rel_url = url('node/' . $petition->nid);
      $petition_abs_url = petitions_data_url($petition_rel_url, TRUE);

      $petition_short_url = count($petition->field_short_url) ? $petition->field_short_url[LANGUAGE_NONE][0]['value'] : '';

      if (isset($petition->field_timestamp_reached_ready[LANGUAGE_NONE][0]['value'])) {
        $timestamp_reached_ready = $petition->field_timestamp_reached_ready[LANGUAGE_NONE][0]['value'];
      }
      else {
        $timestamp_reached_ready = NULL;
      }

      $petitions[] = array(
        'id' => $petition->nid,
        'url' => $petition_abs_url,
        'short_url' => $petition_short_url,
        'title' => $petition->title,
        'signature_count' => $petition->signature_count,
        'timestamp_reached_ready' => $timestamp_reached_ready,
      );
    }

    $response = array(
      'id' => $this->node->nid,
      'title' => $this->node->title,
      'body' => $body,
      'body_plain' => $body_plain,
      'petitions' => $petitions,
      'url' => $this->getUrl(TRUE),
      'created' => $this->node->created,
      'agency' => $agency,
      'issues' => $issues,
    );
    return $response;
  }
}
