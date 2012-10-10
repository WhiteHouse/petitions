<div id="how-why-content" class="clearfix">
  <?php print $left_nav; ?>

  <div class="main-content">
    <h2 class="page-title"><?php print $title; // $title should already be sanitized ?></h2>
    <?php
      hide($content['links']['#links']['node-readmore']);
      print render($content);
    ?>
  </div><!--/main-content-->
</div><!--/how-why-content-->
