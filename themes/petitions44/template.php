<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * A QUICK OVERVIEW OF DRUPAL THEMING
 *
 *   The default HTML for all of Drupal's markup is specified by its modules.
 *   For example, the comment.module provides the default HTML markup and CSS
 *   styling that is wrapped around each comment. Fortunately, each piece of
 *   markup can optionally be overridden by the theme.
 *
 *   Drupal deals with each chunk of content using a "theme hook". The raw
 *   content is placed in PHP variables and passed through the theme hook, which
 *   can either be a template file (which you should already be familiary with)
 *   or a theme function. For example, the "comment" theme hook is implemented
 *   with a comment.tpl.php template file, but the "breadcrumb" theme hooks is
 *   implemented with a theme_breadcrumb() theme function. Regardless if the
 *   theme hook uses a template file or theme function, the template or function
 *   does the same kind of work; it takes the PHP variables passed to it and
 *   wraps the raw content with the desired HTML markup.
 *
 *   Most theme hooks are implemented with template files. Theme hooks that use
 *   theme functions do so for performance reasons - theme_field() is faster
 *   than a field.tpl.php - or for legacy reasons - theme_breadcrumb() has "been
 *   that way forever."
 *
 *   The variables used by theme functions or template files come from a handful
 *   of sources:
 *   - the contents of other theme hooks that have already been rendered into
 *     HTML. For example, the HTML from theme_breadcrumb() is put into the
 *     $breadcrumb variable of the page.tpl.php template file.
 *   - raw data provided directly by a module (often pulled from a database)
 *   - a "render element" provided directly by a module. A render element is a
 *     nested PHP array which contains both content and meta data with hints on
 *     how the content should be rendered. If a variable in a template file is a
 *     render element, it needs to be rendered with the render() function and
 *     then printed using:
 *       <?php print render($variable); ?>
 *
 * ABOUT THE TEMPLATE.PHP FILE
 *
 *   The template.php file is one of the most useful files when creating or
 *   modifying Drupal themes. With this file you can do three things:
 *   - Modify any theme hooks variables or add your own variables, using
 *     preprocess or process functions.
 *   - Override any theme function. That is, replace a module's default theme
 *     function with one you write.
 *   - Call hook_*_alter() functions which allow you to alter various parts of
 *     Drupal's internals, including the render elements in forms. The most
 *     useful of which include hook_form_alter(), hook_form_FORM_ID_alter(),
 *     and hook_page_alter(). See api.drupal.org for more information about
 *     _alter functions.
 *
 * OVERRIDING THEME FUNCTIONS
 *
 *   If a theme hook uses a theme function, Drupal will use the default theme
 *   function unless your theme overrides it. To override a theme function, you
 *   have to first find the theme function that generates the output. (The
 *   api.drupal.org website is a good place to find which file contains which
 *   function.) Then you can copy the original function in its entirety and
 *   paste it in this template.php file, changing the prefix from theme_ to
 *   petitions44_. For example:
 *
 *     original, found in modules/field/field.module: theme_field()
 *     theme override, found in template.php: petitions44_field()
 *
 *   where petitions44 is the name of your sub-theme. For example, the
 *   zen_classic theme would define a zen_classic_field() function.
 *
 *   Note that base themes can also override theme functions. And those
 *   overrides will be used by sub-themes unless the sub-theme chooses to
 *   override again.
 *
 *   Zen core only overrides one theme function. If you wish to override it, you
 *   should first look at how Zen core implements this function:
 *     theme_breadcrumbs()      in zen/template.php
 *
 *   For more information, please visit the Theme Developer's Guide on
 *   Drupal.org: http://drupal.org/node/173880
 *
 * CREATE OR MODIFY VARIABLES FOR YOUR THEME
 *
 *   Each tpl.php template file has several variables which hold various pieces
 *   of content. You can modify those variables (or add new ones) before they
 *   are used in the template files by using preprocess functions.
 *
 *   This makes THEME_preprocess_HOOK() functions the most powerful functions
 *   available to themers.
 *
 *   It works by having one preprocess function for each template file or its
 *   derivatives (called theme hook suggestions). For example:
 *     THEME_preprocess_page    alters the variables for page.tpl.php
 *     THEME_preprocess_node    alters the variables for node.tpl.php or
 *                              for node--forum.tpl.php
 *     THEME_preprocess_comment alters the variables for comment.tpl.php
 *     THEME_preprocess_block   alters the variables for block.tpl.php
 *
 *   For more information on preprocess functions and theme hook suggestions,
 *   please visit the Theme Developer's Guide on Drupal.org:
 *   http://drupal.org/node/223440 and http://drupal.org/node/1089656
 */


/**
 * Override or insert variables into the page template.
 */
