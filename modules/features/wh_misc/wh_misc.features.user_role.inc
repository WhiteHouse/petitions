<?php

/**
 * Implementation of hook_user_default_roles().
 */
function wh_misc_user_default_roles() {
  $roles = array();

  // Exported role: Captioner
  $roles['Captioner'] = array(
    'name' => 'Captioner',
    'weight' => '0',
  );

  // Exported role: Content Reviewer
  $roles['Content Reviewer'] = array(
    'name' => 'Content Reviewer',
    'weight' => '0',
  );

  // Exported role: Featured Commenter
  $roles['Featured Commenter'] = array(
    'name' => 'Featured Commenter',
    'weight' => '0',
  );

  // Exported role: Intern Office Manager
  $roles['Intern Office Manager'] = array(
    'name' => 'Intern Office Manager',
    'weight' => '0',
  );

  // Exported role: Microsite Boss
  $roles['Microsite Boss'] = array(
    'name' => 'Microsite Boss',
    'weight' => '0',
  );

  // Exported role: Microsite General User
  $roles['Microsite General User'] = array(
    'name' => 'Microsite General User',
    'weight' => '0',
  );

  // Exported role: New Media Content Boss
  $roles['New Media Content Boss'] = array(
    'name' => 'New Media Content Boss',
    'weight' => '0',
  );

  // Exported role: New Media Content Creator
  $roles['New Media Content Creator'] = array(
    'name' => 'New Media Content Creator',
    'weight' => '0',
  );

  // Exported role: Photo Office
  $roles['Photo Office'] = array(
    'name' => 'Photo Office',
    'weight' => '0',
  );

  // Exported role: Press Office Content Boss
  $roles['Press Office Content Boss'] = array(
    'name' => 'Press Office Content Boss',
    'weight' => '0',
  );

  // Exported role: Press Office Content Creator
  $roles['Press Office Content Creator'] = array(
    'name' => 'Press Office Content Creator',
    'weight' => '0',
  );

  // Exported role: admin
  $roles['admin'] = array(
    'name' => 'admin',
    'weight' => '2',
  );

  return $roles;
}
