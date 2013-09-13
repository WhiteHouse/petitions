<?php
/**
 * @file
 * Petitions 44 theme's implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * Fortyfour theme specific defined theme settings (see README.txt in fortyfour
 * theme
 * for more details)
 * - $fortyfour_page_wrapper_class: CSS class id to use when creating the page
 *   wrappers
 * - $fortyfour_header: Fortyfour header elements
 * - $fortyfour_header_menu: Fortyfour header navigation menu
 * - $fortyfour_subfooter_menu: Fortyfour subfooter navigation menu
 * - $fortyfour_footer_menu: Fortyfour theme footer navigation menu
 * - $fortyfour_use_microsite_banner: whether or not to use the microsite banner
 *
 * Pettiions 44 variables:
 * - $petitions44_help_text: HTML
 *
 * Is Not Cached module provides:
 * - $is_not_cached
 */
?>

<?php if ($fortyfour_use_microsite_banner): ?>
  <div class="banner-bg">
    <div id ="banner" role="banner" class="region-banner clearfix">

      <?php if ($secondary_menu): ?>
        <nav id="secondary-menu" role="navigation">
          <?php print theme('links__system_secondary_menu', array(
            'links' => $secondary_menu,
            'attributes' => array(
              'class' => array('links' , 'clearfix'),
            ),
            'heading' => array(
              'text' => $secondary_menu_heading,
              'level' => 'h2',
              'class' => array('menu-title', 'arrow-down'),
            ),
          )); ?>
        </nav>
      <?php endif; ?>
      <!-- /external links menu-->
      <span class="flag">President Barack Obama</span>
    </div><!--End banner-->
  </div><!--End banner-bg-->
<?php endif; ?>

<div id="<?php echo rtrim($fortyfour_page_wrapper_class) . '-page' ?>" class="page-wrapper">
  <div class="center-on-page clearfix" id="page-inner">
  <div id="wh-header" class="clearfix">
    <?php if ($fortyfour_header): ?>
      <?php print render($fortyfour_header); ?>
    <?php endif; ?>
    <?php if ($fortyfour_mainnav): ?>
      <?php print render($fortyfour_mainnav); ?>
    <?php endif; ?>
    <?php if ($page['takeover']): ?>
      <div class="takeover"><?php print render($page['takeover']); ?></div>
    <?php endif; ?>
    <?php if ($page['header']): ?>
      <?php print render($page['header']); ?>
    <?php endif; ?>
  </div><!--wh-header-->

<div id="petition-header" class="clearfix">
  <div class="col-1">
    <a class="intro" href="<?php print $base_path ?>">
      <?php print t("We the people: your voice in our government"); ?>
    </a>
    <!--/intro-->
    <?php if ($main_menu): ?>
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'main-menu-links',
            'class' => array('links', 'user-state-links', 'clearfix'),
          ),
        )); ?>
    <?php endif; ?>
    <!--/petition-nav-->
  </div>
  <!--/col 1-->

  <div class="col-2">
    <div class="text">
      <?php if ($petitions44_help_text): ?>
        <?php print $petitions44_help_text; ?>
      <?php endif; ?>
    </div>
    <!--/text-->
    <div class="share-link link-wrapper">
      <span class="view-my-link">
        <a href="http://www.whitehouse.gov/feedback-petitions" class="wh-feedback-link no-follow" target="_blank">Share Your Feedback</a>
      </span>
    </div>
    <div class="link-wrapper">
      <?php if ($secondary_menu): ?>
        <?php print theme('links__system_secondary_menu', array(
          'links' => $secondary_menu,
          'attributes' => array(
            'id' => 'secondary-menu-links',
            'class' => array(
              'secondary-links',
              'user-state-links',
              'inline',
              'clearfix',
            ),
          ),
        )); ?>
      <?php endif; ?>
    </div>
    <!--/link wrapper-->
  </div>
  <!--/col 2-->
</div>
<!--/petition header-->

<?php if (!$is_front): ?>
  <div id="main">
<?php endif; ?>

    <?php if (!empty($is_not_cached)): ?>
      <?php print $messages; ?>
    <?php endif;?>

    <?php if (user_is_logged_in()): ?>
      <?php print render($tabs); ?>
      <?php print render($page['help']); ?>
      <?php if ($action_links): ?>
        <ul class="action-links"><?php print render($action_links); ?></ul>
      <?php endif; ?>
    <?php endif; ?>

    <div class="clearfix" id="<?php echo rtrim($fortyfour_page_wrapper_class) . '-outer' ?>">
      <div class="clearfix" id="<?php echo rtrim($fortyfour_page_wrapper_class) . '-inner' ?>">

        <?php if ($is_front): ?>
          <div class="petition-landing-wrapper grid-container clearfix">
          <?php print render($page['content']); ?>

        <?php else: ?>
          <div id="main-content-wrapper" class="column clearfix main-content" role="main">
            <a id="main-content"></a>
            <?php print render($page['content']); ?>
          </div><!-- /#content -->
        <?php endif; ?>

        <?php if ($page['right_rail']): ?>
            <aside class="right-rail">
              <div class="container">
                <?php print render($page['right_rail']); ?>
              </div>
            </aside><!-- /.right-rail -->
        <?php endif; ?>

      </div><!-- /#$fortyfour_page_wrapper-inner -->
    </div><!-- /#$fortyfour_page_wrapper-outer -->

<?php if (!$is_front): ?>
  </div><!-- /#main -->
<?php endif; ?>

<div class="bottom-divider"></div>
<div class="footer-wrap">
  <div class="footer-inner">
  <?php print render($page['footer']); ?>
  <div class="footer-bottom">
    <?php print render($page['bottom']); ?>
    <?php if ($fortyfour_footer): ?>
      <?php print render($fortyfour_footer); ?>
    <?php endif; ?>
    <?php if ($fortyfour_subfooter): ?>
      <?php print render($fortyfour_subfooter); ?>
    <?php endif; ?>
  </div>
  </div>
</div>

  </div><!--/center on page-->
</div><!--/petition-page -->
