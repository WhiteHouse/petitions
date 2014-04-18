<?php

/**
 * @file
 * wh-petitions-page-petition.tpl.php
 *
 * Available variables:
 *
 * $path_to_petitions44: Dynamically generates path to petitions44 theme.
 * $login_required: TRUE if login is required or FALSE if set to simplified signing.
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
          <?php if ($logged_in && $login_required): ?>
          <div class="col-full">
            <?php print l(t('Sign This Petition'), 'petition/sign/' . $petition_id, array('attributes' => array('rel' => $petition_id, 'id' => 'button-sign-this-petition', 'class' => array('no-follow')))); ?>
          </div><!--/col-2-->
          <div class="col-right"> </div>
          <p><?php print t('Note: When you sign this petition, your first name, last initial and city and state will be publicly displayed on the petition page. Once you sign a petition, your signature cannot be removed.'); ?></p>
          <div class="display-none modal" id="thank-you-modal">
            <div class="header">
              <h2><?php print t('Thank you for signing this petition'); ?></h2>
              <div class="close-button"><?php print t('Close'); ?></div>
            </div><!--/header-->
            <div class="inner">
      <div class="sig-num clearfix">
        <div class="col-2" id="sig-needed">
          <h4><?php print t('Signatures needed by !date to reach goal of !total_needed', array('!date' => $end_date, '!total_needed' => $total_needed)); ?></h4>
          <div class="num-block num-block1"><?php print $signatures_needed; ?></div>
        </div>
        <div class="col-2" id="total-on">
          <h4><?php print t('Total signatures on this petition'); ?></h4>
          <div class="num-block num-block2"><?php print $signatures; ?></div>
        </div>
      </div>
              <div id="comment-form-wrapper">
                <?php print $comment_form; ?>
              </div>
              <h3><?php print t('Email this link to your friends and family:'); ?> <?php print $short_url; ?></h3>
              <h3><?php print t('Or use social media sites like Facebook and Twitter to help promote this petition.'); ?></h3>
              <ul class="share-this-petition">
                <li class="share-link twitter"><?php print $twitter_link; ?></li>
                <li class="share-link facebook"><?php print $facebook_link; ?></li>
              </ul><!--/share this petition-->
            </div><!--/inner-->
          </div><!--/thank you modal-->
          <?php else: ?>
            <?php if ($login_required): ?>
              <div class="col-left">
                <a id="button-sign-this-petition-inactive" href="/user" class="no-follow inactive-margin"><?php print t('Sign this Petition'); ?></a>
              </div><!--/col-2-->
              <div class="col-right">
                <h3><?php print t('A whitehouse.gov account is required to sign Petitions.'); ?></h3> <div id="why-overlay-text"><?php print t('WHY?'); ?></div>
                <div id="why-overlay" class="display-none">
                  <div class="entry"><?php print $why_text; ?></div>
                </div><!--/#why overlay-->
                <div class="buttons">
                  <?php print l(t('Sign In'), 'user', array('query' => $return_destination, 'attributes' => array('class' => array('no-follow'), 'id' => 'button-sign-in'))); ?>
                  <div class="or"><?php print t('or'); ?></div><!--/or-->
                  <?php print l(t('Create an Account'), 'user/register', array('query' => $return_destination, 'attributes' => array('class' => array('no-follow'), 'id' => 'button-create-an-account'))); ?>
                </div><!--/buttons-->
              </div><!--/col-2-->
              <h3 class="clearfix" style="float: left"><?php print t('If you\'re logged in, but having trouble signing this petition, <a href="/how-why/frequently-asked-questions">click here for help.</a>') ?></h3><br />
            <?php else: ?>
              <div><?php print $signature_form; ?></div>
            <?php endif; ?>
          <?php endif; ?>
        </div>
      <?php else: ?>
        <div id="signed-by-user">
          <div id="sign-this-petition" class="clearfix">
            <?php if ($login_required): ?>
              <h3><?php print t("You've already signed this petition"); ?></h3>
              <p><?php print t("Thank you for participating.  Find other petitions you're interested in or start your own."); ?></p>
            <?php endif; ?>
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
        <img class="response-call" alt ="" src="<?php print $path_to_petitions44; ?>/img/bg-response-call.gif">
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

    <?php if($archived): ?>
      <div id="closed-signatures" class="closed">
    <?php endif; ?>
    <?php if(!$flagged): ?>
      <div id="signature-list" class="clearfix">
        <div class="numbers">
          <?php print t('Signatures: !count of !total', array('!count' => '<span class="count">' . $signature_count . '</span>', '!total' => '<span class="total-count">' . $signature_total . '</span>')); ?>
        </div><!--/numbers-->

        <?php print $signature_html; ?>
      </div>
    <?php endif; ?>
    <?php if($archived): ?>
      </div>
    <?php endif; ?>
    </div>
  </div>
</div>
