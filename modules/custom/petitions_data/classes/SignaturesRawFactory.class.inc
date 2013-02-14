<?php
/**
 * @file
 * Factory class for signature queries.
 */

class SignaturesRawFactory {

  /**
   * Instantiate and return a new instance of iPetitionsRaw.
   *
   * @return ISignaturesRaw
   *   New instance of iPetitionsRaw.
   */
  public static function create() {
    return new SignaturesMongoRaw();
  }
}
