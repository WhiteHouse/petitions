<?php

/**
 * @file
 * Contains SignatureItem.
 */

/**
 * Class SignatureItem.
 */
class SignatureItem {

  protected $id;
  protected $ipAddress;
  protected $legacyId;
  protected $firstName;
  protected $lastName;
  protected $city;
  protected $state;
  protected $zip;
  protected $created;
  protected $petitionId;
  protected $legacyPetitionId;
  protected $petition;
  protected $user;
  protected $userAgent;
  protected $userCountry;
  protected $uid;

  /**
   * Sets $this->city.
   *
   * @param string $city
   *   The city of the signatory.
   *
   * @return $this
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
   * @return $this
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
   * Sets $this->firstName.
   *
   * @param string $first_name
   *   The first name of the signatory.
   *
   * @return $this
   */
  public function setFirstName($first_name) {
    $this->firstName = $first_name;

    return $this;
  }

  /**
   * Gets $this->firstName.
   *
   * @return string
   *   The first name of the signatory.
   */
  public function getFirstName() {
    return $this->firstName;
  }

  /**
   * Sets $this->id.
   *
   * @param string $id
   *   The signature id.
   *
   * @return $this
   */
  public function setId($id) {
    if (!empty($id)) {
      $this->id = $id;
    }

    return $this;
  }

  /**
   * Sets the IP from which the request to create the signature originated.
   *
   * @param string $ip_address
   *   An IP address.
   *
   * @throws InvalidArgumentException
   *   Throws an exception in case of an invalid argument.
   *
   * @return $this
   */
  public function setIpAddress($ip_address) {
    if (!filter_var($ip_address, FILTER_VALIDATE_IP)) {
      throw new InvalidArgumentException('Invalid IP address.');
    }
    $this->ipAddress = $ip_address;
    return $this;
  }

  /**
   * Gets the IP from which the request to create the signature originated.
   *
   * @return string|null
   *   Returns the IP address the request to create the signature originated
   *   from if set or NULL if not.
   */
  public function getIpAddress() {
    return $this->ipAddress;
  }

  /**
   * Sets $this->legacyId.
   *
   * @param string $legacy_id
   *   The signature id.
   *
   * @return $this
   */
  public function setLegacyId($legacy_id) {
    $this->legacyId = $legacy_id;

    return $this;
  }

  /**
   * Gets the canonical ID.
   *
   * If there is a legacy ID, that is used, for API backward compatibility.
   *
   * @return string
   *   The signature id.
   */
  public function getId() {
    if (!is_null($this->legacyId)) {
      return $this->legacyId;
    }
    else {
      return $this->id;
    }
  }

  /**
   * Gets the legacy ID.
   *
   * @return string|null
   *   Returns the signature's legacy ID if set or NULL if not.
   */
  protected function getLegacyId() {
    return (!empty($this->legacyId)) ? $this->legacyId : NULL;
  }

  /**
   * Gets $this->legacyId.
   *
   * @return string
   *   Returns the signature's legacy ID if set or an empty string if not.
   */
  public function getEntityId() {
    return (!empty($this->id)) ? $this->id : '';
  }

  /**
   * Sets $this->lastName.
   *
   * @param string $last_name
   *   The last name of the signatory.
   *
   * @return $this
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
  public function getLastName() {
    return $this->lastName;
  }

  /**
   * Gets $this->name.
   *
   * @return string
   *   The name of the signatory.
   */
  public function getName() {
    return $this->formatName($this->getFirstName(), $this->getLastName());
  }

  /**
   * Formats a name.
   *
   * @param string $first_name
   *   The first name.
   * @param string $last_name
   *   The first name.
   *
   * @return string
   *   Returns the formatted name.
   */
  protected function formatName($first_name, $last_name) {
    // Use a display style, if available.
    if (variable_get('petitions_data_signatures_display_names', 0)) {
      $name = petitions_data_sanitize_output(petitions_signatures_display_style($first_name, $last_name));
    }
    // Otherwise use first and last initials (e.g., "JD" for "John Doe").
    else {
      $first_initial = strtoupper(substr(petitions_data_sanitize_output($first_name), 0, 1));
      $last_initial = strtoupper(substr(petitions_data_sanitize_output($last_name), 0, 1));
      $name = $first_initial . $last_initial;
    }

    return (string) $name;
  }

  /**
   * Sets $this->state.
   *
   * @param string $state
   *   The state of the signatory.
   *
   * @return $this
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
   * @param int $uid
   *   The signatory user's uid.
   *
   * @return $this
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
   * Sets the user agent used to create the signature.
   *
   * @param string $user_agent
   *   A string corresponding to the User-Agent header sent with the request to
   *   create the signature.
   *
   * @throws InvalidArgumentException
   *   Throws an exception in case of an invalid argument.
   *
   * @return $this
   */
  public function setUserAgent($user_agent) {
    if (!is_string($user_agent)) {
      throw new InvalidArgumentException('Invalid user agent.');
    }
    $this->userAgent = $user_agent;
    return $this;
  }

