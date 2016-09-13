<div class="<?php print $classes; ?>"<?php print $attributes; ?> data-bid="<?php print $block->bid ?>">
  <?php print render($title_prefix); ?>
  <?php if ($block->subject): ?>
    <h2 class="title"<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
  <?php endif;?>
  <?php print render($title_suffix); ?>

  <?php print $content; ?>
</div> <!-- /block -->
