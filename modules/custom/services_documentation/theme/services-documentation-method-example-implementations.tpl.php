<?php
/**
 * @file
 * services-documentation-method-example-implementations.tpl.php
 *
 * Template file for theming an example implementation for a given Services
 * method.
 *
 * Available custom variables:
 * - $name:
 * - $description:
 * - $code:
 * - $download_link:
 * - $uses_sdk:
 */
?>
<!-- services-documentation-method-example-implementation -->
<div class="services-documentation-method-example-implementation">
  <h7 class="example-name"><?php print $name; ?></h7>
  <?php if ($uses_sdk = TRUE): ?>
    <div class="examample-uses-sdk"><strong>Uses SDK</strong></div>
  <?php endif; ?>
  <?php if ($description): ?>
    <div class="example-description"><?php print $description; ?></div>
  <?php endif; ?>
  <?php if ($code): ?>
    <pre class="example-implementation"><?php print $code; ?></pre>
  <?php endif; ?>
  <?php if ($download_link): ?>
    <div class="example-download">
      <?php print l(t('Download'), $download_link, array('absolute' => TRUE)); ?>
    </div>
  <?php endif; ?>
</div>
<!-- /services-documentation-method-example-implementation -->
