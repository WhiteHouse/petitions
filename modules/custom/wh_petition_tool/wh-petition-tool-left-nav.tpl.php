<div class="left-nav">
  <ul>
    <?php foreach($links as $link): ?>
      <li><?php print l($link['link_title'], $link['link_path']); ?></li>
    <?php endforeach; ?>    	
  </ul>        
</div><!--/left-nav-->