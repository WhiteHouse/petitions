<?php

/**
 * @file
 * wh-petitions-page-petition.tpl.php
 *
 * Available variables:
 *
 * $path_to_petitions_responsive: Dynamically generates path to petitions_responsive theme.
 * $signature_form: rendered form to be displayed on signing section.
 */

?>
<div id="petition-wrapper">
  <?php if ($has_reported):
      $atts = array('attributes' => array('class' => 'arrow'));
      if (stripos($reported_link, 'http://') !== FALSE || stripos($reported_link, 'https://') !== FALSE) { $atts = array('attributes' => array('class' => 'arrow no-follow')); }
    ?>
    <div class="top-msg-bar top-msg-bar-reported"><strong><?php print t('Thank you for reporting this petition as inappropriate.') . '</strong>' . ' ' . l(t('Learn More'), $reported_link, $atts); ?></div><!--/top msg bar-->
  <?php endif; ?>

  <div id="petitions-individual">
    <div id="petition-detail">
      <?php print $petition; ?>
    </div>

    <div class="petition-detail">
    <?php if(!$responded && !$archived && !$flagged): ?>
      <?php if(!$already_signed): ?>
        <div id="sign-this-petition" class="clearfix">
          <div><?php print $signature_form; ?></div>
        </div>
      <?php else: ?>
        <div id="signed-by-user">
          <div id="sign-this-petition" class="clearfix">
          </div><!--/sign this petition-->
      <?php endif; ?>

      <?php if($has_share_bar): ?>
        <div id="share-this-petition">
          <ul>
            <li class="title"><?php print t('Promote this Petition'); ?></li>
            <li class="share-link twitter"><?php print $twitter_link; ?></li>
            <li class="share-link facebook"><?php print $facebook_link; ?></li>
          </ul>
          <?php if(!empty($inappropriate_link)): ?>
          <div class="report-petition">
            <?php print l(t('Report this Petition as Inappropriate'), 'petition/inappropriate/' . $petition_id, array('attributes' => array('id' => 'report-inappropriate-link', 'class' => array('no-follow')))); ?>
            <div id="report-petition-overlay" class="display-none">
              <span class="close-button"><?php print t('Close'); ?></span>
              <div class="entry">
                <h2 class="alert"><?php print t('You are about to report this petition.'); ?></h2>
                <?php print $inappropriate_form; ?>
              </div><!--/entry-->
            </div><!--/report petition overlay-->
          </div><!--/report petition-->
          <?php endif; ?>
        </div><!--/share this petition-->
        <div class="clear divide-green-fuz"></div>
      <?php endif; ?>

      <?php if($already_signed): ?></div><?php endif; ?>
    <?php endif; ?>

    <?php if($responded): ?>
      <div id="share-this-petition">
        <ul>
          <li class="title"><?php print t('Share this Petition'); ?></li>
          <li class="share-link twitter"><?php print $twitter_link; ?></li>
          <li class="share-link facebook"><?php print $facebook_link; ?></li>
        </ul>
        <div class="report-petition"><?php print $inappropriate_link; ?></div><!--/report petition-->
      </div><!--/share this petition-->

      <div class="petition-response petition-detail">
        <img class="response-call" alt ="" src="<?php print $path_to_petitions_responsive; ?>/img/bg-response-call.gif">
        <div class="response-author-org clearfix">
          <?php print $agency_seal; ?>
          <span class="official"><?php print t('Official !agency_name Response to', array('!agency_name' => $agency_name)); ?></span><!--/official-->
          <span class="petition-title"><?php print $petition_title; ?></span><!--/petition title-->
        </div><!--/response author-org-->

        <h2 class="title"><?php print $response_title; ?></h2>

        <?php if($response_intro): ?>
          <p><?php print $response_intro; ?></p>
        <?php endif; ?>

        <?php if (!empty($response_video)): ?>
          <div class="media"><?php print $response_video; ?></div>
        <?php endif; ?>
        <?php if (!empty($response_audio)): ?>
          <div class="media"><?php print $response_audio; ?></div>
        <?php endif; ?>

        <?php print $response_body; ?>
      </div>
    <?php endif; ?>
    </div>
  </div>
</div>
