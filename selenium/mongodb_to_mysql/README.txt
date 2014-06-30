create_2_users [run as admin]
==================================
This test creates 2 user accounts that are then used by the test
mongodb_petition_signature_creation



Then logout, and run the next test.


mongodb_petition_signature_creation [run as anonymous user]
===========================================================

WARNING: This test actually creates a petition and 2 signatures so make sure to
run it in a sandbox if not you will need to remove the petitions, signatures,
user1 and user2



mongodb_petitions_list [run as anonymous user]
==============================
This test requires some specific petition content in order to run successfully.
The content is created by running a drush command.

/**
 * @see /profiles/petitions/drush/commands/generate_petitions_content.drush.inc
 */
drush genpetcon --seed=7659 mongodb petition 4

In order to run genpetcon you will also need the Faker library. To download the
Faker library run the following command from the root of your site.
git clone   git@github.com:fzaninotto/Faker.git vendors/Faker

mongodb_mysql_petition_signature_comparison [run as the user that owns the petitions]
==============================
This test uses the /dashboard to loop through all the petitions and validates
the mongo and the mysql version of the petition and each of its signatures.

The comparison page is part of the migrate_mongo2mysql_petsig module, so it is
required to run the test successfully.

Because this test requires while loops, a Selenium IDE plugin is also required.

This plugin can be found:
https://github.com/darrenderidder/sideflow

To install the plugin:
1) Download sideflow.js.
2) In the Selenium IDE menu, go to Options > options.
3) Select sideflow.js under "Selenium Core Extensions".
4) Click OK.
5) Restart Selenium IDE.

