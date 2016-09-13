<?php

/**
 * @file
 * Queue for validating signatures.
 */

/**
 * Class SignatureValidationProcessorQueue
 */
class SignatureValidationProcessorQueue implements DrupalReliableQueueInterface {

  /**
   * The name of the queue.
   *
   * @var string
   */
  protected $name;

  /**
   * Get the queue name.
   *
   * @return string
   *   The name of the queue.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Set queue name.
   *
   * @param string $name
   *   The name of the queue.
   */
  public function setName($name) {
    $this->name = $name;
    return $this;
  }

  /**
   * SignatureValidationProcessorQueue constructor.
   *
   * @param string $name
   *   The name of the queue.
   */
  public function __construct($name) {
    $this->name = $name;
  }

  /**
   * Create queue.
   *
   * @inheritDoc
   */
  public function createQueue() {
    // Nothing needed here. The queue table is created externally.
  }

  /**
   * Instantiate item in the database.
   *
   * @inheritDoc
   */
  public function createItem($data) {
    $fields = $data;
    $fields['name'] = $this->name;
    $fields['created'] = time();
    try {
      $insert = db_insert('signatures_pending_validation_queue')->fields($fields);
      return (bool) $insert->execute();
    }
    catch (PDOException $e) {
      if ($e->errorInfo[1] == 1062) {
        // Duplicate entry in unique key - log and return TRUE since the item
        // exists in the queue already.
        logger_event('signatures_queue.data_store.signature_validation_processor_queue.duplicate_entry');
        return TRUE;
      }
      else {
        // Log an error that is not a duplicate entry in unique key.
        logger_event('exceptions.signatures_queue.b91f8e2');
        watchdog('signatures_queue', "PDO error adding item !item to SignatureValidationProcessorQueue: !exception", array(
          '!item' => petition_format_for_watchdog($data),
          '!exception' => petition_format_for_watchdog($e),
        ), WATCHDOG_ERROR);
        return FALSE;
      }
    }
  }

  /**
   * Return the number of items in the queue.
   *
   * @inheritDoc
   */
  public function numberOfItems() {
    return db_query('
    SELECT COUNT(sid) FROM {signatures_pending_validation_queue} p
    JOIN {pending_validations_queue} v ON p.secret_validation_key = v.secret_validation_key
    WHERE p.expire = 0 AND p.name = :name AND p.processed = 0 ', array(':name' => $this->name))->fetchField();
  }

  /**
   * Claim item.
   *
   * @inheritDoc
   */
  public function claimItem($lease_time = 60) {
    while (TRUE) {
      $item = db_query_range('
      SELECT p.*, v.vid, v.client_ip, v.petition_id AS validated_petition_id,
      v.timestamp_received_signature_validation, v.timestamp_preprocessed_validation
      FROM {signatures_pending_validation_queue} p
      JOIN {pending_validations_queue} v ON p.secret_validation_key=v.secret_validation_key
      WHERE p.expire = 0 AND p.name = :name AND p.processed = 0
      ORDER BY p.created, p.sid', 0, 1, array(':name' => $this->name))->fetchObject();

      if ($item) {
        $update = db_update('signatures_pending_validation_queue')
          ->fields(array(
            'expire' => time() + $lease_time,
          ))
          ->condition('sid', $item->sid)
          ->condition('expire', 0);
        // If there are affected rows, this update succeeded.
        if ($update->execute()) {
          return $item;
        }
      }
      else {
        // No items exist in the queue.
        return FALSE;
      }
    }
  }

  /**
   * Delete item.
   *
   * @inheritDoc
   */
  public function deleteItem($item) {
    // Set processed flag so record can be archived.
    $update = db_update('signatures_pending_validation_queue')
      ->fields(array(
        'processed' => 1,
        'timestamp_processed_signature' => time(),
      ))
      ->condition('sid', $item->sid);
    return $update->execute();
  }

  /**
   * Release item.
   *
   * @inheritDoc
   */
  public function releaseItem($item) {
    $update = db_update('signatures_pending_validation_queue')
      ->fields(array(
        'expire' => 0,
      ))
      ->condition('sid', $item->sid);
    return $update->execute();
  }

  /**
   * Delete queue.
   * @inheritDoc
   */
  public function deleteQueue() {
    // This method is not implemented for this queue.
  }
}
