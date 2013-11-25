<?php
/**
 * @file
 * Factory class for Petitions queries.
 */

/**
 * Class SignaturesQueryFactory.
 */
class SignaturesSelectQueryFactory {

  /**
   * Instantiate and return a new instance of query class.
   *
   * @return SignaturesSelectQueryMongo
   *   New instance of SignaturesSelectQueryMongo.
   */
  public static function create() {
    return new SignaturesSelectQueryMongo();
  }
}
