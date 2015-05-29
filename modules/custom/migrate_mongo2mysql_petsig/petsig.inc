<?php

/**
 * @file
 * Migration class definitions for signatures and petitions
 */

abstract class PetSigMigration extends Migration {

  /**
   * Create a basic migration definition for the other migration pieces.
   * 
   * @param array $arguments
   *   the arguments. 
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->team = array();
  }
}

class PetitionMigration extends PetSigMigration {

  /**
   * Defining petition's migration.
   * 
   * @param array $arguments
   *   the arguments. 
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate mongodb petition objects, to a content type petition');

    $collection = wh_petitions_mongo_petition_connection();

    $query = array();

    $include_unpublished_petitions = variable_get('migrate_mongo2mysql_pet_unpublish', 0);
    if (empty($include_unpublished_petitions)) {
      // Exclude unpublished petitions.
      $query = array(
        'petition_status' => array(
          '$nin' => array((int) WH_PETITION_STATUS_DRAFT),
        ),
      );
    }
    $options = array('track_changes' => 1);
    $this->source = new MigrateSourceMongoDB($collection, $query, array(
        '_id' => "MongoDB identifier",
        'uid' => "User ID", 'title' => "Title", 'body' => "Body",
        'response_id' => "Response ID", "issues" => "Issues",
        'related_petitions' => "Related Petitions",
        "petition_status" => "Petition Status",
        "response_status" => "Response Status",
        "published" => "Published", "reached_public" => "Reached Public",
        "reached_ready" => "Reached Ready", "signature_count" => "Signature Count",
        "abuse_flags" => "Abuse Flags", "abuse_count" => "Abuse Count",
        "review_timeframe" => "Review Timeframe",
        "response_signatures" => "Response Signature",
        "public_signatures" => "Public Signature", "short_url" => "Short URL",
        "created" => "Created", "closed" => "Closed", "nice_url" => "Legacy Path",
        "review_threshold_mail_sent" => "Review threshold email sent",

    ), array('_id' => 1), $options);

    $this->destination = new MigrateDestinationNodeModified('petition');

    $this->addSimpleMappings(array('uid', 'title', 'body', 'created'));

    $this->addFieldMapping("field_legacy_id", '_id');

    $this->addFieldMapping("field_response_id", 'response_id');
    $this->addFieldMapping("field_abuse_count", "abuse_count");
    $this->addFieldMapping("field_abuse_flags", "abuse_flags");
    $this->addFieldMapping("field_petition_issues", 'issues');
    $this->addFieldMapping("field_petition_issues:source_type")->defaultValue("tid");
    $this->addFieldMapping("field_petition_public_signatures", "public_signatures");
    $this->addFieldMapping("field_petition_related_petitions", 'related_petitions')->sourceMigration("Petition");
    $this->addFieldMapping("field_petition_response_sign", 'response_signatures');
    $this->addFieldMapping("field_petition_review_timeframe", "review_timeframe");
    $this->addFieldMapping("field_petition_signature_count", 'signature_count');
    $this->addFieldMapping("field_petition_status", 'petition_status');
    $this->addFieldMapping("field_response_status", 'response_status');
    $this->addFieldMapping("field_short_url", "short_url");
    $this->addFieldMapping("field_legacy_path", "nice_url");
    $this->addFieldMapping("field_timestamp_published", 'published');
    $this->addFieldMapping("field_timestamp_reached_public", 'reached_public');
    $this->addFieldMapping("field_timestamp_reached_ready", 'reached_ready');
    $this->addFieldMapping("field_timestamp_responded", "closed");
    $this->addFieldMapping("field_review_threshold_mail_sent", "review_threshold_mail_sent");

    $this->addUnmigratedSources(array("field_response_id"));

    $this->addUnmigratedDestinations(array(
        'changed',
        'sticky',
        'revision',
        'log',
        'language',
        'tnid',
        'translate',
        'revision_uid',
        'is_new',
        'promote',
        'body:summary',
        'body:language',
        'metatag_title',
        'metatag_description',
        'metatag_abstract',
        'metatag_keywords',
        'metatag_robots',
        'metatag_news_keywords',
        'metatag_generator',
        'metatag_copyright',
        'metatag_image_src',
        'metatag_canonical',
        'metatag_shortlink',
        'metatag_publisher',
        'metatag_author',
        'metatag_original-source',
        'comment',
        'field_petition_issues:create_term',
        'field_petition_issues:ignore_case',
        "field_short_url:language",
        "field_legacy_path:language",
    ));

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        '_id' =>
        array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'description' => 'Petition ID',
        ),
      ), MigrateDestinationNode::getKeySchema()
    );
  }
}

class SignatureMigration extends PetSigMigration {

  /**
   * Define the Signatures migration.
   * 
   * @param array $arguments
   *   the arguments.
   */
  public function __construct($arguments) {
    parent::__construct($arguments);

    $this->description = t('Migrate mongodb signature objects, to a signature entity');

    $collection = wh_petitions_mongo_petition_signatures_connection();

    $query = array();

    $include_all_signatures = variable_get('migrate_mongo2mysql_sig_all', 0);
    if (empty($include_all_signatures)) {
      // Exclude signatures created after specified date.
      $signatures_created_after = variable_get('migrate_mongo2mysql_sig_date_start', 0);
      if (!empty($signatures_created_after)) {
        $signatures_created_after = strtotime($signatures_created_after);
        $query['timestamp']['$gt'] = (int) $signatures_created_after;
      }
      // Exclude signatures created before specified date.
      $signatures_created_before = variable_get('migrate_mongo2mysql_sig_date_end', 0);
      if (!empty($signatures_created_before)) {
        $signatures_created_before = strtotime($signatures_created_before);
        $query['timestamp']['$lt'] = (int) $signatures_created_before;
      }
    }
    $this->source = new MigrateSourceMongoDB($collection, $query, array(
        '_id' => "MongoDB identifier",
        'petition_id' => "Petition ID", "uid" => "User ID",
        "timestamp" => "Created Date", 'user_agent' => "User Agent",
        'ip_address' => "Ip Address", 'user' => "User"));

    $this->destination = new MigrateDestinationSignatureEntity();

    $this->addSimpleMappings(array('uid', 'timestamp', 'user_agent',
        'ip_address'));

    $this->addFieldMapping('legacy_id', '_id');

    $this->addFieldMapping('legacy_petition_id', 'petition_id');
    $this->addFieldMapping('petition_id', 'petition_id')->sourceMigration("Petition");

    $this->addFieldMapping('user_first_name', 'user')->callbacks(array($this, 'firstName'));
    $this->addFieldMapping('user_last_name', 'user')->callbacks(array($this, 'lastName'));
    $this->addFieldMapping('user_city', 'user')->callbacks(array($this, 'city'));
    $this->addFieldMapping('user_state', 'user')->callbacks(array($this, 'state'));
    $this->addFieldMapping('user_zip', 'user')->callbacks(array($this, 'zipcode'));
    $this->addFieldMapping('user_username', 'user')->callbacks(array($this, 'username'));
    $this->addFieldMapping('user_country', 'user')->callbacks(array($this, 'country'));

    $destination_id_schema = MigrateDestinationSignatureEntity::getKeySchema();
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          '_id' =>
          array(
            'type' => 'varchar',
            'length' => 255,
            'not null' => TRUE,
            'description' => 'Mongo Signature ID',
          ),
        ), $destination_id_schema
    );
  }

  /**
   * Callback for user field mapping.
   */
  public function firstName($value) {
    return $value['first_name'];
  }

  /**
   * Callback for user field mapping.
   */
  public function lastName($value) {
    return $value['last_name'];
  }

  /**
   * Callback for user field mapping.
   */
  public function city($value) {
    return $value['city'];
  }

  /**
   * Callback for user field mapping.
   */
  public function state($value) {
    return $value['state'];
  }

  /**
   * Callback for user field mapping.
   */
  public function country($value) {
    return $value['country'];
  }

  /**
   * Callback for user field mapping.
   */
  public function zipcode($value) {
    return $value['zip'];
  }

  /**
   * Callback for user field mapping.
   */
  public function username($value) {
    return $value['username'];
  }
}
