<div id="response-<?php print $nid; ?>" class="<?php if($cols == 2): ?>col-2 float-left<?php endif; ?> <?php if(($entry_num % 2 == 0) && $cols == 2): ?>add-margin<?php endif; ?>">
  <div class="entry response-entry">
    <div class="content">
      <div class="title"><?php print $title; ?></div><!--/title-->
      <div class="creation-date"><?php print $creation_date; ?></div><!--/creation date-->
    </div>
    <div class="action-bar clearfix">
      <div class="read-response"><?php print $response_link; ?></div><!--/read response-->
    </div><!--/action bar-->
  </div>
</div><!--/entry-->
