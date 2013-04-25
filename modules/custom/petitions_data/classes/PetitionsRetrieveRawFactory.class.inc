<?php
/**
 * @file
 * Factory class for Petition retrieve operation queries.
 */

class PetitionsRetrieveRawFactory {

  /**
   * Instantiate and return a new instance of iPetitionRetrieveRaw.
   *
   * @return iPetitionRetrieveRaw
   *   New instance of iPetitionRetrieveRaw.
   */
  public static function create() {
    return new PetitionsRetrieveMongoRaw();
  }
}
