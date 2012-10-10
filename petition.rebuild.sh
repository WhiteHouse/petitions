#!/bin/bash
echo "Rebuilding..."
rm -rf modules/contrib themes/tao themes/rubik themes/zen themes/fortyfour libraries/
drush -y make --no-core --contrib-destination=. drupal-org.make
