<?php
/**
 * @file
 * Zen theme's implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   - view-mode-[mode]: The view mode, e.g. 'full', 'teaser'...
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 *   The following applies only to viewers who are registered users:
 *   - node-by-viewer: Node is authored by the user currently viewing the page.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $petition_title: the (sanitized) title of the petition node.
 * - $body: Field variable: Main content of response with <p> tags.
 * - $nid: $node->nid
 * - $petition_id: Legacy ID if available, $node->nid if not.
 * - $legacy_id: Legacy ID if available, FALSE if not.
 * - $short_url: Short URL if available, '' if not.
 * - $issues: Comma separated issues, linked to respective faceted search.
 * - $date_published: Time the petition was published, FALSE if not published.
 * - $flagged: TRUE or FALSE, whether the petition has been flagged for review.
 * - $flagged_text: Text to display if petition is flagged.
 * - $flagged_text_owner: Text to display to petition owner if petition is flagged.
 * - $responded: TRUE or FALSE, whether the petition has been responded to.
 * - $archived: TRUE or FALSE, whether the petition status is WH_PETITION_STATUS_CLOSED.
 * - $admin: TRUE or FALSE, whether the current user has permission to administer petitions.
 * - $admin_link: HTML link to administer petitions.
 * - $reached_ready: The time the petition reached 'ready for review' status, '' if not.
 * - $end_date: Deadline for the petition to receive signatures.
 * - $total_needed: Total signatures needed to obligate a response.
 * - $signatures_needed: Remaining signatures needed to obligate a response.
 * - $signature_count: Quantity of signatures currently displayed.
 * - $signature_total: Total quantity of signatures.
 * - $already_signed: Whether the current user signed the petition.
 * - $signature_form: Petition signature form used login is required.
 * - $comment_form: Comment form used when commenting is enabled.
 * - $why_text: Tooltip 'Why?' text.
 * - $inappropriate_form: Form for flagging the petition as inappropriate.
 * - $inappropriate_link: TRUE or FALSE, whether to display the 'Flag as inappropriate' link.
 * - $has_reported: Whether the current user has flagged the petition as inappropriate.
 * - $reported_link: Link displayed after reporting a petition as inappropriate.
 * - $petition_goals_link: Link to learn about petition thresholds.
 * - $has_share_bar: TRUE or FALSE, whether to include the share functionality.
 * - $login_required: TRUE or FALSE, whether logging in is required to sign the petition.
 * - $logged_in: TRUE or FALSE, whether the current user is a logged-in member.
 * - $response_title: Title of the response node, if petition is responded to.
 * - $response_body: Body of the response node, if petition is responded to.
 * - $response_audio: Field variable: Accompanying audio file for this response.
 * - $response_video: Field variable: Accompanying audio file for this response.
 * - $agency_name: The name of the agency that has generated this response
 * - $agency_seal: The seal of the agency that has generated this response
 * - $twitter_link: Link for user to submit this response to twitter
 * - $facebook_link:Link for user to submit this response to facebook
 * - $path_to_petitions44: Path to petitions44 theme.
 * - $return_destination: Return path for links away from the petition.
 * - $signature_html: HTML for displaying signatures at the bottom of petitions.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see zen_preprocess_node()
 * @see template_process()
 * @see wh_petitions_preprocess_node()
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
    <?php if(!$flagged): ?>
      <?php if($archived): ?>
        <div class="top-msg-bar top-msg-bar-red"><?php print t('This petition has been archived because it did not meet the signature requirements.'); ?></div><!--/top msg bar-->
      <?php endif; ?>
      <?php if($responded): ?>
        <div class="top-msg-bar top-msg-bar-blue"><?php print t('This petition has been responded to by the White House. <span>See the response below.</span>'); ?></div>
      <?php endif; ?>

      <div class="petition-detail petition-detail-margin-right">
        <?php if(empty($date_published)): ?>
          <div class="top-msg-bar top-msg-bar-red"><?php print t('Your petition has not been published yet.  Publish it below.'); ?></div>
        <?php endif; ?>
        <h4><?php print t('we petition the obama administration to:'); ?></h4>
        <h1 class="title"><?php print $petition_title; ?></h1>
        <?php print $body; ?>
        <?php if(!empty($date_published)): ?>
          <div class="date"><strong><?php print t("Published Date:"); ?></strong> <?php print $date_published; ?></div>
        <?php endif; ?>
        <div class="issues"><strong><?php print t("Issues:"); ?></strong> <?php print $issues; ?></div>

        <?php if($admin): ?>
          <?php if($reached_ready): ?><div class="issues"><strong><?php print t("Became Ready For Response:"); ?></strong> <?php print $reached_ready; ?></div><?php endif; ?>
          <?php if (!empty($govdelivery)): ?>
            <div class="issues"><strong><?php print t("GovDelivery Topic:"); ?></strong> <?php print $govdelivery; ?></div>
          <?php endif; ?>
          <div class="issues"><?php print $admin_link; ?></div>
        <?php endif; ?>

        <?php if($date_published && !$archived && !$responded): ?>
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
            <div class="num-block"><?php print $signature_total; ?></div><!--/num block-->
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
              <div class="num-block num-block2"><?php print $signature_total; ?></div>
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
          <div class="num-block num-block2"><?php print $signature_count; ?></div>
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
