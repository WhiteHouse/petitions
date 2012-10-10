INSTALL.md
==============

**Contents**

* "Alpha" software status
* Install MongoDB for local development
* Installing and configuring Petitions
* Manual Rules import
* MongoDB configuration in settings.php


"Alpha" software status
--------------------------------

"Alpha" means we cannot promise to provide an upgrade path to users who build sites on the current code base.

Later releases will remove this application's dependence on MongoDB. Our intention is to evolve this code base into an install profile that others can easily reuse, extend and contribute to. This is not the state of the current application, which was made specifically for the White House's particular use cases and hosting environment.
 
These instructions will help you install Drupal, get Drupal talking to MySQL and MongoDB, and let you try out the existing code base.

Where the application still has dependencies on configuration stored in the site's database, these are areas where the install profile remains a work in progress. We will release improvements as we make them on GitHub. In the meanwhile, patches are welcome too.


Install MongoDB for local development
-------------------------------------

For local development on Mac OSX with MAMP (similar with XAMPP), install Homebrew, then do this:

```
$ brew versions mongo
$ cd /usr/local/Cellar
$ git checkout dae14ec /usr/local/Library/Formula/mongodb.rb 
$ brew install mongo

$ /Applications/MAMP/bin/php/php5.3.6/bin/pecl install mongo
$ mkdir /data/db
$ mongod  # This starts mongo.
$ mongo   # This starts the mongo client.
```

Note: The application was built to run on MongoDB 1.8 (not 2.x). This doesn't mean it won't work on 2.x. But it hasn't been rigorously tested on 2.x.


Installing and configuring Petitions
------------------------------------

1) Install Drush

2) Download Drupal 7.x

3) Place this petitions directory inside Drupal's profiles directory.

4) Use Drush make to download contrib projects like this:

```
drush -y make --no-core --contrib-destination=. drupal-org.make
```

5) Follow the normal Drupal installation process. When prompted to select
     a profile, select "Petitions". Drupal will rewrite your settings.php file.
     After it does, you will be prompted to add a snippet like this to the end
     of settings.php. Do this before you visit your site, otherwise Drupal will
     be unhappy: 

```php
      // Set mongo configuration
      $mongo_host = '127.0.0.1';
      $mongo_db_name = 'petition';
      $conf['mongodb_connections'] = array(
        'default' => array('host' => $mongo_host, 'db' => $mongo_db_name),
        'petition_tool' => array('host' => $mongo_host, 'db' => $mongo_db_name),
        'petition_tool_archive' => array('host' => $mongo_host, 'db' => $mongo_db_name),
        'petition_tool_response' => array('host' => $mongo_host, 'db' => $mongo_db_name),
        'petition_tool_signatures' => array('host' => $mongo_host, 'db' => $mongo_db_name),
      );
      $conf['mongodb_collections'] = array(
        'petitions' => 'petition_tool',
        'archive_petitions' => 'petition_tool_archive',
        'petition_response' => 'petition_tool_response',
        'petition_signatures' => 'petition_tool_signatures',
      );
  
      # (Optional): 
      # $conf['mongodb_options'] = array(
      #   'replicaSet' => 'petitions',
      #   'timeout' => 1,
      # );
```

6) To quickly, easily install the rest of the required modules, install:
* Petition Install 1
* Petition Install 2
* Petition Install 3
* Petition Install 4

You can do this with drush by copying and pasting these commands at the command line:

```
drush en -y petition_install1
drush en -y petition_install2
drush en -y petition_install3
drush en -y petition_install4
```

Drupal will complain when you install these modules. If you ignore the error 
messages and proceed, you should be able to get a site running with the 
basic White House petition creation and signing workflow.

For development, you may prefer to look at the .info files in the Petition
Install modules, and just install these one-by-one. Petition Install modules
don't actually do anything. They just make it easy to get through
installation quickly and are helpful for work currently in progress to
repackage Petition as an install profile. It doesn't harm anything to have
them on. But for development, you may not want these modules hanging around
your site adding unnecessary dependencies.

Here is a list of modules that should be enabled for the system to run properly:
<table>
  <tr>
     <th><B>Module Name</B></th>
	<th><B>Module</B></th>
	<th><B>Status</B></th>
  </tr>
  <tr>
    	<td>Petitions - LoginToboggan Settings</td>
	<td>petitions_logintoboggan_settings</td>
	<td>Enabled</td>
  </tr>
  <tr>
    	<td>Petitions - User Registration</td>
	<td>petitions_user_registration</td>
	<td>Enabled Overridden</td>
  </tr>
  <tr>
    	<td>Taxonomy Sync</td>
	<td>taxonomy_sync</td>
	<td>Enabled</td>
  </tr>
  <tr>
    	<td>Page</td>
	<td>wh_petition_page</td>
	<td>Enabled</td>
  </tr>
  <tr>
    	<td>Response</td>
	<td>wh_response_feature</td>
	<td>Enabled Overridden</td>
  </tr>
  <tr>
    	<td>WH User SS Data</td>
	<td>wh_user_ss_data</td>
	<td>Enabled</td>
  </tr>
  <tr>
    	<td>Whitehouse User Profile</td>
	<td>wh_user_profile</td>
	<td>Enabled Overridden</td>
  </tr>
  <tr>
    	<td>Contexts</td>
	<td>wh_contexts</td>
	<td>Enabled</td>
  </tr>
  <tr>
    	<td>Misc</td>
	<td>wh_misc</td>
	<td>Enabled Overridden</td>
  </tr>
</table>

8) The "main" profile should have these fields: First Name, Last Name, City, 
     State, Zip, Country. To confirm, check here:
       
* `admin/structure/profiles`
* `admin/structure/profiles/manage/main/fields`

If required profile fields are missing, revert Whitehouse User Profile (wh_user_profile) to default here: `admin/structure/features`

9) The Petitions - User Registration (petitions_user_registration) feature module doesn't always install correctly.

* Try revert. (Even though Rules Configuration will still say "overridden".)
        `admin/structure/features`

* Enable Rules UI (rules_admin) here:
        `admin/modules`

* Go to Rules and confirm you have two rules, user_submit, user_validate_redirect:
        `admin/config/workflow/rules`

* If these rules are missing, you can import them manually.

* Import the rules_user_validate_redirect here (check "Overwrite"):
        `admin/config/workflow/rules/reaction/import`

```php
        { "rules_user_validate_redirect" : {
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
        }
```

Import the user_submit rule here (check "Overwrite"):
`admin/config/workflow/rules/reaction/import`

```php
        { "rules_user_submit" : {
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
        }
```

10) By default petitions are not made public on the site until they clear a certain threshold of signatures. To collect these first signatures, signers must go directly to the petition's URL. Set the signature threshold here:
        `admin/config/system/petitions`

11) Users won't be able to create accounts and sign petitions until CAPTCHA is configured. Just to get things working, all you need to do is go here and follow the link on the config page to get an API key for your site:
        `admin/config/people/captcha/recaptcha`

12) For development, you may want to add this to settings.php:
 
```php
        $conf['error_level'] = 2;         // Show all messages on your screen.
        ini_set('display_errors', TRUE);  // These lines give you content on
                                          // "white screen of death" (WSOD) pages.
       ini_set('display_startup_errors', TRUE);
```