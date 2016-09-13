<article class="<?php print $classes; ?>" data-nid="<?php print $node->nid; ?>" >

  <?php if ($title_prefix || $title_suffix || $display_submitted || $unpublished || !$page && $title): ?>
    <header>
      <?php print render($title_prefix); ?>
      <?php if (!$page && $title): ?>
        <h2<?php print $title_attributes; ?>><?php print $title; ?></h2>
      <?php endif; ?>
      <?php print render($title_suffix); ?>

      <?php if ($display_submitted): ?>
        <div class="submitted">
          <?php print $user_picture; ?>
          <?php print $submitted; ?>
        </div>
      <?php endif; ?>

      <?php if ($unpublished): ?>
        <p class="unpublished"><?php print t('Unpublished'); ?></p>
      <?php endif; ?>
    </header>
  <?php endif; ?>

  <div class="content">
    <?php
      // We hide the comments to render below.
      hide($content['comments']);
      hide($content['links']);

      // Similarly, hide the intro field to render in the intro region.
      hide($content['field_page_intro']);

      print render($content);
     ?>
  </div> <!-- /content -->

</article> <!-- /article #node -->
