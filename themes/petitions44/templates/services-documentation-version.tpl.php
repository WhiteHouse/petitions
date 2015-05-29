<?php

/**
 * @file
 * Theme implementation for a documentation version.
 *
 * A given documentation version contains the following nested elements:
 * - Resources defined by Services, e.g., user, node, etc.
 * - Method bundles, e.g., operations, actions, targeted actions.
 * - Methods, e.g., create, update, index, etc.
 *
 * Available variables:
 * - $resources: An array of resources for this documentation version.
 * - $description
 * - $table_of_contents
 */
?>
<div id="how-why-content" class="clearfix">
  <?php if (!empty($left_nav)): ?>
    <?php print $left_nav; ?>
  <?php endif; ?>

  <div class="main-content">
    <div class="services-documentation-version">
      <?php if ($description): ?>
        <div class="services-version-description">
        <?php print render($description); ?>
      </div>
      <?php endif; ?>
      <?php if ($table_of_contents): ?>
        <div class="services-documentation-toc">
          <h2 class="toc-title">Resources</h2>
          <?php print render($table_of_contents); ?>
        </div>
      <?php endif; ?>
      <?php if ($resources): ?>
        <div class="services-documentation-resources">
          <?php print render($resources); ?>
        </div>
      <?php endif; ?>
    </div><!-- /services-documentation-version -->

  </div><!--/main-content-->
</div><!--/how-why-content-->
