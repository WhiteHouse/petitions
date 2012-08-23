<?php

/**
 * Implementation of hook_default_rules_configuration().
 */
function petitions_user_registration_default_rules_configuration() {
  $items = array();
  $items['rules_user_submit'] = entity_import('rules_config', '{ "rules_user_submit" : {
      "LABEL" : "user_submit",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "user_reg" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_insert" ],
      "IF" : [
        { "entity_is_of_type" : { "entity" : [ "account" ], "type" : "user" } },
        { "NOT data_is_empty" : { "data" : [ "account:field-origin" ] } }
      ],
      "DO" : [ { "redirect" : { "url" : "[account:field-origin]#thank-you=p" } } ]
    }
  }');
  $items['rules_user_validate_redirect'] = entity_import('rules_config', '{ "rules_user_validate_redirect" : {
      "LABEL" : "user_validate_redirect",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "user_reg" ],
      "REQUIRES" : [ "rules", "wh_core", "logintoboggan_rules" ],
      "ON" : [ "logintoboggan_validated" ],
      "IF" : [
        { "entity_is_of_type" : { "entity" : [ "account" ], "type" : "user" } },
        { "NOT data_is_empty" : { "data" : [ "account:field-origin" ] } }
      ],
      "DO" : [
        { "login_user" : { "account" : [ "account" ] } },
        { "redirect" : { "url" : "[account:field-origin]" } },
        { "data_set" : { "data" : [ "account:field-origin" ] } }
      ]
    }
  }');
  return $items;
}
