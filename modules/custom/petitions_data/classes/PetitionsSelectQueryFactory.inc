<?php
/**
 * @file
 * Factory class for Petitions select queries.
 */

class PetitionsSelectQueryFactory {

  /**
   * Instantiate and return a new instance of query class.
   *
   * @return PetitionsQueryMongo
   *   New instance of PetitionsQueryMongo.
   */
  public static function create() {
    return new PetitionsSelectQueryMongo();
  }
}
