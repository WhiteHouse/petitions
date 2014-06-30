<?php
/**
 * @file
 * Signatures comparison page.
 */
extract($variables);
?>
<h1>Comparing Mongo data and MySQL data for Signatures</h1>

<p>signature id of: <?php echo $signature_id; ?> (legacy_id in MySql)</p>

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
  foreach ($o_signature->compare as $item):

    $mongo = (isset($o_signature->mongo[$item['mongo']])) ?
          $o_signature->mongo[$item['mongo']] : 'non-existent';

    $lookup = "o_signature->mongo" . $item['mongo'];

    // @TODO This eval is risky, but can't get it to work any other way.
    $mongo = eval("return \${$lookup};");
    $mongo = (!empty($mongo) || ($mysql === "0") || ($mysql === 0)) ? $mongo : 'non-existent';

    $lookup = "o_signature->mysql->" . $item['mysql'];
    // @TODO This eval is risky, but can't get it to work any other way.
    $mysql = eval("return \${$lookup};");
    $mysql = (!empty($mysql) || ($mysql === "0") || ($mysql === 0)) ? $mysql : 'non-existent';
    $id = strtolower($item['name']);

    $compare = ($mongo == $mysql) ? '=' : 'X'; ?>
    
    <tr id="row_<?php echo $id ?>">
      <td>
        <?php echo $item['name'] ?>
      </td>
      <td id="mysql_<?php echo $id ?>">
        <?php echo $mysql; ?>
      </td>
      <td id="comp_<?php echo $id ?>">
        <?php echo $compare; ?>
      </td>
      <td id="mongo_<?php echo $id ?>">
        <?php echo $mongo; ?>
      </td>
    </tr>
  <?php
  endforeach; ?>
  </tbody>
</table>


<h2>Full Signature objects</h2>
<div>===========================================</div>
<h3 id="mysql">MySQL Signature</h3>
<a href="#tablecompare">to table</a>
<pre>
  <?php print_r($o_signature->mysql); ?>
</pre>


<div>===========================================</div>
<h3 id="mongo">Mongo Signature</h3>
<a href="#tablecompare">to table</a>
<pre>
  <?php print_r($o_signature->mongo); ?>
</pre>
