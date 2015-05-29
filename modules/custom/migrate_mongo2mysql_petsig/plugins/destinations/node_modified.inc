<?php

/**
 * @file
 * Support for node destinations.
 */

class MigrateDestinationNodeModified extends MigrateDestinationNode {

  /**
   * Overloading prepare method.
   */
  public function prepare($entity, stdClass $source_row) {
    parent::prepare($entity, $source_row);

    // Let's check the title for unsupported chars.
    $entity->title = replace4byte($entity->title);

    // Let's check the body for unsupported chars.
    $entity->body[LANGUAGE_NONE][0]['value'] = replace4byte(
      $entity->body[LANGUAGE_NONE][0]['value']);

    // Let's check the legacy path for unsupported chars.
    $entity->field_legacy_path[LANGUAGE_NONE][0]['value'] = replace4byte(
      $entity->field_legacy_path[LANGUAGE_NONE][0]['value']);

    $entity->status = (empty($entity->field_petition_status[LANGUAGE_NONE][0]['value']) ? 0 : 1);
  }
}