  /**
   * Gets the user agent used to create the signature.
   *
   * @return string|null
   *   Returns a string corresponding to the User-Agent header sent with the
   *   request to create the signature if set or NULL if not.
   */
  public function getUserAgent() {
    return $this->userAgent;
  }

  /**
   * Sets the country of residence of the signatory.
   *
   * @param string $user_country
   *   The signatory's country of residence.
   *
   * @throws InvalidArgumentException
   *   Throws an exception in case of an invalid argument.
   *
   * @return $this
   */
  public function setUserCountry($user_country) {
    if (!is_string($user_country)) {
      throw new InvalidArgumentException('Invalid user country.');
    }
    $this->userCountry = $user_country;
    return $this;
  }

  /**
   * Gets the country of residence of the signatory.
   *
   * @return string
   *   Returns the signatory's country of residence if set or NULL if not.
   */
  public function getUserCountry() {
    return $this->userCountry;
  }

  /**
   * Sets $this->user.
   *
   * @param object $user
   *   The signatory user. Either $user->name or $user->uid must be set.
   *
   * @return $this
   *
   * @throws InvalidArgumentException()
   *   Throws an exception in case of missing user name or UID variables.
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
   * @return object
   *   The signatory user.
   *
   * @throws InvalidArgumentException()
   *   Throws an exception in case of missing user object.
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
   * @return $this
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
   *
   * @return $this
   */
  public function setPetitionId($petition_id) {
    // This test is necessary during the Mongo-to-MySQL transition to avoid
    // retrofitting legacy client code to know the difference between entity IDs
    // and legacy IDs.
    if (petition_is_legacy_id($petition_id)) {
      $this->legacyPetitionId = $petition_id;
    }
    else {
      $this->petitionId = $petition_id;
    }
    return $this;
  }

  /**
   * Sets $this->legacyPetitionId.
   *
   * @param string $legacy_petition_id
   *   The petition id to which this signature belongs.
   *
   * @return $this
   */
  public function setLegacyPetitionId($legacy_petition_id) {
    $this->legacyPetitionId = $legacy_petition_id;
    return $this;
  }

  /**
   * Gets the canonical ID of the petition.
   *
   * If there is a legacy petition ID, that is used, for API backward
   * compatibility.
   *
   * @param bool $legacy_primary
   *   When TRUE, will return legacyPetitionId first otherwise fallback to petitionId if null.
   *   When FALSE, will return petitionId first otherwise fallback to legacyPetitionId if null.
   *
   * @return string
   *   The petition id to which this signature belongs.
   */
  public function getPetitionId($legacy_primary = TRUE) {
    if ($legacy_primary) {
      if (!is_null($this->legacyPetitionId)) {
        return $this->legacyPetitionId;
      }
      else {
        return $this->petitionId;
      }
    }
    else {
      if (!is_null($this->petitionId)) {
        return $this->petitionId;
      }
      else {
        return $this->legacyPetitionId;
      }
    }
  }

  /**
   * Gets the legacy petition ID.
   *
   * @return string
   *   Returns the signature's legacy petition ID if set or an empty string if
   *   not.
   */
  public function getLegacyPetitionId() {
    return (!empty($this->legacyPetitionId)) ? $this->legacyPetitionId : '';
  }

  /**
   * Gets the petition node ID (nid).
   *
   * @return int
   *   Returns the signature's petition node ID (nid) if set or 0 if not.
   */
  protected function getPetitionEntityId() {
    return (!empty($this->petitionId)) ? $this->petitionId : 0;
  }

  /**
   * Gets the signature in the form of a REST response item array.
   *
   * @return array
   *   An array as used by the REST API.
   */
  public function toRestResponseItemArray() {

    $output = array(
      'id' => $this->getId(),
      'type' => 'signature',
      'petitionId' => $this->getPetitionId(),
      'name' => $this->getName(),
      'city' => $this->getCity(),
      'state' => $this->getState(),
      'zip' => $this->getZip(),
      'created' => $this->getCreated(),
    );

    return $output;
  }

  /**
   * Creates a Signature entity from the object.
   *
   * @return Signature
   *   Returns a Signature entity created from the object.
   */
  public function toEntity() {
    $values = array(
      'id' => $this->getEntityId(),
      'uid' => $this->getUid(),
      'petition_id' => $this->getPetitionEntityId(),
      'user_first_name' => $this->getFirstName(),
      'user_last_name' => $this->getLastName(),
      'legacy_id' => $this->getLegacyId(),
      'legacy_petition_id' => $this->getLegacyPetitionId(),
      'timestamp' => $this->getCreated(),
      'user_agent' => $this->getUserAgent(),
      'ip_address' => $this->getIpAddress(),
      'user_city' => $this->getCity(),
      'user_state' => $this->getState(),
      'user_zip' => $this->getZip(),
      'user_username' => $this->getUser()->name,
      'user_country' => $this->getUserCountry(),
    );
    // If it already has an entity ID, it's an UPDATE not an INSERT.
    if ($this->getEntityId()) {
      $values['is_new'] = FALSE;
    }
    return entity_create('signature_mail', $values);
  }

}
