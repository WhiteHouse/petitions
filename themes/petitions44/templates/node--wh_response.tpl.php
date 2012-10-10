<div class="petition-detail clearfix">
  <div class="petition-response individual-response">
    <div class="response-author-org clearfix">
      <?php print $agency_seal; ?>
      <span class="official"><?php print t('Official !agency_name Response to', array('!agency_name' => $agency_name)); ?></span>
      <?php if ($petition_count > 0): ?>
        <span class="petition-title"><?php print $petition_title; ?>       
          <?php if ($petition_count > 1): ?>
            <a href="" class="no-follow" id="more-petitions">
            <?php if ($petition_count == 2): ?>
              <?php print t('and !count other petition', array('!count' => $petition_count - 1)); ?>
            <?php else: ?>
              <?php print t('and !count other petitions', array('!count' => $petition_count - 1)); ?>
            <?php endif; ?>
            </a>
          <?php endif; ?>
        </span><!--/petition title-->
      <?php endif; ?>
    </div><!--/response author-org-->
                             
    <h1 class="title"><?php print $title; ?></h1>
    
    <?php if (!empty($video)): ?>
      <div class="media"><?php print $video; ?></div>
    <?php endif; ?>
    <?php if (!empty($audio)): ?>
      <div class="media"><?php print $audio; ?></div>
    <?php endif; ?>  
      
    <?php print $body; ?>
  </div><!--/petition response-->            

  <div id="share-this-petition">
    <ul>
      <li class="title"><?php print t('Share this Response'); ?></li>
      <li class="share-link twitter"><?php print $twitter_link; ?></li>
      <li class="share-link facebook"><?php print $facebook_link; ?></li>
    </ul>
  </div><!--/share this petition-->
</div><!--/petition detail-->
              
<?php if ($petition_count > 0): ?>              
  <div class="petition-list clearfix">
    <div class="response-applies-msg">
      <?php if ($petition_count == 1): ?>
        <?php print t('The above response applies to the following !count petition', array('!count' => $petition_count)); ?>
      <?php else: ?>
        <?php print t('The above response applies to the following !count petitions', array('!count' => $petition_count)); ?>
      <?php endif; ?>
    </div><!--/response applies-msg-->
              
    <div class="full-page-list" id="petitions">
      <?php print $petition_html; ?>
    </div><!--full-page-list-->
  </div><!--/petition-list-->
<?php endif; ?>