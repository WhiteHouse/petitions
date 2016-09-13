<div id="page" class="<?php print $classes; ?>"<?php print $attributes; ?>>

  <!-- ______________________ HEADER _______________________ -->

  <?php if ($page['preface']): ?>
    <div id="preface">
      <?php print render($page['preface']); ?>
    </div>
  <?php endif; ?> <!-- /preface -->

  <header id="header" class="header">
    <div id="header-mobile-bg"></div>
    <div class="header-container">
      <div id="header-region" class="header-region container">
        <?php print render($page['header']); ?>
        <div class="logo">
          <div class="logo--desktop"><a href="<?php print $base_url ?>"><img alt="We The People logo" src="<?php print $theme_path ?>/images/wtp_logo_desktop.png"></a></div>
          <div class="logo--mobile"><a href="<?php print $base_url ?>"><img alt="We The People logo" src="<?php print $theme_path ?>/images/wtp_logo_mobile.png"></a></div>
        </div>

        <?php if ($main_menu || $secondary_menu): ?>
          <nav id="navigation" class="menu <?php if (!empty($main_menu)) {print "with-primary";}
              if (!empty($secondary_menu)) {print " with-secondary";} ?>">
            <?php print theme('links', array('links' => $main_menu, 'attributes' => array('id' => 'primary', 'class' => array('main-menu')))); ?>
            <?php print theme('links', array('links' => $secondary_menu, 'attributes' => array('id' => 'secondary', 'class' => array('sub-menu')))); ?>
          </nav> <!-- /navigation -->
        <?php endif; ?>
      </div>
    </div>
    <div class="tagline">
     <div class="container">
       <div class="tagline--desktop"><a href="<?php print $base_url ?>"><img alt="Your Voice in the White House tagline" src="<?php print $theme_path ?>/images/wtp_tagline_desktop.png"></a></div>
       <div class="tagline--mobile"><a href="<?php print $base_url ?>"><img alt="Your Voice in the White House tagline" src="<?php print $theme_path ?>/images/wtp_tagline_mobile.png"></a></div>
       <div class="menu--user">
         <?php print render($user_menu); ?>
       </div>
     </div>
    </div>
  </header> <!-- /header -->

  <!-- ______________________ MAIN _______________________ -->

  <div id="main">
    <div class="container">
      <?php if($logged_in): ?>
        <?php print render($dashboard_menu); ?>
      <?php endif; ?>

      <?php if ($page['content_top'] || $breadcrumb || $title || $messages || $tabs): ?>
        <header id="content-top">
          <?php print $breadcrumb; ?>

          <?php if (render($tabs)): ?>
            <div class="tabs"><?php print render($tabs); ?></div>
          <?php endif; ?>

          <?php print render($title_prefix); ?>

          <?php if ($title): ?>
            <h1 class="title"><?php print $title; ?></h1>
          <?php endif; ?>

          <?php print render($title_suffix); ?>
          <?php print $messages; ?>

          <?php if ($page['content_top']): ?>
            <?php print render($page['content_top']); ?>
          <?php endif; ?>
        </header>
      <?php endif; ?> <!-- /content-top -->

      <?php if ($page['help']): ?>
        <div id="help">
          <?php print render($page['help']); ?>
        </div>
      <?php endif; ?> <!-- /help -->

      <?php if ($page['sidebar_top']): ?>
        <aside id="sidebar-top">
          <?php print render($page['sidebar_top']); ?>
        </aside>
      <?php endif; ?> <!-- /sidebar-top -->

      <section id="content">

          <?php if ($action_links): ?>
              <?php if ($action_links): ?>
                <ul class="action-links"><?php print render($action_links); ?></ul>
              <?php endif; ?>
          <?php endif; ?>

          <div id="content-main">
            <?php print render($page['content']) ?>
          </div>

      </section> <!-- /content-inner /content -->

      <?php if ($page['sidebar_bottom']): ?>
        <aside id="sidebar-bottom">
          <?php print render($page['sidebar_bottom']); ?>
        </aside>
      <?php endif; ?> <!-- /sidebar-bottom -->

      <?php if ($page['content_bottom']): ?>
        <div id="content-bottom">
          <?php print render($page['content_bottom']); ?>
        </div>
      <?php endif; ?> <!-- /content-bottom -->
    </div> <!-- /container -->
  </div> <!-- /main -->

  <!-- ______________________ FOOTER _______________________ -->

  <div class="container"><a href="#main" class="return-top"><?php print t('Return to top'); ?></a></div>
  <?php if ($page['footer']): ?>
    <footer id="footer">
      <div class="container">
      <?php print render($page['footer']); ?>
      </div>
    </footer> <!-- /footer -->
  <?php endif; ?>

  <?php if ($page['appendix']): ?>
    <div id="appendix">
      <?php print render($page['appendix']); ?>
    </div>
  <?php endif; ?> <!-- /appendix -->

</div> <!-- /page -->
