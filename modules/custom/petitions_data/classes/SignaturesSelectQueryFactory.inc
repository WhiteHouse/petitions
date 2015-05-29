<?php

/**
 * @file
 * Contains SignaturesSelectQueryFactory.
 */

/**
 * Provides a factory for creating SignaturesSelectQuery objects.
 */
class SignaturesSelectQueryFactory {

  /**
   * Creates and returns a new SignaturesSelectQuery object.
   *
   * @param bool $realtime
   *   Whether or not realtime accuracy is required. Realtime queries are issued
   *   against the system of record (e.g., MySQL), so their results can be
   *   depended upon to be current. Queries without realtime requirements are
   *   issued against a specialized search server with better performance and
   *   free text search capabilities but an indexing delay. Defaults to TRUE,
   *   realtime.
   *
   * @return SignaturesSelectQuery
   *   Returns an instance of the appropriate child class of
   *   SignaturesSelectQuery given the active storage backend.
   */
  public static function create($realtime = TRUE) {
    if (petitions_data_mongo_reads_are_enabled()) {
      return new SignaturesSelectQueryMongo();
    }
    elseif ($realtime || shunt_is_enabled('signatures_data_solr')) {
      return new SignaturesSelectQueryMysql();
    }
    else {
      return new SignaturesSelectQuerySolr();
    }
  }
}
