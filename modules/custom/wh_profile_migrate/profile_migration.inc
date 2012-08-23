<?php

/*
 *migration class to test import of various date fields.
 */
class ProfileMigration extends Migration {
  public $destinationDBName = 'petitions';
  public $sourceDBName = 'default';
  public function __construct() {
    parent::__construct();
    $this->description = t('Migration of user profiles from shared_users to D7');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationProfile2::getKeySchema()
    );
 
    // We are getting data from Profile table from the shared database so create connection and set up a query for this data. 

    $query = db_select('profile_values', 'pv');
    $query->addField('pv', 'uid');
    
    // First Name
    $query->leftJoin('profile_values', 'pv1', 'pv1.uid = pv.uid AND pv1.fid = 1');
    $query->addField('pv1', 'value', 'profile_first_name');

    // Last Name
    $query->leftJoin('profile_values', 'pv2', 'pv2.uid = pv.uid AND pv2.fid = 2');
    $query->addField('pv2', 'value', 'profile_last_name');

    // City
    $query->leftJoin('profile_values', 'pv7', 'pv7.uid = pv.uid AND pv7.fid = 7');
    $query->addField('pv7', 'value', 'profile_city');

    // State
    $query->leftJoin('profile_values', 'pv8', 'pv8.uid = pv.uid AND pv8.fid = 8');
    $query->addField('pv8', 'value', 'profile_state');

    // Zip
    $query->leftJoin('profile_values', 'pv9', 'pv9.uid = pv.uid AND pv9.fid = 9');
    $query->addField('pv9', 'value', 'profile_zip');

    // Country
    $query->leftJoin('profile_values', 'pv10', 'pv10.uid = pv.uid AND pv10.fid = 10');
    $query->addField('pv10', 'value', 'profile_country');

    // Join to Users so we can users Login as highwaterField
    $query->leftJoin('users', 'u', 'u.uid = pv.uid');
    $query->addField('u', 'access');

    $query->distinct();

    $query->orderBy('u.access');

    $this->highwaterField = array(
      'name'  => 'access',
      'alias' => 'u',
      'type'  => 'int',
    );

    $this->source = new MigrateSourceSQL($query, array(), NULL, array('map_joinable' => FALSE));
    
    // Cache the source counts so we don't kill the admin UI the first time we try to load the migration
    // $this->source->cacheCount = TRUE;

    $this->destination = new MigrateDestinationProfile2("main");

    $this->addUnmigratedDestinations(array('pid', 'language', 'path'));

    // Assign mappings TO destination fields FROM source fields.
    
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('revision_uid', 'uid');
    $this->addFieldMapping('field_first_name', 'profile_first_name');
    $this->addFieldMapping('field_last_name', 'profile_last_name');
    $this->addFieldMapping('field_city', 'profile_city');
    $this->addFieldMapping('field_state', 'profile_state');
    $this->addFieldMapping('field_zip', 'profile_zip');
    $this->addFieldMapping('field_country', 'profile_country');
    $this->addFieldMapping('is_new', '')->defaultValue(TRUE);
  }
} 
