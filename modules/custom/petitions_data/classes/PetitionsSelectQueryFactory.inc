<?php

/**
 * @file
 * Contains PetitionsSelectQueryFactory.
 */

/**
 * Provides a factory for creating SignaturesSelectQuery objects.
 */
class PetitionsSelectQueryFactory {

  /**
   * Instantiate and return a petitions SELECT query class.
   *
   * @param bool $realtime
   *   Whether or not realtime accuracy is required. Realtime queries are issued
   *   against the system of record (e.g., MySQL), so their results can be
   *   depended upon to be current. Queries without realtime requirements are
   *   issued against a specialized search server with better performance and
   *   free text search capabilities but an indexing delay. Defaults to TRUE,
   *   realtime.
   *
   * @return PetitionsSelectQuery
   *   Returns an instance of the appropriate child class of
   *   PetitionsSelectQuery given the active storage backend.
   */
  public static function create($realtime = TRUE) {
    if (petitions_data_mongo_reads_are_enabled()) {
      return new PetitionsSelectQueryMongo();
    }
    elseif ($realtime || shunt_is_enabled('petitions_data_solr')) {
      return new PetitionsSelectQueryMysql();
    }
    else {
      return new PetitionsSelectQuerySolr();
    }
  }

}
