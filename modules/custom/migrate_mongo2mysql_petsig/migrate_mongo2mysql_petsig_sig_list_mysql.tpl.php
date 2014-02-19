<?php
/**
 * @file
 * Displaying MySQL signature listing.
 */
extract($variables);
$i = 1;
if (!empty($a_signatures)): ?>
  <ol>
  <?php
  foreach (is_array($a_signatures) ? $a_signatures : array() as $key => $o_signature): ?>
    <li class="sig_<?php echo $i++; ?>">
    <?php
    if (!empty($o_signature->legacy_id)):?>
      <a href="/admin/reports/mongo-mysql-signature/compare/<?php echo $o_signature->legacy_id; ?>"><?php echo $o_signature->legacy_id; ?></a>
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
