<?php
/**
 * @file
 * Template file for theming a given Services method bundle.
 *
 * Method bundles groups a specific type of method, as defined by Services
 * terminology. E.g., operations, actions, targeted actions.
 *
 */
?>
<!-- services-documentation-method-bundle -->
<div class="services-documentation-method-bundle">
  <?php if (!empty($name)): ?>
    <h3><?php print $name; ?></h3>
  <?php endif; ?>

  <?php print render($methods); ?>
</div>
<!-- /services-documentation-method-bundle -->
