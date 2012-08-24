#!/bin/bash
echo "Rebuilding..."
rm -rf modules/contrib themes/tao themes/rubik libraries/
drush -y make --no-core --contrib-destination=. drupal-org.make
