<div class="top-msg-bar"><?php print t('Thank you for creating this petition!'); ?></div><!--/top msg bar-->
<div class="basic-text-block" id="petition-thankyou">
  <p><?php print t('This is just the beginning! Right now only you know this unique URL. Share it with others to get more signatures on this petition. Until your petition reaches 150 signatures, it will not be publicly viewable on the Open Petitions section of We the People, so be sure to share this URL:'); ?></p>
  <p>
    <?php print t('Short URL:'); ?> <?php print $short_url; ?><br />
    <?php print t('Save and Share this URL:'); ?> <?php print $link; ?>
  </p>
        
  <p><?php print t('An email has been sent to !email with this information.  You can forward it to others to help promote this petition.', array('!email' => $email_address)); ?></p>
        
  <div class="petition-detail">
      <div class="sig-num clearfix">
        <div class="col-2" id="sig-needed">
          <h4><?php print t('Signatures needed by !date to reach goal of !total_needed', array('!date' => $date, '!total_needed' => $total_needed)); ?></h4>
          <div class="num-block num-block1"><?php print $total_needed; ?></div>
        </div>
        <div class="col-2" id="total-on">
          <h4><?php print t('Total signatures on this petition'); ?></h4>  
          <div class="num-block num-block2">1</div>
        </div>  
      </div>        
  </div>
        
  <p><?php print t('Use email, and social media sites like Facebook and Twitter to promote this petition and get to !signatures_needed signatures by !date_needed', array('!signatures_needed' => $total_needed, '!date_needed' => $date)); ?></p> 

  <div id="share-this-petition">
    <ul>
      <li class="title"><?php print t('Promote this Petition'); ?></li>
      <li class="share-link twitter"><?php print $twitter_link; ?></li>
      <li class="share-link facebook"><?php print $facebook_link; ?></li>
    </ul>
  </div><!--/share this petition-->
</div><!--/basic text block-->
