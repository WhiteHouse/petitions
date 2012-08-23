<?php
/**
 * @file
 * Defines the GovDeliverySubscriberAPI, which handles creating Topics and assigning them to Catgories
 */

class GovDeliverySubscriberAPI {

  public $govdelivery_account_name;
  public $govdelivery_account_password;
  public $govdelivery_url_base;
  private $auth_header;

  /**
   *  Constructor
   */
  function __construct($govdelivery_account_name, $govdelivery_account_password, $govdelivery_url_base) {
    $this->govdelivery_account_name = $govdelivery_account_name;
    $this->govdelivery_account_password = $govdelivery_account_password;
    $this->govdelivery_url_base = $govdelivery_url_base;
    // Login header:
    // Authorization: Basic base64_encode('username:password');
    $this->auth_header = $this->govdelivery_account_name. ':' . $this->govdelivery_account_password;
  }

  /**
   *  Take a topic item from the queue and create it in GovDelivery
   *
   *  @param $topic Topic object
   *
   *  @return array of return object from topic creation
   */
  function create_topic($topic) {
    $url = $this->govdelivery_url_base . '/topics.xml';

    if (!isset($topic->topic_id) || is_null($topic->topic_id)) {
      throw new Exception(t('topic_id attribute of supplied topic is unset or null.  You must supply a topic_id'));
    }

    // GovDelivery Subscriber API requires al topic_id's to be be 32 characers or less
    $id_length = strlen($topic->topic_id);
    if ($id_length > 32) {
      throw new Exception(t('topic_id must be 32 characters or less. Supplied id of %id is %length characters', array('%id' => $topic->topic_id, '%length' => $id_length)));  
    }

    // GovDelivery Subscriber API requires all topic_id's to be all CAPS. 
    $topic->topic_id = strtoupper($topic->topic_id);

    $create_topic_xml_request = '
      <?xml version="1.0" encoding="UTF-8"?>
      <topic>
         <code type="string">'. $topic->topic_id  .'</code>
         <default-pagewatch-results type="integer" nil="true"></default-pagewatch-results>
         <description nil="true"></description>
         <lock-version type="integer">0</lock-version>
         <name>'. $topic->title  .'</name>
         <pagewatch-autosend type="boolean">false</pagewatch-autosend>
         <pagewatch-enabled type="boolean">false</pagewatch-enabled>
         <pagewatch-suspended type="boolean">false</pagewatch-suspended>
         <rss-feed-description nil="true"></rss-feed-description>
         <rss-feed-title>Topic Name - Recent Updates</rss-feed-title>
         <rss-feed-url>http://url_for_rss_feed.rss</rss-feed-url>
         <send-by-email-enabled type="boolean">false</send-by-email-enabled>
         <short-name>'. $topic->short_title .'</short-name>
         <subscribers-count type="integer">0</subscribers-count>
         <wireless-enabled type="boolean">false</wireless-enabled>
         <pages type="array">
         <page>
         <url>http://www.govdelivery.com</url>
         </page>
         </pages>
         <visibility>2</visibility>
         </topic>';

    // Set options
    $topic_options = array(
      CURLOPT_URL => $url,
      CURLOPT_FAILONERROR => TRUE,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => 120,
      CURLOPT_USERPWD => $this->auth_header,
      CURLOPT_HEADER => TRUE,
      CURLOPT_HTTPHEADER => array('Content-Type: application/xml',),
      CURLOPT_POST => TRUE,
      CURLOPT_POSTFIELDS => $create_topic_xml_request,
    );

    $ch = curl_init();    // create curl resource
    curl_setopt_array($ch, $topic_options);

    $topic_return = new stdClass;
    $topic_return->response = curl_exec($ch); // execute and get response
    $topic_return->header   = substr($topic_return->response, 0, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $topic_return->body     = substr($topic_return->response, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $topic_return->error    = curl_error($ch);
    $topic_return->info     = curl_getinfo($ch);
    $topic_return->topic_id = $topic->topic_id;

    return $topic_return;
  }

  /**
   *  Update the GovDelivery category associated with the given topic
   *
   *  @param $topic  Topic object
   *
   *  @return array of return object from category updating
   */
  function update_topic_category($topic) { 
    if (!isset($topic->topic_id) || is_null($topic->topic_id)) {
      throw new exception(t('topic_id attribute of supplied topic is unset or null.  you must supply a topic_id'));
    }

    if (!isset($topic->category_id) || is_null($topic->category_id)) {
      throw new exception(t('category_id attribute of supplied topic is unset or null.  you must supply a category_id'));
    }

    $url = $this->govdelivery_url_base . '/topics/' . $topic->topic_id . '/categories.xml';
    $set_category_xml_request = '
        <?xml version="1.0" encoding="UTF-8"?>
        <topic>
           <categories type="array">
            <category>
              <code>' . $topic->category_id . '</code>
            </category>
          </categories>
        </topic>';

    //write the request out to a tmp file since PUT isn't happy taking a string
    $fp = fopen('php://memory', 'w+');
    fwrite($fp, $set_category_xml_request);
    fseek($fp, 0);

    // Set options 
    $category_options = array(
      CURLOPT_URL => $url,
      CURLOPT_FAILONERROR => TRUE,
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_TIMEOUT => 30,
      CURLOPT_USERPWD => $this->auth_header,
      CURLOPT_HEADER => TRUE,
      CURLOPT_HTTPHEADER => array('Content-Type: application/xml',),
      CURLOPT_PUT => TRUE,
      CURLOPT_INFILE => $fp,
      CURLOPT_INFILESIZE => strlen($set_category_xml_request),
    );

    $ch = curl_init();    // create curl resource
    curl_setopt_array($ch, $category_options);

    $category_return = new stdClass;
    $category_return->response = curl_exec($ch); // execute and get response
    $category_return->header   = substr($category_return->response, 0, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $category_return->body     = substr($category_return->response, curl_getinfo($ch, CURLINFO_HEADER_SIZE));
    $category_return->error    = curl_error($ch);
    $category_return->info     = curl_getinfo($ch);

    //kill the file stream we created above
    fclose($fp);
    curl_close($ch);
    return $category_return; 
  }
}
