<?php
/**
 * @file
 * Class with the meat for content generation of signatures and petitions in
 * MongoDB and MySQL.
 */

/**
 * Generate a new user for each signature generated.
 */
define("GENERATE_MONGO2MYSQL_PETSIG_USER_MODE_GENERATE", 0);

/**
 * Attach all signatures to Admin.
 */
define("GENERATE_MONGO2MYSQL_PETSIG_USER_MODE_ADMIN", 1);

class PetitionContentGenerator {
  private $faker;
  private $db_type;
  private $content_type;
  private $petition_count;

  private $user_mode;

  /**
   * Constructor.
   */
  public function __construct(Faker\Generator $faker, $db_type, $content_type,
          $user_mode = GENERATE_MONGO2MYSQL_PETSIG_USER_MODE_GENERATE) {
    $this->faker = $faker;
    $this->setDatabaseType($db_type);
    $this->setContentType($content_type);
    $this->petition_count = 0;

    $this->user_mode = $user_mode;
  }

  /**
   * Setter.
   */
  private function setDatabaseType($db_type) {
    if (strcmp($db_type, "mongodb") != 0 && strcmp($db_type, "mysql") != 0) {
      throw new Exception('the first argument should be either "mongodb" or "mysql".');
    }

    $this->db_type = $db_type;
  }

  /**
   * Setter.
   */
  private function setContentType($content_type) {
    if (strcmp($content_type, "petition") != 0 &&
            strcmp($content_type, "signature") != 0) {
      throw new Exception('the second argument should be either "signature" or "petition".');
    }

    $this->content_type = $content_type;
  }

  /**
   * Public method to generate content given a configuration.
   */
  public function generate() {
    // Here we want to have a 2 step process, first we generate the dummy data.
    $dummy = array();
    if (strcmp($this->content_type, 'petition') == 0) {
      $dummy = $this->getPetitionDummyData();
    }
    elseif (strcmp($this->content_type, 'signature') == 0) {
      $dummy = $this->getSignatureDummyData();
    }

    // Then we want to turn that data and save it as an entity for the mysql
    // process, or with queries for the mongodb scenarion.
    if (strcmp($this->db_type, 'mysql') == 0) {
      return $this->saveMySQLDummyData($dummy);
    }
    elseif (strcmp($this->db_type, 'mongodb') == 0) {
      return $this->saveMongoDBDummyData($dummy);
    }
  }

  /**
   * Generate dummy data for signatures.
   */
  private function getSignatureDummyData() {
    $data = array();
    $faker = $this->faker;

    $user = $this->selectDrupalUser();

    $petition_id = $this->selectRandomPetitionID();

    // Even though we don't need these for mongo, we want to keep the random
    // number generator in sync.
    $fake_legacy_petition_id = $faker->sha1;
    $fake_legacy_id = $faker->sha1;

    if ($this->db_type == 'mongodb') {
      $data['petition_id'] = $petition_id;
    }
    else {
      $data['petition_id'] = $petition_id;

      // Generated petitions do not have a treal legacy_id.
      $data['legacy_petition_id'] = $fake_legacy_petition_id;
      // Since this signature is being generated and is not the result of a
      // migration, we do not have a real legacy_id.
      $data['legacy_id'] = $fake_legacy_id;
    }
    $data['uid'] = $user->uid;
    $data['timestamp'] = $faker->unixTime;
    $data['ip_address'] = $faker->ipv4;
    $data['user_agent'] = $faker->userAgent;
    $data['number'] = $faker->randomDigitNotNull;

    $data['first_name'] = $faker->firstName;
    $data['last_name'] = $faker->lastName;
    $data['city'] = $faker->city;
    $data['state'] = $faker->state;
    $data['zip'] = $faker->postcode;
    $data['username'] = $faker->userName;
    $data['country'] = $faker->country;

    return $data;
  }

