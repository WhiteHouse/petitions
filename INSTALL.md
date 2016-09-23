INSTALL.md
==============

**Contents**

* "Alpha" software status
* Installing and configuring Petitions

"Alpha" software status
--------------------------------

"Alpha" means we cannot promise to provide an upgrade path to users who build sites on the current code base.

Later releases will remove this application's dependence on MongoDB. Our intention is to evolve this code base into an install profile that others can easily reuse, extend and contribute to. This is not the state of the current application, which was made specifically for the White House's particular use cases and hosting environment.

These instructions will help you install Drupal, get Drupal talking to MySQL and MongoDB, and let you try out the existing code base.

Where the application still has dependencies on configuration stored in the site's database, these are areas where the install profile remains a work in progress. We will release improvements as we make them on GitHub. In the meanwhile, patches are welcome too.


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
     a profile, select "Petitions." Drupal will rewrite your settings.php file.

6) **IMPORTANT!** Configure second database for signature processing:

For production sites, add databases for processing and archiving the signature
queue in your settings.php file. Use 'signatures_processing' and
'signatures_archive' as the keys for the configuration. For example, the
database configuration in your settings.php should look similar to this:

```php
    $databases = array (
      'default' =>
        array (
          'default' =>
          array (
            'database' => 'petitions',
            'username' => 'dbuser',
            'password' => '******',
            'host' => 'localhost',
            'port' => '',
            'driver' => 'mysql',
            'prefix' => '',
        ),
      ),
      'signatures_processing' =>
        array (
          'default' =>
          array (
            'database' => 'signatures_processing',
            'username' => 'dbuser',
            'password' => '******',
            'host' => 'localhost',
            'port' => '',
            'driver' => 'mysql',
            'prefix' => '',
        ),
      ),
      'signatures_archive' =>
        array (
          'default' =>
          array (
            'database' => 'signatures_archive',
            'username' => 'dbuser',
            'password' => '******',
            'host' => 'localhost',
            'port' => '',
            'driver' => 'mysql',
            'prefix' => '',
        ),
      ),
    );
```

(Further documentation on multiple databases: https://drupal.org/node/18429)

7) Enable needed modules.

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
    <td>wtp_contexts</td>
    <td>Enabled</td>
  </tr>
  <tr>
    <td>Misc</td>
    <td>wh_misc</td>
    <td>Enabled</td>
  </tr>
</table>

8) The "main" profile should have these fields: First Name, Last Name, City,
     State, Zip, Country. To confirm, check here:

* `admin/structure/profiles`
* `admin/structure/profiles/manage/main/fields`

If required profile fields are missing, revert Whitehouse User Profile (wh_user_profile) to default here: `admin/structure/features`

9) By default petitions are not made public on the site until they clear a certain threshold of signatures. To collect these first signatures, signers must go directly to the petition's URL. Set the signature threshold here:
        `admin/config/system/petitions`

10) Users won't be able to create accounts until CAPTCHA is configured. Just to get things working, all you need to do is go here and follow the link on the config page to get an API key for your site:
        `admin/config/people/captcha/recaptcha`

11) For development, you may want to add this to settings.php:

```php
        $conf['error_level'] = 2;         // Show all messages on your screen.
        ini_set('display_errors', TRUE);  // These lines give you content on
                                          // "white screen of death" (WSOD) pages.
       ini_set('display_startup_errors', TRUE);
```
Necessary Setup
--------------
- create taxonomy with machine name petition_type and populate with terms
- create a menu link to add petition entities (/petition/create)
- add terms to media type taxonomy used in Response nodes

- setup signature form
  -  create api key node and set key value to accepted.
  - add api key to /admin/config/services/petitionssignatureform
  - add signature form block to a page region /admin/structure/block/manage/petitionssignatureform/petitionssignatureform_form/configure

Known Issues
--------
- signature form displays a Contact Administrator message at present.
- /responses page gives 403
- error presented when viewing responses

```
Error: Call to undefined function wh_petition_tool_twitter_link() in wh_response_preprocess_node() (line 868 of /var/www/petitions/docroot/profiles/petitions/modules/custom/wh_response/wh_response.module
```
