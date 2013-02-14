<?php
/**
 * @file
 * Factory class for Petitions queries.
 */

class PetitionLoadRawFactory {

  /**
   * Instantiate and return a new instance of iPetitionsRaw.
   *
   * @return iPetitionsRaw
   *   New instance of iPetitionsRaw.
   */
  public static function create() {
    return new PetitionLoadMongoRaw();
  }
}