function petitions44_process_page(&$variables) {
  /*
  Load template asset variables from theme_settings if locally modified,
  otherwise from variable_get / strongarm export
  */

  // Special variables to set for page--front.tpl.php.
  // @todo move all the $is_front logic out of page.tpl.php, into here wherever possible.
  if ($variables['is_front']) {
    // @todo $page['content'] here is HTML. Make this a renderable array?
    $variables['page']['content'] = _petitions44_front_page_content();
  }

  $variables['petitions44_help_text'] = _petitions44_help_text();
}

/**
 * Override theme_menu_local_tasks().
 */
function petitions44_menu_local_tasks(&$variables) {
  $output = '';
  global $user;
  $arg = arg();

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#suffix'] = '</ul>';
    if ($arg[0] == 'user' && $arg[1] == $user->uid && $arg[2] == 'edit' ) { 
      $variables['secondary']['#prefix'] .= '<ul class="tabs petitions44-user-edit">';
    }
    else {
      $variables['secondary']['#prefix'] .= '<ul class="tabs secondary">';
    }
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}


/**
 * Help text can be set as a theme setting.
 *
 * If no setting is set by the end user, a module can provide default help text.
 * If help text is not provided by the user or a module, a default
 * is defined here.
 *
 * @return String
 *   Help text.
 */
function _petitions44_help_text() {
  // Check for user-defined header.
  if (!$petitions44_help_text = theme_get_setting('petitions44_help_text')) {
    // If no user-defined header exists, check to see if an included module is
    // providing defaults (this is useful for implmentations like petitions).
    $text = t('Help make We the People even better. Share your feedback on how this new platform can improve.');
    $petitions44_help_text = variable_get('petitions44_help_text', $text);
  }
  return $petitions44_help_text;
}

/**
 * Preprocessor for theme('page').
 */

function petitions44_preprocess_page(&$variables) {
  drupal_add_js('(function($) {$(".recent ul").append($("#block-wh-petitions-wh-petitions-recent-petitions ul").html());$(".recent ul li a").addClass("no-follow");})(jQuery)', array('type' =>'inline' , 'scope' => 'footer'));
}

function _petitions44_petition_hero() {
return <<<EOF
  <!-- Petition Hero -->
  <div class="petition-hero">
    <p class="line1">Giving all Americans a way to engage</p>
    <p class="line2">their government on the issues that</p>
    <p class="line3">matter to them.</p>
    <p class="line4">Get Started</p>
    <a href="/petitions" class="view-petition-btn">View Petitions</a><a href="/petition/create" class="start-petition-btn no-follow">Start a Petition</a>
  </div><!-- /petition-hero -->
  <!-- /Petition Hero -->
EOF;
}

function _petitions44_easy_steps() {
return <<<EOF
  <!-- Easy Steps -->
  <div class="grid-38 first-grid">
    <div class="easy-steps"><span class="bold-intro">We the People</span> in Three Easy Steps</div>
    <div class="grid-12 steps">
      <h3 class="step">Step 1</h3>
      <p>Browse open petitions to find a petition related to your issue, and add your signature.</p>
      <hr />
      <div class="take-action">Take Action</div>
        <a class="triangle-link no-follow" href="/petitions">Find a petition</a>
    </div>
    <div class="grid-11 first-grid steps">
      <h3 class="step">Step 2</h3>
      <p>If your issue is not currently represented by an active petition, start a new petition.</p>
      <hr />
      <div class="take-action">Take Action</div>
      <a class="triangle-link no-follow" href="/petition/create">Start a Petition</a>
    </div>
    <div class="grid-11 first-grid last-grid steps">
      <h3 class="step">Step 3</h3>
      <p>If a petition meets the signature threshold, it will be reviewed by the Administration and we will issue a response.</p>
      <hr />
      <div class="take-action">Take Action</div>
      <a class="triangle-link no-follow" href="/responses">View all responses</a>
    </div>
  </div><!--/grid-38 steps-->
  <!-- /Easy Steps -->
EOF;
}

function _petitions44_introductory_video() {
return <<<EOF
  <!-- Introductory Video -->
  <div class="grid-38 first-grid video-region">
    <a href ="#introvideo" class="video-link no-follow">Watch the Introductory Video</a>
    <blockquote>&#8220;My administration is committed to creating an unprecedented level of openness in government. We will work together to ensure the public trust and establish a system of transparency, public participation and collaboration. Openness will strengthen our democracy and promote efficiency and efffectiveness in government.&#8221;</blockquote>
    <div class="attribution">&#151; President Barack Obama</div>
  </div>
  <!-- /Introductory Video -->
EOF;
}

