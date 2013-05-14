<?php
/**
 * @file
 * Factory class for Petition retrieve operation queries.
 */

class PetitionsRetrieveRawFactory {

  /**
   * Instantiate and return a new instance of IPetitionsRetrieveRaw.
   *
   * @return IPetitionsRetrieveRaw
   *   New instance of IPetitionsRetrieveRaw.
   */
  public static function create() {
    return new PetitionsRetrieveMongoRaw();
  }
}
