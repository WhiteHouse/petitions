<?php
/**
 * @file
 * Factory class for Petitions queries.
 */

class PetitionsIndexRawFactory {

  /**
   * Instantiate and return a new instance of IPetitionsRaw.
   *
   * @return IPetitionsRaw
   *   New instance of IPetitionsRaw.
   */
  public static function create() {
    return new PetitionsIndexMongoRaw();
  }
}
