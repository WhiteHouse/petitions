<div id="petition-<?php print $petition_id; ?>" class="<?php if($cols == 2): ?>col-2 float-left<?php endif; ?> <?php if(($entry_num % 2 == 0) && $cols == 2): ?>add-margin<?php endif; ?>">
  <div class="entry <?php if(!empty($petition_status)): ?>entry-<?php print $petition_status; ?><?php endif; ?>">
    <?php if(!empty($petition_status)): ?>
      <div class="tag-status"><?php print $petition_status; ?></div><!--/tag status-->
    <?php endif; ?>
    <div class="title">
      <?php if(!empty($draft)): ?>
        <?php print l($title, 'petition/create/' . $petition_id, array("html" => TRUE)); ?>   
      <?php else: ?>
        <?php $atts = array('html' => TRUE); if (!empty($target)) { $atts['attributes'] = array('target' => $target); } ?>
        <?php $atts['attributes']['rel'] = 'nofollow'; ?>
        <?php if (empty($title_link)) { $title_link = $nice_url; } ?>
        <?php print l($title, $title_link, $atts); ?>      
      <?php endif; ?>
    </div>
    <div class="action-bar clearfix">
      <div class="num-sig"><span class="num"><?php print $signature_count; ?></span>&nbsp;&nbsp;<?php print t('Signatures'); ?></div>
      <?php if(!empty($dashboard)): ?>
        <?php if(!empty($draft)): ?>
          <div class="action-links">
            <?php print $petition_link; ?>
            <?php if($delete_link): ?>
              <?php print l(t('Delete'), 'petition/delete/' . $petition_id, array('attributes' => array('id' => 'delete-petition-link-' . $petition_id, 'class' => 'delete delete-petition no-follow', 'rel' => $petition_id))); ?>
              <div class="delete-insert delete-insert-<?php print $petition_id; ?>"> 
              <div id="delete-petition-<?php print $petition_id; ?>" class="delete-petition-overlay display-none">
                <a class="close-button no-follow" rel="<?php print $petition_id; ?>"><?php print t('Close'); ?></a>
                <div class="wrap">
                  <h2 class="alert"><?php print t('You are about to delete your petition draft.'); ?></h2>
                  <p><?php print $delete_text; ?></p>
                </div>
                  
                <?php print $delete_form; ?>
              </div><!--/report petition overlay-->
              </div>
            <?php endif; ?>
          </div>
        <?php else: ?>
          <div class="action-links"><?php print $petition_link; ?></div>
        <?php endif; ?>
      <?php else: ?>
        <?php if(strlen($signature_count) > 8): ?>
          <div class="action-links"><div class="view"><?php print $petition_link; ?></div></div>
        <?php else: ?>
          <div class="find-out"><?php print $petition_link; ?></div>
        <?php endif; ?>
      <?php endif; ?>
    </div>
  </div>
</div>