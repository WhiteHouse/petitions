<?php
/**
 * @file
 * Factory class for Petitions queries.
 */

class PetitionsRawFactory {

  /**
   * Instantiate and return a new instance of IPetitionsRaw.
   *
   * @return IPetitionsRaw
   *   New instance of IPetitionsRaw.
   */
  public static function create() {
    return new PetitionsMongoRaw();
  }
}
