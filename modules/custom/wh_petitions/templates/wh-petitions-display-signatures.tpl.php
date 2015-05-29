<?php if(empty($from_ajax) || $page == 2) { $i = 1; } else { $i = 4; } ?>
<?php foreach($signatures as $key => $signature): ?>
  <?php
    if (!empty($signature['is_creator'])) { $class = 'entry-creator'; }
    elseif (!empty($signature['is_user'])) { $class = 'entry-you-signed'; }
    else { $class = 'entry-reg'; }
  ?>
   
  <div class="entry <?php print $class; ?> <?php if($i % 4 == 0): ?>last<?php endif; $i++; ?>">  
    <?php if(!empty($signature['comment']) && $class != 'entry-creator'): ?>
      <div class="comment-popup display-none">
        <div class="top"></div><!--/top-->
        <div class="mid"><?php print $signature['comment'];  // This has already gone through check_plain in wh_petitions_get_signatures() ?></div><!--/mid-->
        <div class="bottom"></div><!--/bottom-->
      </div><!--/comment popup--> 
    <?php endif; ?> 
      
    <?php if(!empty($signature['is_creator'])): ?><div class="title"><?php print t('creator'); ?></div><?php endif; ?>
    
    <div class="name"><?php print check_plain($signature['full_name']); ?></div><!--/name-->
    <div class="details">
      <?php if (!empty($signature['location'])): ?>
        <?php print $signature['location']; ?><br />
      <?php endif; ?>
      <?php print date("F d, Y", $signature['timestamp']); ?><br />
      <?php print t('Signature #'); ?> <?php print $signature['number']; ?>
    </div>
    
    <?php //if(!empty($signature['comment']) && $class != 'entry-creator'): ?>
    <?php if (FALSE): ?>
      <div class="read-comment">
        <?php print t("Read Comment"); ?>
      </div>
    <?php endif; ?>
  </div>
<?php endforeach; ?>                           	 

<?php if($has_more): ?>
  <?php print l(t('Load Next !count Signatures', array('!count' => $sigs_per_page)), $nice_url, array('query' => array('page' => ($page + 1), 'last' => $last_id), 'html' => TRUE, 'attributes' => array('class' => array('load-next', 'no-follow'), 'rel' => $petition_id))); ?>
  <div id="last-signature-id" class="display-none"><?php print $last_id; ?></div>
  <div id="found-creator" class="display-none"><?php print $previously_found_creator; ?></div>
<?php endif; ?>