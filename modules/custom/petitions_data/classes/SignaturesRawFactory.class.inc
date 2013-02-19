<?php
/**
 * @file
 * Factory class for signature queries.
 */

class SignaturesRawFactory {

  /**
   * Instantiate and return a new instance of ISignaturesRaw.
   *
   * @return ISignaturesRaw
   *   New instance of ISignaturesRaw.
   */
  public static function create() {
    return new SignaturesMongoRaw();
  }
}
