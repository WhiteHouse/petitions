<!DOCTYPE html>
<html>
<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0" />
</head>
<body class="<?php print $classes; ?>" <?php print $attributes; ?>>

  <div id="page" class="<?php print $classes; ?>"<?php print $attributes; ?>>

    <!-- ______________________ HEADER _______________________ -->

    <header id="header">
      <div class="container">

        <?php if ($logo || $site_name || $site_slogan): ?>
          <div id="name-and-slogan">

            <?php if ($logo): ?>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
                <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>"/>
              </a>
            <?php else: /* Use h1  to show site name if there is no logo */ ?>
              <h1 id="site-name">
                <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><?php print $site_name; ?></a>
              </h1>
            <?php endif; ?>

            <?php if ($site_slogan): ?>
              <div id="site-slogan"><?php print $site_slogan; ?></div>
            <?php endif; ?>

          </div>
        <?php endif; ?>

      </div>
    </header> <!-- /header -->

    <!-- ______________________ MAIN _______________________ -->

    <div id="main">
      <div class="container">
        <section id="content">

          <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
          <?php print $content; ?>
          <?php if ($messages): ?>
            <div id="messages"><div class="section clearfix">
              <?php print $messages; ?>
            </div></div>
          <?php endif; ?>

        </section> <!-- /content-inner /content -->

      </div>
    </div> <!-- /main -->

  </div> <!-- /page -->

</body>

</html>
