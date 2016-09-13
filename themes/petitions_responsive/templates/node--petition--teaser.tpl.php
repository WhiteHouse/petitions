<article class="<?php print $classes; ?>" data-nid="<?php print $node->nid; ?>" >

  <?php if (!empty($content['field_petition_issues'])): ?>
    <?php print render($content['field_petition_issues']); ?>
  <?php endif; ?>

  <h3<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h3>

  <div class="signatures-progress-bar">
    <?php print render($content['signature_progress_bar']); ?>
  </div> <!-- /signatures-progress-bar -->

  <div class="more-link">
    <?php if (!empty($content['links'])): ?>
      <div class="links">
        <?php print render($content['links']); ?>
      </div> <!-- /links -->
    <?php endif; ?>
  </div>
</article> <!-- /article #node -->
