<div id="filter-list" class="petition-filter">
  <ul class="select-view">
    <li id="sort-all" <?php if(arg(1) != 'trending' && arg(1) != 'popular' && empty($issues)): ?>class="active"<?php endif; ?>><?php print l(t("All Petitions"), "petitions/all/0/" . $cols . '/' . $issues . '/' . $search); ?></li>
<!--    
<li id="sort-trending" <?php if(arg(1) == 'trending'): ?>class="active"<?php endif; ?>><?php print l(t("Trending"), "petitions/trending/0/" . $cols . '/' . $issues . '/' . $search); ?></li>
-->
    <li id="sort-popular" <?php if(arg(1) == 'popular'): ?>class="active"<?php endif; ?>><?php print l(t("Popular"), "petitions/popular/0/" . $cols . '/' . $issues . '/' . $search); ?></li>
  </ul>
            
  <ul class="filter-list">
    <?php if($cols == 2) $atts = array('attributes' => array('class' => 'active no-follow')); else $atts = array('attributes' => array('class' => 'no-follow')); ?>
    <li class="view-two-column"><?php print l(t("2 Col"), "petitions/" . $sort . '/' . $page . '/2/' . $issues . '/' . $search . '/', $atts); ?></li>
    <?php if($cols == 1) $atts = array('attributes' => array('class' => 'active no-follow')); else $atts = array('attributes' => array('class' => 'no-follow')); ?>
    <li class="view-one-column"><?php print l(t("1 Col"), "petitions/" . $sort . '/' . $page . '/1/' . $issues . '/' . $search . '/', $atts); ?></li>
    <li class="filter-by-issue">
      <?php print l(t('Filter by Issue'), 'filter-issues', array('attributes' => array("class" => "popup-title no-follow"))); ?>  
    </li>
    
    <li class="search">
      <?php print l(t('Search'), 'filter-search', array('attributes' => array("class" => "popup-title no-follow"))); ?>
    </li>
  </ul>  
  <div class="drop-down display-none" id="issues-filter-drop">
    <div id="issues-filter-top"></div>
    <div id="issues-filter-mid">
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
</div><!--/petition filter-->

<div class="petition-list clearfix">
  <div class="list-we-petition-msg"><?php print t('we petition the obama administration to:'); ?></div><!--/list we petition msg-->
  <div class="petitions" id="petition-wrapper">
    <?php if($cols == 1): ?>
      <div class="full-page-list">
    <?php endif; ?>
    <?php print $petitions; ?>
    <?php if($cols == 1): ?>
      </div>
    <?php endif; ?>    
  </div>
  
  <div id="petition-bars">
    <div class="display-none" id="page-num"><?php print $page; ?></div>
  <?php if($next_page): ?>
    <?php print l(t('Viewing !count of !total: Show More Petitions', array('!count' => '<span id="petition-count">' . $count . '</span>', '!total' => $total)), "petitions/".$sort."/".($page+1)."/" . $cols . '/' . $issues . '/' . $search, array("html" => TRUE, "attributes" => array("class" => "clear show-more-petitions-bar no-follow"))); ?>
  
    <div class="clear loading-more-petitions-bar display-none">
      <?php print t('loading more petitions...'); ?>
    </div><!--show more petitions bar-->
  <?php endif; ?>
  </div>
</div>