  /**
   * Generate dummy data for Petitions.
   */
  private function getPetitionDummyData() {
    $data = array();
    $faker = $this->faker;

    // Retrieve admin settings.
    $signature_public_threshold = variable_get('wh_petitions_public_signatures', 20);
    $signature_threshold = variable_get('wh_petitions_review_signatures', 500);
    $review_timeframe = variable_get('wh_petitions_review_timeframe', 30);

    $data['type'] = 'petition';
    $data['title'] = $faker->sentence(5);
    $data['body'] = $faker->paragraph(5);
    $data['uid'] = 1;
    $data['status'] = 1;
    $data['field_petition_issues'] = $faker->randomNumber(1, 40);
    $data['field_petition_user_tags'] = 1;
    $data['field_timestamp_published'] = $faker->unixTime;
    $data['field_timestamp_reached_public'] = $faker->unixTime;
    $data['field_timestamp_reached_ready'] = $faker->unixTime;
    $data['field_petition_signature_count'] = $faker->randomDigitNotNull;
    $data['field_petition_response_sign'] = $signature_threshold;
    $data['field_petition_public_signatures'] = $signature_public_threshold;
    $data['field_petition_featured'] = $faker->randomNumber(0, 1);
    $data['field_petition_hidden'] = 0;
    $data['field_petition_status'] = WH_PETITION_STATUS_PUBLIC;
    $data['field_response_status'] = WH_PETITION_RESPONSE_STATUS_UNANSWERED;
    $data['field_petition_review_timeframe'] = $review_timeframe;

    return $data;
  }

  /**
   * Generate a Drupal user.
   */
  private function createDrupalUser() {
    // Both this fields need to be unique, so if we find a user with them
    // already, lets return that.
    // we generate both things at once to keep the random number generator in
    // sync (behavior could change if user has already been created vs not).
    $name = $this->faker->userName;
    $mail = $this->faker->safeEmail;

    $user = user_load_by_name($name);
    if ($user) {
      return $user;
    }

    $user = user_load_by_mail($mail);
    if ($user) {
      return $user;
    }

    $edit = array();
    $edit['name'] = $name;
    $edit['mail'] = $mail;
    $edit['pass'] = user_password();
    $edit['status'] = 1;

    return user_save(drupal_anonymous_user(), $edit);
  }

  /**
   * Get a Drupal user.
   */
  private function selectDrupalUser() {
    // We have two modes to generate users:
    // 1) Generate a user every time.
    if ($this->user_mode == GENERATE_MONGO2MYSQL_PETSIG_USER_MODE_GENERATE) {
      return $this->createDrupalUser();
    }
    // 2) Always assign signature to admin.
    elseif ($this->user_mode == GENERATE_MONGO2MYSQL_PETSIG_USER_MODE_ADMIN) {
      return user_load(1);
    }
  }

  /**
   * Save dummy data to mysql.
   */
  private function saveMySQLDummyData($data) {
    // First we set the entity type.
    if (strcmp($this->content_type, 'signature') == 0) {
      $entity_type = "signature_mail";
    }
    else {
      $entity_type = "node";
    }

    $entity = entity_create($entity_type, array());

    foreach ($data as $property => $value) {
      // We have to handle fields differently.
      if (substr_count($property, "field") > 0 ||
              strcmp($property, "body") == 0) {

        // And we have to handle term references and entity references
        // differently also.
        if (strcmp($property, "field_petition_issues") == 0 ||
                strcmp($property, 'field_petition_user_tags') == 0) {
          $entity->{$property}[LANGUAGE_NONE][0]['tid'] = $value;
        }
        elseif (strcmp($property, "field_petition_related_petitions") == 0) {
          $entity->{$property}[LANGUAGE_NONE][0]['target_id'] = $value;
        }
        else {
          $entity->{$property}[LANGUAGE_NONE][0]['value'] = $value;
        }
      }
      // The user properties are not a straight match.
      elseif (in_array($property, array('first_name', 'last_name', 'city', 'state',
          'zip', 'username', "country"))) {
        $property_name = "user_" . $property;
        $entity->{$property_name} = $value;
      }
      else {
        $entity->{$property} = $value;
      }
    }

    entity_save($entity_type, $entity);
    return entity_id($entity_type, $entity);
  }

  /**
   * Save dummy data to MongoDB.
   */
  private function saveMongoDBDummyData($data) {
    switch ($this->content_type) {
      case 'signature':
        $return = $this->saveMongoSignature($data);
        break;

      case 'petition':
        $return = $this->saveMongoPetition($data);
        break;
    }
    return $return;
  }

