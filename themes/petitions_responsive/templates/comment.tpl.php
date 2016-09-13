<article class="<?php print $classes . ' ' . $zebra; ?>">
  <header>
    <?php print render($title_prefix); ?>
    <h3 class="title"><?php print $title ?></h3>
    <?php print render($title_suffix); ?>

    <?php if ($new) : ?>
      <mark class="new"><?php print drupal_ucfirst($new) ?></mark>
    <?php endif; ?>

    <?php print $picture ?>

    <span class="submitted"><?php print $submitted; ?></span>
  </header>

  <div class="content">
    <?php
      hide($content['links']);
      print render($content);
      ?>
    <?php if ($signature): ?>
      <footer class="signature"><?php print $signature ?></footer>
    <?php endif; ?>
  </div>

  <?php if (!empty($content['links'])): ?>
    <div class="links"><?php print render($content['links']); ?></div>
  <?php endif; ?>
</article><!-- /comment -->