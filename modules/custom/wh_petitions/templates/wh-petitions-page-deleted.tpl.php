<div class="petition-detail">
  <div class="top-msg-bar top-msg-bar-red"><?php print t('You have deleted this draft petition from the system'); ?></div><!--/top msg bar-->
  <p><?php print $delete_text; ?></p>
 
  <p><?php print l(t("Create a new petition"), 'petition/create'); ?> | <?php print l(t("View my petitions"), 'dashboard'); ?></p>
</div>