function _petitions44_featured_reponses() {
return <<<EOF
  <!-- Featured Petition Responses -->
  <div class="featured">
    <h3>Featured Petition Responses</h3>
    <a href="/responses" class="see-all no-follow">See All</a>
    <ul>
      <li><a href="/response/combating-online-piracy-while-protecting-open-and-innovative-internet" class="no-follow">Combatting Online Piracy while Protecting an Open and Innovative Internet</a></li>
      <li><a href="/response/building-21st-century-immigration-system" class="no-follow">Building a 21st Century Immigration System</a></li>
      <li><a href="/response/taking-action-reduce-burden-student-loan-debt" class="no-follow">Taking Action to Reduce the Burden of Student Loan Debt</a></li>
      <li><a href="/response/repealing-discriminatory-defense-marriage-act" class="no-follow">Repealing the Discriminatory Defense of Marriage Act</a></li></ul>
  </div>
  <!-- /Featured Petition Responses -->
EOF;
}

function _petitions44_front_page_aside() {
  $output = '';

  // Featured responses
  $featured_responses = _petitions44_featured_reponses();

  // Recent petitions
  $block = module_invoke('wh_petitions', 'block_view', 'wh_petitions_recent_petitions');
  db_set_active();

  $recent_petitions = '<div class="recent">';
  $recent_petitions = '<div class="recent">';
  $recent_petitions .= '<h3>Most Recent Petitions</h3>';
  $recent_petitions .= '<a href="/petitions" class="see-all">See All</a>';
  $recent_petitions .= $block['content'];
  $recent_petitions .= '</div><!-- /recent -->';

  // Twitter
  $twitter = '<div id="latest-tweet" class="petition-twitter"></div>';
  $twitter .= '<img class="bird-img" src="/profiles/petition/themes/petitions/img/petitions_tw_landing.png" alt="twitter bird" />';

  $output .= '<aside>';
    $output .= '<div class="right-side">';
      $output .= '<div class="right-inner">';
      $output .= $featured_responses;
      $output .= $recent_petitions;
      $output .= '</div><!-- /right-inner -->';
      $output .= $twitter;
    $output .= '</div><!--right side-->';
  $output .= '</aside>';

  return $output;
}

function _petitions44_front_page_content() {
  $output = '';
  
  $petition_hero = _petitions44_petition_hero();
  $easy_steps = _petitions44_easy_steps();
  $introductory_video = _petitions44_introductory_video();
  $aside = _petitions44_front_page_aside();
  $more_from_the_whitehouse = _petitions44_more_from_the_whitehouse();

  $output .= $petition_hero;
  $output .= $easy_steps;
  $output .= $introductory_video;;
  $output .= $aside;
  $output .= $more_from_the_whitehouse;
  
  return $output;
}

function _petitions44_more_from_the_whitehouse() {
  $output = <<<EOF
  <div class="grid-container">
    <div class="grid-59 first-grid" style="height:360px;">
      <h3 class="more">More from the <span class="bold-intro">White House</span></h3>

      <div class="grid-18 follow">
        <h4>Follow Us</h4>
        <a href="https://www.facebook.com/whitehouse" class="facebook-link no-follow">Join the conversation</a>
        <a href="https://www.twitter.com/whitehouse" class="twitter-link no-follow">Get the latest news and engage</a>
        <a href="https://plus.google.com/105479712798762608629" class="gplus-link no-follow">Hangout and go behind the scenes</a>
      </div>

      <div class="grid-20 last-grid featured-1">
        <a href="http://www.whitehouse.gov/blog/2012/01/24/blueprint-america-built-last" class="image-link no-follow"><img src="/profiles/petition/themes/petitions/img/petitions_landing_wh_content_01.jpg" /></a>
        <a class="featured-link bold no-follow" href="http://www.whitehouse.gov/blog/2012/01/24/blueprint-america-built-last">Blueprint for an America Built to Last</a>
        <p>Learn more about President Obama's plan to build an economy that works for everyone.</p>
        <a class="featured-link no-follow" href="http://www.whitehouse.gov/blog/2012/01/24/blueprint-america-built-last">Read the blueprint</a>
      </div>

      <div class="grid-20 last-grid featured-2">
        <a href="http://www.whitehouse.gov/economy/jobs/we-cant-wait" class="image-link no-follow"><img src="/profiles/petition/themes/petitions/img/petitions_landing_wh_content_02.jpg" /></a>
        <a class="featured-link bold no-follow" href="http://www.whitehouse.gov/economy/jobs/we-cant-wait">We Can't Wait</a>
        <p>President Obama is not letting congressional gridlock slow our economic growth.</p>
        <a class="featured-link no-follow" href="http://www.whitehouse.gov/economy/jobs/we-cant-wait">Learn More</a>
      </div>

  </div>
</div>
<!-- /two -->
EOF;
  return $output;
}
