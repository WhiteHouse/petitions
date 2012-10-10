<?php if (!empty($block->subject) && $block->bid != 'wh_petitions-wh_petitions_create_petition'): ?>
<div id="<?php print $block_html_id; ?>" class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <h2 class="trigger"><a href="#" class="no-follow"><?php print $block->subject ?></a></h2>

  <div class="toggle-container display-none">
    <?php print $content ?>
  </div>
</div>
<?php else: ?>
  <?php print $content ?>
<?php endif; ?>