  /**
   * Save dummy data for petitions in MongoDB.
   */
  private function saveMongoPetition($data) {

    $conn = wh_petitions_mongo_petition_connection();

    $petition = array(
      'uid'               => $data['uid'],
      'title'             => $data['title'],
      'body'              => $data['body'],
      'response_id'       => 0,
      'issues'            => array($data['field_petition_issues']),
      'user_tags'         => array($data['field_petition_user_tags']),
      'private_tags'      => array(),
      'related_petitions' => array(),
      'petition_status'   => $data['field_petition_status'],
      'response_status'   => $data['field_response_status'],
      'published'         => $data['field_timestamp_published'],
      'reached_public'    => $data['field_timestamp_reached_public'],
      'reached_ready'     => $data['field_timestamp_reached_ready'],
      'closed'            => 0,
      'signature_count'   => $data['field_petition_signature_count'],
      'abuse_flags'       => array(),
      'abuse_count'       => rand(0, 5000),
      'review_timeframe'  => $data['field_petition_review_timeframe'],
      'response_signatures' => $data['field_petition_response_sign'],
      'public_signatures' => $data['field_petition_public_signatures'],
      'featured'          => $data['field_petition_featured'],
      'bookmarked'        => array(),
      'hidden'            => $data['field_petition_hidden'],
    );

    wh_petitions_generate_nice_url($petition);

    $petition_id = wh_petitions_save_petition($conn, $petition);

    return $petition_id;
  }

  /**
   * Returns a random user ID.
   */
  protected function selectRandomUserID() {
    $query = db_select("users", "n");
    $query->fields("n", array('uid'));
    $query->orderRandom();
    $query->range(0, 1);

    $results = $query->execute();
    foreach ($results as $result) {
      return $result->uid;
    }
    return 1;
  }

  /**
   * Returns a petitions id from the present ones, or 0 if non exist.
   */
  protected function selectRandomPetitionID() {
    // Instead of random, we are going to do an even spread across all
    // petitions, the pseudo random element will come from ordering the
    // petitions by published date which is randomly generated.
    $petition_id = NULL;

    // If we don't have any petitions, this could get us into an infinite loop
    // so lets kee that from happening.
    $first_attempt = TRUE;

    while (!isset($petition_id)) {
      if (strcmp($this->db_type, "mongodb") == 0) {
        $collection = mongodb_collection('petitions');
        $results = $collection
        ->find()
        ->skip($this->petition_count)
        ->limit(1)
        ->sort(array('published' => 1));

        foreach ($results as $result) {
          $petition_id = (string) $result['_id'];
        }
      }
      else {
        $query = new EntityFieldQuery();
        $query->entityCondition("entity_type", "node");
        $query->entityCondition('bundle', "petition");
        $query->fieldOrderBy("field_timestamp_published", "value");
        $query->range($this->petition_count, 1);

        $results = $query->execute();

        if (!empty($results)) {
          foreach ($results['node'] as $nid => $info) {
            $petition_id = $nid;
          }
        }
      }

      // So next time we want to get the next petition in our set.
      $this->petition_count++;

      // If we did not get a petitions id, it means that we reached the end of
      // the list or that we don't have any petitions, lets deal with both
      // cases.
      // If this is our second attempt, it means that we have no petitions.
      if (!isset($petition_id) && !$first_attempt) {
        $petition_id = 0;
      }

      // This is the first attempt, maybe we are at the end of the list
      // so lets reset the counter to start at the begining.
      elseif (!isset($petition_id)) {
        $this->petition_count = 0;
        $first_attempt = FALSE;
      }
    }

    return $petition_id;
  }

  /**
   * Save dummy data in MongoDB.
   */
  private function saveMongoSignature($data) {

    $conn = wh_petitions_mongo_petition_signatures_connection();

    // User signs their own.
    $signature = array(
      'petition_id' => $data['petition_id'],
      'uid' => $data['uid'],
      'timestamp' => $data['timestamp'],
      'comment' => "",
      'abuse_flags' => array(),
      'status' => 1,
      'number' => $data['number'],
      'ip_address' => $data['ip_address'],
      'user_agent' => $data['user_agent'],
      'user' => array(
        'first_name' => $data['first_name'],
        'last_name' => $data['last_name'],
        'city' => $data['city'],
        'state' => $data['state'],
        'zip' => $data['zip'],
        'username' => $data['username'],
        'country' => $data['country'],
      ),
    );

    return wh_petitions_save_signature($signature, $conn);
  }
}
