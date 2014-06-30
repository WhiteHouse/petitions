<?php
/**
 * @file
 * View for MongoDB Signature listing
 */
extract($variables);
$i = 1;
if (!empty($o_signatures)): ?>
  <ol>
  <?php 
  foreach (is_object($o_signatures) ? $o_signatures : array() as $key => $o_signature): ?>
    <li class="sig_<?php echo $i++; ?>">
    <?php
    if (!empty($o_signature['_id'])): ?>
      <a href="/admin/reports/mongo-mysql-signature/compare/<?php echo $o_signature['_id']; ?>"><?php echo $o_signature['_id'] ?></a>
    <?php
    else: ?>
      <div>empty</div>
    <?php
    endif ?>
    </li>
  <?php
  endforeach ?>
  </ol>
<?php
endif ?>
