<?php

/**
 * @file
 * Provides configuration settings to define output.
 */


/**
 * Menu callback for configuration form (admin/config/petitions/homepage).
 * @todo Use a custom form instead of system_settings_form() and use fieldsets.
 */
function petitions_homepage_configure() {
  $form['petitions_homepage_petition_hero'] = array(
    '#type' => 'textarea',
    '#title' => t('Hero Content'),
    '#description' => t('Content displayed in the <em>Hero</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_petition_hero', petitions_homepage_petition_hero_default()),
  );

  $form['petitions_homepage_easy_steps'] = array(
    '#type' => 'textarea',
    '#title' => t('Easy Steps Content'),
    '#description' => t('Content displayed in the <em>Easy Steps</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_easy_steps', petitions_homepage_easy_steps_default()),
  );

  $form['petitions_homepage_introductory_video'] = array(
    '#type' => 'textarea',
    '#title' => t('Introductory Video Content'),
    '#description' => t('Content displayed in the <em>Introductory Video</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_introductory_video', petitions_homepage_introductory_video_default()),
  );

  $form['petitions_homepage_aside_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Right Sidebar Content?'),
    '#description' => t('Select to display content in the <em>Aside</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_aside_flag', 1),
  );

  $form['petitions_homepage_featured_responses_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Featured Responses?'),
    '#description' => t('Select to display content in the <em>Featured Responses</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_featured_responses_flag', 1),
  );

  $form['petitions_homepage_featured_responses'] = array(
    '#type' => 'textarea',
    '#title' => t('Featured Responses Content'),
    '#description' => t('Content displayed in the <em>Featured Responses</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_featured_responses', petitions_homepage_featured_responses_default()),
  );

  $form['petitions_homepage_recent_petitions_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Recent Petitions?'),
    '#description' => t('Select to display content in the <em>Recent Petitions</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_recent_petitions_flag', 1),
  );

  $form['petitions_homepage_twitter_flag'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Latest Tweet?'),
    '#description' => t('Select to display content in the <em>Latest Tweet</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_twitter_flag', 1),
  );

  $form['petitions_homepage_twitter'] = array(
    '#type' => 'textarea',
    '#title' => t('Latest Tweet Content'),
    '#description' => t('Content displayed in the <em>Latest Tweet</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_twitter', petitions_homepage_twitter_default()),
  );

  $form['petitions_homepage_more_from_the_whitehouse'] = array(
    '#type' => 'textarea',
    '#title' => t('More from the White House Content'),
    '#description' => t('Content displayed in the <em>More from the White Hosue</em> area of the homepage.'),
    '#default_value' => variable_get('petitions_homepage_more_from_the_whitehouse', petitions_homepage_more_from_the_whitehouse_default()),
  );

  return system_settings_form($form);
}
