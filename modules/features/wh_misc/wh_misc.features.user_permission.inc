<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function wh_misc_user_default_permissions() {
  $permissions = array();

  // Exported permission: access administration pages
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: access content
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      '0' => 'Captioner',
      '1' => 'Content Reviewer',
      '2' => 'Featured Commenter',
      '3' => 'Intern Office Manager',
      '4' => 'Microsite Boss',
      '5' => 'Microsite General User',
      '6' => 'New Media Content Boss',
      '7' => 'New Media Content Creator',
      '8' => 'Photo Office',
      '9' => 'Press Office Content Boss',
      '10' => 'Press Office Content Creator',
      '11' => 'anonymous user',
      '12' => 'authenticated user',
    ),
  );

  // Exported permission: access content overview
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: access contextual links
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(),
  );

  // Exported permission: access dashboard
  $permissions['access dashboard'] = array(
    'name' => 'access dashboard',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: access site in maintenance mode
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: access site reports
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: access toolbar
  $permissions['access toolbar'] = array(
    'name' => 'access toolbar',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: access user profiles
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer actions
  $permissions['administer actions'] = array(
    'name' => 'administer actions',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer blocks
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer content types
  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(),
  );

  // Exported permission: administer features
  $permissions['administer features'] = array(
    'name' => 'administer features',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer feeds
  $permissions['administer feeds'] = array(
    'name' => 'administer feeds',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer filters
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(),
  );

  // Exported permission: administer image styles
  $permissions['administer image styles'] = array(
    'name' => 'administer image styles',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer imce
  $permissions['administer imce'] = array(
    'name' => 'administer imce',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer menu
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer modules
  $permissions['administer modules'] = array(
    'name' => 'administer modules',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer nodes
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer pathauto
  $permissions['administer pathauto'] = array(
    'name' => 'administer pathauto',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer permissions
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer petition settings
  $permissions['administer petition settings'] = array(
    'name' => 'administer petition settings',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer petitions
  $permissions['administer petitions'] = array(
    'name' => 'administer petitions',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer search
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: administer shortcuts
  $permissions['administer shortcuts'] = array(
    'name' => 'administer shortcuts',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer site configuration
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer software updates
  $permissions['administer software updates'] = array(
    'name' => 'administer software updates',
    'roles' => array(),
  );

  // Exported permission: administer taxonomy
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer themes
  $permissions['administer themes'] = array(
    'name' => 'administer themes',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer url aliases
  $permissions['administer url aliases'] = array(
    'name' => 'administer url aliases',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: administer users
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: block IP addresses
  $permissions['block IP addresses'] = array(
    'name' => 'block IP addresses',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: bypass node access
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(),
  );

  // Exported permission: cancel account
  $permissions['cancel account'] = array(
    'name' => 'cancel account',
    'roles' => array(),
  );

  // Exported permission: change own username
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: clear issues_taxonomy feeds
  $permissions['clear issues_taxonomy feeds'] = array(
    'name' => 'clear issues_taxonomy feeds',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: create petition
  $permissions['create petition'] = array(
    'name' => 'create petition',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: create url aliases
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: create wh_response content
  $permissions['create wh_response content'] = array(
    'name' => 'create wh_response content',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: customize shortcut links
  $permissions['customize shortcut links'] = array(
    'name' => 'customize shortcut links',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: delete any wh_response content
  $permissions['delete any wh_response content'] = array(
    'name' => 'delete any wh_response content',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: delete own wh_response content
  $permissions['delete own wh_response content'] = array(
    'name' => 'delete own wh_response content',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: delete revisions
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: delete terms in 1
  $permissions['delete terms in 1'] = array(
    'name' => 'delete terms in 1',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: delete terms in 2
  $permissions['delete terms in 2'] = array(
    'name' => 'delete terms in 2',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: delete terms in 3
  $permissions['delete terms in 3'] = array(
    'name' => 'delete terms in 3',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: edit any wh_response content
  $permissions['edit any wh_response content'] = array(
    'name' => 'edit any wh_response content',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: edit own wh_response content
  $permissions['edit own wh_response content'] = array(
    'name' => 'edit own wh_response content',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: edit terms in 1
  $permissions['edit terms in 1'] = array(
    'name' => 'edit terms in 1',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: edit terms in 2
  $permissions['edit terms in 2'] = array(
    'name' => 'edit terms in 2',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: edit terms in 3
  $permissions['edit terms in 3'] = array(
    'name' => 'edit terms in 3',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: import issues_taxonomy feeds
  $permissions['import issues_taxonomy feeds'] = array(
    'name' => 'import issues_taxonomy feeds',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: manage features
  $permissions['manage features'] = array(
    'name' => 'manage features',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: notify of path changes
  $permissions['notify of path changes'] = array(
    'name' => 'notify of path changes',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: revert revisions
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: search content
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: select account cancellation method
  $permissions['select account cancellation method'] = array(
    'name' => 'select account cancellation method',
    'roles' => array(),
  );

  // Exported permission: switch shortcut sets
  $permissions['switch shortcut sets'] = array(
    'name' => 'switch shortcut sets',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: use advanced search
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      '0' => 'admin',
    ),
  );

  // Exported permission: use text format full_html
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'admin',
    ),
  );

  // Exported permission: view own unpublished content
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  // Exported permission: view revisions
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: view the administration theme
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      '0' => 'New Media Content Boss',
      '1' => 'New Media Content Creator',
      '2' => 'admin',
    ),
  );

  return $permissions;
}
