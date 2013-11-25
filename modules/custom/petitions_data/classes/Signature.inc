<?php

/**
 * @file
 * Defines Signature class.
 */

/**
 * Class Signature.
 */
class Signature {

  protected $id;
  protected $firstName;
  protected $lastName;
  protected $city;
  protected $state;
  protected $zip;
  protected $created;
  protected $petitionId;
  protected $petition;
  protected $user;
  protected $uid;

  /**
   * Constructs object.
   */
  public function __construct($petition_id) {
    $this->setPetitionId($petition_id);
  }

  /**
   * Sets $this->city.
   *
   * @param string $city
   *   The city of the signatory.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setCity($city) {
    $this->city = $city;

    return $this;
  }

  /**
   * Gets $this->city.
   *
   * @return string
   *   The city of the signatory.
   */
  public function getCity() {
    return petitions_data_sanitize_output($this->city);
  }

  /**
   * Sets $this->created.
   *
   * @param int $created
   *   The UNIX timestamp when this signature was created.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setCreated($created) {
    $this->created = $created;

    return $this;
  }

  /**
   * Gets $this->created.
   *
   * @return int
   *   The UNIX timestamp when this signature was created.
   */
  public function getCreated() {
    return $this->created;
  }

  /**
   * Sets $this->firstName
   *
   * @param string $firstName
   *   The first name of thet signatory.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setFirstName($firstName) {
    $this->firstName = $firstName;

    return $this;
  }

  /**
   * Gets $this->firstName.
   *
   * @return string
   *   The first name of thet signatory.
   */
  protected function getFirstName() {
    return $this->firstName;
  }

  /**
   * Sets $this->id.
   *
   * @param string $id
   *   The signature id.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setId($id) {
    $this->id = $id;

    return $this;
  }

  /**
   * Gets $this->id.
   *
   * @return string
   *   The signature id.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Sets $this->lastName.
   *
   * @param string $last_name
   *   The last name of the signatory.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setLastName($last_name) {
    $this->lastName = $last_name;

    return $this;
  }

  /**
   * Get $this->lastName.
   *
   * @return string
   *   The last name of the signatory.
   */
  protected function getLastName() {
    return $this->lastName;
  }

  /**
   * Gets $this->name.
   *
   * @return string
   *   The name of the signatory.
   *
   * @throws InvalidArgumentException.
   */
  public function getName() {
    // I'm intentionally not passing in $this->user->name because it would
    // require an unnecessary user_load() per signature.
    return $this->formatName('', $this->getFirstName(), $this->getLastName());
  }

  /**
   * Helper function to format first and last name.
   *
   * @param string $username
   *   Username of petition signer.
   * @param string $first_name
   *   First name of petition signer.
   * @param string $last_name
   *   Last name of petition signer.
   *
   * @return string
   *   Formatted name.
   */
  protected function formatName($username, $first_name, $last_name) {
    if (module_exists('petitions_signatures_display')) {
      if (variable_get('petitions_data_signatures_display_names', 0)) {
        $function = 'petitions_signatures_display_style';
        $name = petitions_data_sanitize_output($function($username, $first_name, $last_name));
      }
    }
    else {
      $first_name = petitions_data_sanitize_output($first_name);
      $last_name = petitions_data_sanitize_output($last_name);

      // Grab first letter of first and last name.
      $name = substr($first_name, 0, 1) . substr($last_name, 0, 1);
    }

    return (string) $name;
  }

  /**
   * Sets $this->state.
   *
   * @param string $state
   *   The state of the signatory.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setState($state) {
    $this->state = $state;

    return $this;
  }

  /**
   * Gets $this->state.
   *
   * @return string
   *   The state of the signatory.
   */
  public function getState() {
    return petitions_data_sanitize_output($this->state);
  }

  /**
   * Sets $this->uid.
   *
   * @param int uid
   *   The signatory user's uid.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setUid($uid) {
    $this->uid = $uid;

    return $this;
  }

  /**
   * Gets $this->uid.
   *
   * @return int
   *   The signatory user's uid.
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * Sets $this->user.
   *
   * @param object $user
   *   The signatory user. Either $user->name or $user->uid must be set.
   *
   * @return Signature
   *   Returns current instance of object.
   *
   * @throws InvalidArgumentException()
   */
  public function setUser($user) {
    if (!isset($user->name) && !isset($user->uid)) {
      throw new InvalidArgumentException('$user->name and $user->uid must be set.');
    }
    $this->user = $user;

    return $this;
  }

  /**
   * Gets $this->user.
   *
   * @return obj
   *   The signatory user.
   *
   * @throws InvalidArgumentException()
   */
  public function getUser() {
    if (!$this->user) {
      if (isset($this->uid)) {
        $this->setUser(user_load($this->uid));
      }
      else {
        throw new InvalidArgumentException('$this->user is not set.');
      }
    }

    return $this->user;
  }

  /**
   * Sets $this->zip.
   *
   * @param string $zip
   *   The zip code of the signatory.
   *
   * @return Signature
   *   Returns current instance of object.
   */
  public function setZip($zip) {
    $this->zip = $zip;

    return $this;
  }

  /**
   * Get $this->zip.
   *
   * @return string
   *   The zip code of the signatory.
   */
  public function getZip() {
    return petitions_data_sanitize_output($this->zip);
  }

  /**
   * Sets $this->petitionId.
   *
   * @param string $petition_id
   *   The petition id to which this signature belongs.
   */
  public function setPetitionId($petition_id) {
    $this->petitionId = $petition_id;
  }

  /**
   * Gets $this->petitionId.
   *
   * @return string
   *   The petition id to which this signature belongs.
   */
  public function getPetitionId() {
    return $this->petitionId;
  }

  /**
   * Converts into a publicly consumable array.
   *
   * @return array
   *   An array to be used for public display.
   */
  public function __toArray() {

    $output = array(
      'id' => $this->getPetitionId(),
      'type' => 'signature',
      'name' => $this->getName(),
      'city' => $this->getCity(),
      'state' => $this->getState(),
      'zip' => $this->getZip(),
      'created' => $this->getCreated(),
    );

    return $output;
  }

}
