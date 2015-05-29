<?php

/**
 * @file
 * Functions specific to the mongo implementation of the Petiton module.
 *
 * @todo This will likely be removed once mongo support is no longer needed.
 *   Be sure to strip out shunt default setting in petition.install
 */

/**
 * Function to load an individual mongo petition from a URL.
 *
 * @param string $url
 *   URL for the current petition.
 */
function petition_load_mongo2mysql_from_url($url) {
  $conn = wh_petitions_mongo_petition_connection();
  $petition = $conn->findOne(array('nice_url' => $url), array('title'));
  return $petition;
}
