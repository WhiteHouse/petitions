<?php
/**
 * @file
 * wh-petitions-display-petition-info.tpl.php
 *
 * Available variables:
 * $path_to_petitions44: Dynamically generates path to petitions44 theme.
 */
?>
<?php if(!$flagged): ?>
  <?php if($archived): ?>
    <div class="top-msg-bar top-msg-bar-red"><?php print t('This petition has been archived because it did not meet the signature requirements.'); ?></div><!--/top msg bar-->
  <?php endif; ?>
  <?php if($responded): ?>
    <div class="top-msg-bar top-msg-bar-blue"><?php print t('This petition has been responded to by the White House. <span>See the response below.</span>'); ?></div>
  <?php endif; ?>

  <div class="petition-detail petition-detail-margin-right">
    <?php if(empty($published)): ?>
      <div class="top-msg-bar top-msg-bar-red"><?php print t('Your petition has not been published yet.  Publish it below.'); ?></div>
    <?php endif; ?>
    <h4><?php print t('we petition the obama administration to:'); ?></h4>
    <h1 class="title"><?php print $title; ?></h1>
    <?php print $body; ?>
    <?php if(!empty($published)): ?>
      <div class="date"><strong><?php print t("Published Date:"); ?></strong> <?php print $published; ?></div>
    <?php endif; ?>
    <div class="issues"><strong><?php print t("Issues:"); ?></strong> <?php print $issues; ?></div>

    <?php if($admin): ?>
      <div class="issues"><strong><?php print t("Petition Creator Keywords:"); ?></strong> <?php print $user_tags; ?></div>
      <div class="issues"><strong><?php print t("Private Keywords:"); ?></strong> <?php print $private_tags; ?></div>
      <?php if($reached_ready): ?><div class="issues"><strong><?php print t("Became Ready For Response:"); ?></strong> <?php print $reached_ready; ?></div><?php endif; ?>
      <?php if (!empty($govdelivery)): ?>
        <div class="issues"><strong><?php print t("GovDelivery Topic:"); ?></strong> <?php print $govdelivery; ?></div>
      <?php endif; ?>
      <div class="issues"><?php print $admin_link; ?></div>
    <?php endif; ?>

    <?php if($published && !$archived && !$responded): ?>
      <div class="learn-more-about">
        <div class="view-threshold-tooltip">
          <?php print l(t("Learn about Petition Thresholds"), $petition_goals_link, array('attributes' => array('class' => array('no-follow')))); ?>
          <div id="threshold-tooltip" class="tooltip display-none no-follow">
            <?php print variable_get('wh_petitions_tooltip_threshold', ''); ?>
            <div class="tip-arrow-down"><img src="<?php print $path_to_petitions44; ?>/img/tip-arrow-down.gif"></div>
          </div>
        </div>
      </div>
    <?php endif; ?>
    <?php if($responded): ?>
      <div id="total-count">
        <h4><?php print t('total signatures'); ?></h4>
        <div class="num-block"><?php print $signatures; ?></div><!--/num block-->
      </div><!--/total count-->
    <?php endif; ?>

    <?php if(!$archived && !$responded): ?>
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
    <?php endif; ?>
  </div>
<?php else: ?>
 <div id="petitions-removed">
   <?php print $flagged_text; ?>

   <?php print $flagged_text_owner; ?>
   <a id="button-view-white-house-petitions" href="/petitions"><?php print t('View White House Petitions'); ?></a>
 </div>
<?php endif; ?>
