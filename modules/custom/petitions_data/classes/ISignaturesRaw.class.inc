<?php
/**
 * @file
 * Interface for backend queries for Petition signatures.
 */

interface ISignaturesRaw {

  /**
   * Set accessor for $this->city.
   *
   * @param int $city
   *   City used to filter results.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setCity($city);

  /**
   * Set accessor for $this->state.
   *
   * @param int $state
   *   State used to filter results.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setState($state);

  /**
   * Set accessor for $this->zipcode.
   *
   * @param int $zipcode
   *   Postal code to identify locations within the United States.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setZipCode($zipcode);

  /**
   * Set accessor for $this->country.
   *
   * @param string $country
   *   Country name for signature.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setCountry($country);

  /**
   * Set accessor for $this->startDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setStartDate($date);

  /**
   * Set accessor for $this->endDate.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setEndDate($date);

  /**
   * Set accessor for $this->createdAt.
   *
   * @param int $date
   *   Epoch (UNIX style) time stamp.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setCreatedAt($date);

  /**
   * Set accessor for $this->limit.
   *
   * @param int $limit
   *   Number of results to return.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setLimit($limit);

  /**
   * Set accessor for $this->offset.
   *
   * @param int $offset
   *   Number of results to skip before return.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setOffset($offset);

  /**
   * Set accessor for $this->nameFormat.
   *
   * @param string $nameFormat
   *   Function used to format name values.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setNameFormat($nameFormat);

  /**
   * Set accessor for $this->locationFormat.
   *
   * @param string $locationFormat
   *   Function used to format location values.
   *
   * @return ISignaturesRaw
   *   Returns current instance of object.
   */
  public function setLocationFormat($locationFormat);

  /**
   * Primary processing function of class. Makes query to backend.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function load($petition_id);

  /**
   * Get accessor for $this->result.
   *
   * @return array
   *   Results of query to MongoDB.
   */
  public function result();

}
