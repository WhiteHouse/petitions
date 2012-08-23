<?php if ($no_responses): ?>
  <?php print $no_response_text; ?>
<?php else: ?>
<div class="response-filter" id="filter-list">
  <ul class="select-view">
    <li class="active featured"><div><?php print t("Featured Responses"); ?></div></li>
  </ul>
  <ul class="filter-list">
    <?php if ($cols == 2) $atts = array('attributes' => array('class' => 'active no-follow')); else $atts = array('attributes' => array('class' => 'no-follow')); ?>
    <li class="view-two-column"><?php print l(t("2 Col"), "responses/" . $sort . '/' . $page . '/2/' . $issues . '/' . $search . '/', $atts); ?></li>
    <?php if ($cols == 1) $atts = array('attributes' => array('class' => 'active no-follow')); else $atts = array('attributes' => array('class' => 'no-follow')); ?>
    <li class="view-one-column"><?php print l(t("1 Col"), "responses/" . $sort . '/' . $page . '/1/' . $issues . '/' . $search . '/', $atts); ?></li>
    <li class="filter-by-issue">
      <?php print l(t('Filter by Issue'), 'filter-issues/responses', array('attributes' => array("class" => "popup-title no-follow"))); ?>
    </li>
    <li class="search">
      <?php print l(t('Search'), 'filter-search/responses', array('attributes' => array("class" => "popup-title no-follow"))); ?>
    </li>
  </ul>

  <div class="drop-down display-none" id="issues-filter-drop">
    <div id="issues-filter-top"></div>
    <div id="issues-filter-mid" class="clearfix">
      <div class="header">
        <h2><?php print t('All Issues'); ?></h2>
        <div class="close-button"><?php print t('Close'); ?></div>
      </div><!--/header-->
                           
      <?php print $issues_form; ?>
    </div>
    <div id="issues-filter-bottom"></div>    
  </div>  
  <div class="drop-down display-none" id="search-drop">
    <div class="close-button"><?php print t('Close'); ?></div>
    <?php print $search_form; ?>
  </div><!--/search drop-->     
</div><!--/response filter-->
              
<div class="petition-list clearfix">
  <div class="responses">
    <?php if ($cols == 1): ?>
      <div class="full-page-list">
    <?php endif; ?>
    <?php print $responses; ?>
    <?php if ($cols == 1): ?>
      </div>
    <?php endif; ?>    
  </div>

  <div id="petition-bars"> 
    <div class="display-none" id="page-num"><?php print $page; ?></div>
  <?php if ($next_page): ?>
    <?php print l(t('Viewing !count of !total: Show More Responses', array('!count' => '<span id="response-count">' . $count . '</span>', '!total' => $total)), "responses/".$sort."/".($page+1)."/" . $cols . '/' . $issues . '/' . $search, array("html" => TRUE, "attributes" => array("class" => "clear show-more-responses-bar no-follow"))); ?>
  
    <div class="clear loading-more-petitions-bar display-none">
      <?php print t('loading more responses...'); ?>
    </div><!--show more responses bar-->
  <?php endif; ?>
  </div>
</div>
<?php endif; ?>