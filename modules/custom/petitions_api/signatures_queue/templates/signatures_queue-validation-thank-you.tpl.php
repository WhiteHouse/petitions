<?php

/**
 * @file
 * Default theme implementation for the signature verification thank you page.
 *
 * Available variables:
 * - $page_title: The page title.
 * - $message: The thank you message body.
 * - $email_link: Email share hyperlink.
 * - $twitter_link: Twitter share hyperlink.
 * - $facebook_link: Facebook share hyperlink.
 */
?>
<h1 class="title"><?php print $page_title; ?></h1><!--/title-->
<?php print $message; ?>

<div class="petition-detail">
  <div id="share-this-petition">
    <ul>
      <li class="share-link email"><?php print $email_link; ?></li>
      <li class="share-link twitter"><?php print $twitter_link; ?></li>
      <li class="share-link facebook"><?php print $facebook_link; ?></li>
    </ul>
  </div><!--/share this petition-->
</div><!--/petition detail-->
