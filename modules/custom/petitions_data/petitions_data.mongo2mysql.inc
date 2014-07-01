<?php
/**
 * @file
 * This file is for phasing out legacy mongo-dependent petitions_data code.
 */

/**
 * Gets a petition.
 *
 * The petition array is cached indefinitely to prevent return trips to the
 * database for the same information.
 *
 * @todo To prevent stale data, cache entries need to be invalidated by petition
 *   ID whenever:
 *     - A petition is modified.
 *     - A new petition is created.
 *
 * @param string $petition_id
 *   A hexidecimal petition ID.
 *
 * @return array
 *   Returns an array of petition data if a petition is found (or an empty array
 *   if not) containing the following (key, value) pairs:
 *   - id: A unique identifier as a hexidecimal string.
 *   - type: The string "petition".
 *   - title: The title string.
 *   - body: The body string.
 *   - issues: An indexed array of terms from the "Issues" vocabulary that the
 *     petition is tagged with, where each value is another array with the
 *     following (key, value) pairs:
 *     - id: The term id (tid).
 *     - name: The term name string.
 *   - signatureThreshold: The number of signatures required to receive an
 *     official response.
 *   - url: The absolute path to the petition, e.g. "/petition/petition-title".
 *   - deadline: The deadline for signing the petition, as a Unix timestamp.
 *   - status: The string "open", "pending response", "responded", or "closed".
 *   - response: An array of response data if a response has been associated
 *     with the petition (or NULL if not) containing the following (key, value)
 *     pairs:
 *     - id: The response node ID (nid).
 *     - url: The absolute path to the response node, e.g.
 *       "/response/response-title".
 *     - associationTime: The date the response was associated with the
 *       petition, in the form of a Unix timestamp.
 *   - created: The date the petition was created, as a Unix timestamp.
 *
 *  The following are *not* included in the returned array because they would
 *  change too often to be reliable in the cached petition data.
 *   - signatureCount: The number of signatures the petition has received to
 *     date.
 *   - signaturesNeeded: The number of signatures still needed in order to reach
 *     the threshold.
 */
function petitions_data_mongo2mysql_get_petition($petition_id) {
  $petition = &drupal_static(__FUNCTION__ . $petition_id);
  if (!isset($petition)) {
    if ($cache = cache_get("petition:{$petition_id}", 'cache_petitions_petitions')) {
      $petition = $cache->data;
    }
    else {
      $results = PetitionsController::load($petition_id);

      // If the petition exists, return it.
      if (!empty($results[0])) {
        $petition = $results[0];
        // Unset signatureCount and signaturesNeeded because they would become
        // quickly stale and show unreliable values if included.
        unset($petition['signatureCount'], $petition['signaturesNeeded']);
      }
      // If no such petition exists, return an empty array.
      else {
        $petition = array();
      }

      // Cache the result.
      cache_set("petition:{$petition_id}", $petition, 'cache_petitions_petitions', CACHE_PERMANENT);
    }
  }
  return $petition;
}

/**
 * Original functions below.  =================================
 */

/**
 * Mongo-dependent legacy petitions_data_get_petition() function.
 *
 * @see petitions_data_mongo2mysql_get_petition()
 */
function petitions_data_get_petition($petition_id) {
  throw new Exception('petitions_data_get_petition is deprecated. Use petitions_data_mongo2mysql_get_petition($petition_id) and see petitions/ABOUT-7.x-3.x.md for information on future-proofing new code including mongo depencencies.');
}
