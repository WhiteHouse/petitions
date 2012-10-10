<?php

print drupal_get_css();
print drupal_get_js();

$markup = $page['#children'];

print $markup . '<br clear="all" />';
