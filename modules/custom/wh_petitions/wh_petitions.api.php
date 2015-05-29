<?php

/**
 * @file
 * Hooks to support mondo to mysql migration.
 *
 * WARNING: THIS API IS GOING TO BE REMOVED BEFORE 7.x-3.0.
 * These hooks should ONLY be used to support the mongo->mysql migration.
 */

/**
 * Act when a petition has been saved to MongoDB.
 *
 * @param array $petition
 *   an array with the petition's data.
 */
function hook_wh_petitions_petition_save($petition) {
}

/**
 * Act when the body of a petition has been updated.
 *
 * @param array $petition
 *   an array with the petition's data.
 */
function hook_wh_petitions_petition_body_update($petition) {
}

/**
 * Act when a petition is being flagged as innappropirate.
 *
 * @param string $petition_id
 *   The petition's identifier.
 */
function hook_wh_petitions_petition_inappropriate($petition_id) {
}
