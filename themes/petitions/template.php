<?php

/**
 * Preprocessor for theme('page').
 */

function petitions_preprocess_page(&$variables) {
  drupal_add_js('(function($) {$(".recent ul").append($("#block-wh-petitions-wh-petitions-recent-petitions ul").html());$(".recent ul li a").addClass("no-follow");})(jQuery)', array('type' =>'inline' , 'scope' => 'footer'));
}
