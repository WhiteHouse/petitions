<h1 class="title"><?php print t('Thank you for signing a petition!'); ?></h1><!--/title-->
<p><?php print t('To further support <a href="@petition_link">"!title"</a> and to help it meet the threshold to get a response, use the share buttons below to share the petition you just signed with your networks.', array('@petition_link' => $petition_link, '!title' => $title)); ?></p>

<div id="share-this-petition">
  <ul>
    <li class="share-link twitter"><?php print $twitter_link; ?></li>
    <li class="share-link facebook"><?php print $facebook_link; ?></li>
  </ul>
</div><!--/share this petition-->
