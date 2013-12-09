<?php

/**
 * @file
 * Defines SignaturesQuery class.
 */

/**
 * Class SignaturesQuery.
 *
 * This should contain methods for querying signatures that are NOT specific to
 * a backend, e.g., MongoDB.
 */
abstract class SignaturesSelectQuery extends SelectQueryBase {

  protected $city;
  protected $state;
  protected $zipcode;
  protected $country;
  protected $startDate;
  protected $endDate;
  protected $createdDate;
  protected $petitionIds;

  /**
   * Constructor. This must be explicitly called by subclasses.
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Get accessor for $this->city.
   *
   * @return string
   *   City name for signature.
   */
  protected function getCity() {
    return $this->city;
  }

  /**
   * Set accessor for $this->city.
   *
   * @param string $city
   *   City name for signature.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setCity($city) {
    $this->city = $city;

    return $this;
  }

  /**
   * Get accessor for $this->state.
   *
   * @return string
   *   State name for signature.
   */
  protected function getState() {
    return $this->state;
  }

  /**
   * Set accessor for $this->state.
   *
   * @param string $state
   *   State name for signature.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setState($state) {
    $this->state = $state;

    return $this;
  }

  /**
   * Get accessor for $this->zipcode.
   *
   * @return int
   *   Postal code to identify locations within the United States.
   */
  protected function getZipCode() {
    return intval($this->zipcode);
  }

  /**
   * Set accessor for $this->zipcode.
   *
   * @param int $zipcode
   *   Postal code to identify locations within the United States.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setZipCode($zipcode) {
    if (is_numeric($zipcode)) {
      $this->zipcode = $zipcode;
    }

    return $this;
  }

  /**
   * Get accessor for $this->country.
   *
   * @return string
   *   Country name for signature.
   */
  protected function getCountry() {
    return $this->country;
  }

  /**
   * Set accessor for $this->country.
   *
   * @param string $country
   *   Country name for signature.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setCountry($country) {
    $this->country = $country;

    return $this;
  }

  /**
   * Get accessor for $this->startDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getStartDate() {
    return intval($this->startDate);
  }

  /**
   * Set accessor for $this->startDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setStartDate($date) {
    if (is_numeric($date)) {
      $this->startDate = $date;
    }

    return $this;
  }

  /**
   * Get accessor for $this->endDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getEndDate() {
    return intval($this->endDate);
  }

  /**
   * Set accessor for $this->endDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return SignaturesMongoRaw
   *   Returns current instance of object.
   */
  public function setEndDate($date) {
    if (is_numeric($date)) {
      $this->endDate = $date;
    }

    return $this;
  }

  /**
   * Set accessor for $this->createdAt.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return PetitionsQuery
   *   Returns current instance of object.
   */
  public function setCreatedAt($date) {
    if (is_numeric($date)) {
      $this->createdDate = $date;
    }
    return $this;
  }

  /**
   * Get accessor for $this->createdDate.
   *
   * @return int
   *   Epoch (UNIX style) time stamp.
   */
  protected function getCreatedDate() {
    return intval($this->createdDate);
  }

  /**
   * Set accessor for $this->petitionIds.
   *
   * @param array $pids
   *   An array of petition ids.
   *
   * @return PetitionsQuery
   *   Returns current instance of object.
   */
  public function setPetitionIds(array $pids) {
    $this->petitionIds = $pids;

    return $this;
  }

  /**
   * Get accessor for $this->petitionIds.
   *
   * @return array
   *   Array of petitionIds.
   */
  public function getPetitionIds() {
    return $this->petitionIds;
  }

  /**
   * Pass through to set $this->petitionIds.
   *
   * @param string $pid
   *   A petition ids.
   *
   * @return PetitionsQuery
   *   Returns current instance of object.
   */
  public function setPetitionId($pid) {
    $this->setPetitionIds(array($pid));

    return $this;
  }
}
