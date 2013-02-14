<?php
/**
 * @file
 * services-documentation-method-example-implementations-bundles.tpl.php
 *
 * Template file for theming an example implementations bundle for a given
 * Services method.
 *
 * Available custom variables:
 * - $language:
 * - $examples:
 */
?>
<!-- services-documentation-method-example-implementations-bundle -->
<div class="services-documentation-method-example-implementations-bundle">
  <h6 class="examples-language"><?php print $language; ?></h6>
  <?php foreach ($examples as $example): ?>
    <?php print render($example); ?>
  <?php endforeach; ?>
</div>
<!-- /services-documentation-method-example-implementations-bundle -->
