<?php
/**
 * @file
 * petitions44 footer static HTML storage file. This file houses the HTML that
 * is being used to generate the header variable in the page.tpl.php
 * file.
 *
 * Available variables:
 * $path_to_fortyfour: Dynamically generates path to fortyfour theme.
 */

/**
 * @file
 * Petitions 44 theme's implementation to display header content within the
 * page.tpl.php file.
 */
?>

<!-- whitehouse wh-header -->
  <header class="panel-panel panel-header">
    <div class="panel-pane pane-panels-mini pane-thin-header" >
      <div class="whr-header-bg">
        <div class="whr-wrapper">
          <div class="whr-contact-slogan">
            <div class="panel-pane pane-page-slogan" >
              the <span class="whr-lead">WHITE HOUSE</span><span class="whr-president">President Barack Obama</span>
            </div>

            <nav class="whr-contact-nav" >
              <div class="menu-block-wrapper menu-block-ctools-sixteenhundred-contact-nav-1 menu-name-sixteenhundred-contact-nav parent-mlid-0 menu-level-1">
            <ul class="menu"><li class="menu__item is-leaf first leaf menu-mlid-6476 horizontal-1-level-menu--menu-item whr-contact-nav--horizontal-1-level-menu--menu-item odd depth-1 no-children" data-position="1"><a href="http://www.whitehouse.gov/contact" title="Contact the Whitehouse." class="menu__link">Contact Us</a></li>
          <li class="menu__item is-leaf last leaf menu-mlid-6481 horizontal-1-level-menu--menu-item whr-contact-nav--horizontal-1-level-menu--menu-item even depth-1 no-children" data-position="2"><a href="http://www.whitehouse.gov/email-updates" title="Get Email Updates." class="menu__link">Get Email Updates</a></li>
          </ul></div>
            </nav>
          </div>
        </div>
        <div class="whr-navbar">
          <div class="whr-wrapper">
            <div class="panel-pane pane-page-logo" >
  <a href="/" rel="home" id="logo" title="Home"><img src="https://www.whitehouse.gov/profiles/forall/modules/custom/gov_whitehouse_www/images/icons/wh_logo_seal.png" alt="Home" /></a>
            </div>
            <div id="block-menu-block-sixteenhundrednav-main" class="">
              <div id="main-menu-header" class="clearfix">
                <div id="icon-wrapper">
                  <a href="#" role='tab' id="main-menu-toggle"><i class="fa fa-bars"></i></a>
                </div>
              </div>
              <div id="main-menu-content">
  <?php
    $block_header_menu = module_invoke('menu_block', 'block_view', 'wh_global_nav_header_block-1');
    print render($block_header_menu['content']);
  ?>
              </div>
            </div>
            <div class="panel-pane pane-block pane-search-form sitewide-header--search-form" >
              <form action="https://search.whitehouse.gov/search" method="get" id="search-block-form" accept-charset="UTF-8"><div><div class="container-inline">
                    <h2 class="element-invisible">Search form</h2>
                    <div class="form-item form-type-textfield form-item-query">
                      <label class="element-invisible" for="edit-search-block-form--2">Search </label>
                      <input title="Enter the terms you wish to search for." type="text" id="edit-search-block-form--2" name="query" value="" size="15" maxlength="128" class="form-text" />
                    </div>
                    <div class="form-actions form-wrapper" id="edit-search-actions"><input type="submit" id="edit-search-submit" name="op" value="Search" class="form-submit" /></div><input type="hidden" name="affiliate" value="wh" />
                  </div>
                </div>
              </form>
            </div>
          </div>
        </div>
      </div>
    </div>
  </header>
<!-- /whitehouse wh-header -->
