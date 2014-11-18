<?php
/**
 * @file
 * Implementation of the Signature abstract class.
 */

module_load_include("php", "signature", "Signature");

/**
 * Implementation of the Signature abstract class.
 */
class SignatureMail extends Signature {

  /**
   * Constructor.
   */
  public function __construct() {
    parent::__construct('mail');
  }

  /**
   * Property Info.
   */
  public function addPropertiesInfo() {
    $this->addPropertyInfo('timestamp', 'type', 'date');
  }

  /**
   * Properties.
   */
  public function addPropertiesToSchema() {
    $this->addPropertyToSchema('legacy_id',
    array(
      'description' => 'MongoDB Signature ID',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('legacy_petition_id',
    array(
      'description' => 'MongoDB ID of the petition node that this signature belongs to',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('timestamp',
    array(
      'description' => 'Date the signature was created',
      'type' => 'int',
      'not null' => TRUE,
      'default' => 0,
    ));

    $this->addPropertyToSchema('user_agent',
    array(
      'description' => 'Information about a user\'s browser',
      'type' => 'text',
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('ip_address',
    array(
      'description' => 'Ip Address',
      'type' => 'varchar',
      'length' => 45,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('user_city',
    array(
      'description' => 'City',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('user_state',
    array(
      'description' => 'State',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('user_zip',
    array(
      'description' => 'zipcode',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('user_username',
    array(
      'description' => 'Username',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));

    $this->addPropertyToSchema('user_country',
    array(
      'description' => 'Country',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
    ));
  }

  /**
   * Getter.
   */
  public function getLabel() {
    return "Signature Mail";
  }
}
