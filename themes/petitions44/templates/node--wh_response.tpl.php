<?php
/**
 * @file
 * Zen theme's implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   - view-mode-[mode]: The view mode, e.g. 'full', 'teaser'...
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 *   The following applies only to viewers who are registered users:
 *   - node-by-viewer: Node is authored by the user currently viewing the page.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $pubdate: Formatted date and time for when the node was published wrapped
 *   in a HTML5 time element.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content. Currently broken; see http://drupal.org/node/823380
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * 
 * - $issues_form: Rendered form for wh_response_list_page_issues_form
 * - $search_form: Rendered form for wh_response_list_page_search_form
 * - $petition_count: Number of petitions that this node is responding to.
 * - $petition_html: Rendered HTML of the petitions related to this response.
 * - $petition_title: Title of the petition related to this response
 * - $audio: Field variable: Accompanying audio file for this response
 * - $video: Field variable: Accompanying audio file for this response
 * - $body: Field variable: Main content of response
 * - $agency_name: The name of the agency that has generated this response
 * - $agency_seal: The seal of the agency that has generated this response
 * - $twitter_link: Link for user to submit this response to twitter
 * - $facebook_link:Link for user to submit this response to facebook
 * - $response_id: $node->nid
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see zen_preprocess_node()
 * @see template_process()
 */
?>

<div class="petition-detail clearfix">
  <div class="petition-response individual-response">
    <div class="response-author-org clearfix">
      <?php print $agency_seal; ?>
      <span class="official"><?php print t('Official !agency_name Response to', array('!agency_name' => $agency_name)); ?></span>
      <?php if ($petition_count > 0): ?>
        <span class="petition-title"><?php print $petition_title; ?>       
          <?php if ($petition_count > 1): ?>
            <a href="" class="no-follow" id="more-petitions">
            <?php if ($petition_count == 2): ?>
              <?php print t('and !count other petition', array('!count' => $petition_count - 1)); ?>
            <?php else: ?>
              <?php print t('and !count other petitions', array('!count' => $petition_count - 1)); ?>
            <?php endif; ?>
            </a>
          <?php endif; ?>
        </span><!--/petition title-->
      <?php endif; ?>
    </div><!--/response author-org-->
                             
    <h1 class="title"><?php print $title; ?></h1>
    
    <?php if (!empty($video)): ?>
      <div class="media"><?php print $video; ?></div>
    <?php endif; ?>
    <?php if (!empty($audio)): ?>
      <div class="media"><?php print $audio; ?></div>
    <?php endif; ?>  
      
    <?php print $body; ?>
  </div><!--/petition response-->            

  <div id="share-this-petition">
    <ul>
      <li class="title"><?php print t('Share this Response'); ?></li>
      <li class="share-link twitter"><?php print $twitter_link; ?></li>
      <li class="share-link facebook"><?php print $facebook_link; ?></li>
    </ul>
  </div><!--/share this petition-->
</div><!--/petition detail-->
              
<?php if ($petition_count > 0): ?>              
  <div class="petition-list clearfix">
    <div class="response-applies-msg">
      <?php if ($petition_count == 1): ?>
        <?php print t('The above response applies to the following !count petition', array('!count' => $petition_count)); ?>
      <?php else: ?>
        <?php print t('The above response applies to the following !count petitions', array('!count' => $petition_count)); ?>
      <?php endif; ?>
    </div><!--/response applies-msg-->
              
    <div class="full-page-list" id="petitions">
      <?php print $petition_html; ?>
    </div><!--full-page-list-->
  </div><!--/petition-list-->
<?php endif; ?>
