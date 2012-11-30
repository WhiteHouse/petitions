<?php

function petitions_form_install_configure_form_alter(&$form, &$form_state, $form_id) {
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['server_settings']['site_default_country']['#default_value'] = 'US';
  $form['update_notifications']['update_status_module']['#default_value'] = array(0);
}

/**
 * Implementation of hook_profile_form_alter().
 */
function petitions_form_alter(&$form, $form_state, $form_id) {
  if ($form_id == 'install_configure_form') {
    $text = "
<pre>
Stop here. If you have not done this yet, add the settings below to settings.php (that is, sites/default/settings.php). Then proceed.

(Replace the values with for \$mongo_host and \$mongo_db_name as appropriate.)

// Set mongo configuration
\$mongo_host = '127.0.0.1';
\$mongo_db_name = 'petition';
\$conf['mongodb_connections'] = array(
  'default' => array(
    'host' => \$mongo_host,
    'db' => \$mongo_db_name
  ),
  'petition_tool' => array(
    'host' => \$mongo_host,
    'db' => \$mongo_db_name
  ),
  'petition_tool_archive' => array(
    'host' => \$mongo_host,
    'db' => \$mongo_db_name
  ),
  'petition_tool_response' => array(
    'host' => \$mongo_host,
    'db' => \$mongo_db_name
  ),
  'petition_tool_signatures' => array(
    'host' => \$mongo_host,
    'db' => \$mongo_db_name
  ),
);
\$conf['mongodb_collections'] = array(
  'petitions' => 'petition_tool',
  'archive_petitions' => 'petition_tool_archive',
  'petition_response' => 'petition_tool_response',
  'petition_signatures' => 'petition_tool_signatures',
);</pre>";
   drupal_set_message($text);
  }
}
