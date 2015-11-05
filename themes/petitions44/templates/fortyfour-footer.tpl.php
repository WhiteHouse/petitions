<?php
/**
 * @file
 * petitions44 footer static HTML storage file. This file houses the HTML that
 * is being used to generate the $fortyfour_footer variable in the page.tpl.php
 * file.
 *
 * Available variables:
 * $path_to_fortyfour: Dynamically generates path to fortyfour theme.
 */

/**
 * @file
 * Petitions 44 theme's implementation to display footer content within the
 * page.tpl.php file.
 */
?>

<!-- whitehouse footer -->
<span style="position:absolute;"><a name="sitemap">&nbsp;</a></span>
<div class="clear"></div>
<div id="wh-footer" class="clearfix">
  <?php
    $block_footer_menu = module_invoke('menu_block', 'block_view', 'wh_global_nav_footer_block-1');
    print render($block_footer_menu['content']);
  ?>
</div>
<!-- /whitehouse wh-footer -->
