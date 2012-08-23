<div id="by-the-numbers">
  <?php foreach($numbers as $key => $value): ?>
  <div class="entry">
    <span class="number"><?php print $value['value']; ?></span>
    <span class="text"><?php print $value['key']; ?></span>
  </div>
  <?php endforeach; ?>
</div>