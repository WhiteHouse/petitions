<?php

/**
 * @file
 * wh-petitions-page-thank-you.tpl.php
 *
 * Available variables:
 *
 * $petition_thankyou_top: Region above content
 * $thank_you_text: Top message text
 * $petition_title: Title of petition.
 * $petition_url: Absolute URL to the petition.
 * $short_url: Shortened URL to the petition.
 * $share_links: Social share links
 * $petition_below_social: Region below social links
 */

?>

<div class="petition-thankyou" id="petition-thankyou">
<?php if (!empty($petition_thankyou_top)): ?>
  <div class="petition-thankyou-top">
  <?php print $petition_thankyou_top; ?>
  </div>
<?php endif; ?>

  <div class="top-msg-bar"><span class="fa fa-check-circle fa-2x"></span><br><?php print $thank_you_text; ?></div><!--/top msg bar-->
  <h1><?php print l($petition_title, $petition_url, array('html' => TRUE, 'attributes' => array('target' => '_top'))); ?></h1>
  <div class="petition-social-share" id="share-this-petition">
    <p class="large"><?php print t('Help the petition reach its goal, share with others:'); ?></p>
    <div class="petition-short-url"><p><?php print t('Short URL:');?> <?php print l($short_url, $short_url); ?></p></div>
    <div id="share-this-petition">
      <?php print render($share_links); ?>
    </div>
  </div>
<?php if (!empty($petition_below_social)): ?>
  <div class="petition-below-social">
  <?php print $petition_below_social; ?>
  </div>
<?php endif; ?>
</div>
