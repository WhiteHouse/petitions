<div id="my-petitions">
  <div class="top-msg-bar"><?php print t('Welcome to your petitions !user', array('!user' => $username)); ?></div><!--/top msg bar-->
  <div class="petition-list clearfix">
    
    <?php if(!empty($draft)): ?>
    <div class="petition-list-group clearfix" id="your-saved-petitions">
      <h2 class="section-title"><?php print t('Saved Petitions'); ?></h2>
      <?php print $draft; ?>                        
    </div>
    <?php endif; ?>
  
    <?php if(!empty($created)): ?>  
    <div class="petition-list-group clearfix clear" id="petitions-you-created">
      <?php if(!empty($draft)): ?><div class="fuzzy-divide"></div><?php endif; ?>
      <h2 class="section-title"><?php print t('Petitions You Created'); ?></h2>
      <?php print $created; ?>
    </div>
    <?php endif; ?>

    <?php if(!empty($signed)): ?> 
    <div class="petition-list-group clearfix clear" id="petitions-you-signed">
      <?php if(!empty($draft) || !empty($created)): ?><div class="fuzzy-divide"></div><?php endif; ?>
      <h2 class="section-title"><?php print t('Petitions You Signed'); ?></h2>
      <?php print $signed; ?>
    </div>
    <?php endif; ?>
  </div>
</div>