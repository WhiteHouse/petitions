<?php
/**
 * @file
 * Interface for backend queries for a single Petition.
 */

interface IPetitionsRetrieveRaw {

  /**
   * Primary processing function of class. Makes query to data backend.
   *
   * @param string $id
   *   Unique identifier of the requested petition.
   *
   * @return array
   *   Results of query to data backend.
   */
  public function load($id);

  /**
   * Get accessor for $this->result.
   *
   * @return array
   *   Results of query to data backend.
   */
  public function result();

}
