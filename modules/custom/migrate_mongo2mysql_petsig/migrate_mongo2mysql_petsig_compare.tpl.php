<?php
/**
 * @file
 * Comparison page template
 */
// extract($variables);
?>
Comparing Mongo data and MySQL data
<table id="tablecompare">
  <thead>
    <tr>
      <th>Item</th>
      <th><a href="#mysql">MySQL</a></th>
      <th>Match</th>
      <th><a href="#mongo">Mongo</a></th>
    </tr>
  </thead>

  <tbody>
  <?php
  $mysql_all = $o_petition->mysql;
  foreach ($o_petition->compare as $item):
     $mongo = (isset($o_petition->mongo[$item['mongo']])) ?
          $o_petition->mongo[$item['mongo']] : 'non-existent';
     $mongo = migrate_mongo2mysql_petsig_arrayout($mongo);

     $mysql_var = $item['mysql'];
     $mysql = (isset($mysql_all->{$mysql_var})) ?
          $mysql_all->{$mysql_var} : 'non-existent';
     $mysql = migrate_mongo2mysql_petsig_arrayout($mysql);

     $id = strtolower($item['name']);

     $compare = ($mongo == $mysql) ? '=' : 'X';

  ?>
    <tr id="row_<?php echo $id; ?>">
      <td>
        <?php echo $item['name']; ?>
      </td>
      <td id="mysql_<?php echo $id; ?>">
        <?php echo $mysql; ?>
      </td>
      <td id="comp_<?php echo $id; ?>">
        <?php echo $compare; ?>
      </td>
      <td id="mongo_<?php echo $id; ?>">
        <?php echo $mongo; ?>
      </td>
    </tr>
  <?php
  endforeach ?>
  </tbody>
</table>


<h2>Full Petition objects</h2>
<div>===========================================</div>
<h3 id="mysql">MySQL Petition</h3>
<a href="#tablecompare">to table</a>
<pre>
  <?php print_r($o_petition->mysql); ?>
</pre>


<div>===========================================</div>
<h3 id="mongo">Mongo Petition</h3>
<a href="#tablecompare">to table</a>
<pre>
  <?php print_r($o_petition->mongo); ?>
</pre>
