<?php

/**
 * @file
 * Contains SignaturesQuery.
 */

/**
 * Class SignaturesSelectQuery.
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
  protected $signatureIds;
  protected $petitionIds;
  protected $uid;
  protected $orderByFieldsMap;
  protected $orderByDirMap;

  /**
   * These constants are used by the setOrderBy function.
   * Mapping is done in SignaturesSelectQueryMysql.inc and
   * SignaturesSelectQuerySolr.inc to determine the column names
   * for each one of these constants.
   */
  const SELECT_QUERY_ORDER_BY_FIELD_TIMESTAMP = 'TIMESTAMP';

  /**
   * Valid signature fields for order by
   *
   * NOTE: Adding any additional fields requires adding of indexes to database.
   * Use of a new field without indexes will cause ADVERSE performance issues.
   *
   * @var array
   */
  protected $validSignaturesOrderByFields = array(
    self::SELECT_QUERY_ORDER_BY_FIELD_TIMESTAMP,
  );

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct();
    // Merge signature specific order by fields with common order by fields from SelectQueryBase.
    $this->setValidOrderByFields(array_merge($this->getValidOrderByFields(), $this->getValidSignaturesOrderByFields()));
  }

  /**
   * Getter accessor for $this->validSignaturesOrderByFields.
   *
   * @return array
   *   The array of validSignaturesOrderByFields
   */
  public function getValidSignaturesOrderByFields() {
    return $this->validSignaturesOrderByFields;
  }

  /**
   * Get accessor for $this->orderByFieldsMap.
   *
   * @return array
   *   Array of orderByFieldsMap
   */
  public function getOrderByFieldsMap() {
    return $this->orderByFieldsMap;
  }

  /**
   * Get accessor for $this->orderByDirMap.
   *
   * @return array
   *   Array of orderByDirMap
   */
  public function getOrderByDirMap() {
    return $this->orderByDirMap;
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
   * @return $this
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
   * @return $this
   */
  public function setState($state) {
    $this->state = $state;

    return $this;
  }

  /**
   * Get accessor for $this->uid.
   *
   * @return int
   *   A user UID if set or FALSE if not.
   */
  protected function getUid() {
    return $this->uid;
  }

  /**
   * Set accessor for $this->uid.
   *
   * @param int $uid
   *   A user UID.
   *
   * @return $this
   */
  public function setUid($uid) {
    if (!empty($uid)) {
      $this->uid = (int) $uid;
    }

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
   * @return $this
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
   * @return $this
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
   * @return $this
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
   * @return $this
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
   * @return $this
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
   *   An array of petition IDs.
   *
   * @return $this
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
   * @return $this
   */
  public function setPetitionId($pid) {
    $this->setPetitionIds(array($pid));

    return $this;
  }

  /**
   * Set accessor for $this->signatureIds.
   *
   * @param array $sids
   *   An array of signature IDs.
   *
   * @return $this
   */
  public function setSignatureIds(array $sids) {
    $this->signatureIds = $sids;

    return $this;
  }

  /**
   * Get accessor for $this->signatureIds.
   *
   * @return array
   *   Array of signatureIds.
   */
  public function getSignatureIds() {
    return $this->signatureIds;
  }

  /**
   * Pass through to set $this->signatureIds.
   *
   * @param string $sid
   *   A signature id.
   *
   * @return $this
   */
  public function setSignatureId($sid) {
    $this->setSignatureIds(array($sid));

    return $this;
  }
}
