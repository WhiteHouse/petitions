<?php
/**
 * @file
 * Template file for theming a given documentation version.
 *
 * A given documentation version contains the following nested elements:
 * - Resources, defined by Services. E.g., user, node, etc.
 * - Method bundles. E.g., operations, actions, targeted actions.
 * - Methods. E.g., create, update, index, etc.
 *
 * Available custom variables:
 * - $resources: An array of resources for this doumentation version.
 *
 */
?>
<!-- services-documentation-version -->
<div class="services-documentation-version">
  <?php print render($table_of_contents); ?>
  <?php print render($resources); ?>
</div>
<!-- /services-documentation-version -->
