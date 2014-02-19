<?php
/**
 * @file
 * Signature Class
 */

/**
 * Description of Signature.
 */
abstract class Signature {
  // The signature name.
  private $name;
  private $schema;
  private $property_info;

  /**
   * Constructor.
   */
  public function __construct($name) {
    $this->name = $name;
    $this->setBasicSchema();
    $this->addPropertiesToSchema();
    $this->setBasicPropertyInfo();
    $this->addPropertiesInfo();
  }

  /**
   * Getter.
   */
  abstract public function getLabel();

  /**
   * Getter.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Default signature properties.
   */
  private function setBasicSchema() {
    // Schema for the signature table.
    $this->schema = array(
      'description' => "The base table for signature storage",
      'fields' => array(
        'id' => array(
          'description' => 'Signature ID',
          'type' => 'serial',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'uid' => array(
          'description' => 'Authored by (uid)',
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'petition_id' => array(
          'description' => 'ID of the petition node that this signature belongs to',
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'user_first_name' => array(
          'description' => 'User\'s First name',
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
        'user_last_name' => array(
          'description' => 'User\'s Last name',
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
        ),
      ),
      'primary key' => array('id'),
    );
  }

  /**
   * Property metadata alters for our default properties.
   */
  private function setBasicPropertyInfo() {
    $this->property_info = array(
      // Let the system know that the uid property is a user.
      'uid' => array('type' => 'user'),
      // Let the system know that the petition_id property is a node.
      'petition_id' => array('type' => 'node'),
    );
  }

  /**
   * Define more properties.
   */
  abstract public function addPropertiesToSchema();

  /**
   * Add extra property info.
   */
  abstract public function addPropertiesInfo();

  /**
   * Add an extra property to the schema.
   *
   * @param string $property_name
   *   the name of the property
   * @param array $info
   *   an array with information about the property: type, null, default, size,
   *   etc, This info matches field information passed to a schema.
   */
  protected function addPropertyToSchema($property_name, $info) {
    $this->schema['fields'][$property_name] = $info;
  }

  /**
   * Add new data about a property.
   *
   * The available info that can be set is determined by the
   * entity_property_info hook defined in the entity module.
   *
   * @param string $property_name
   *   the name of the property
   * @param string $key
   *   which parameter do you want to manipulate: the type for example
   * @param mixed $value
   *   the valie to wich the key should be changed
   */
  protected function addPropertyInfo($property_name, $key, $value) {
    $this->property_info[$property_name][$key] = $value;
  }

  /**
   * Getter.
   */
  public function getSchema() {
    return $this->schema;
  }

  /**
   * Getter.
   */
  public function getPropertiesInfo() {
    return $this->property_info;
  }
}
