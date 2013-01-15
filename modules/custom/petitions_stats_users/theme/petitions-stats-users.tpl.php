<?php
/**
 * @file
 *
 * Available custom variables:
 * - $date: the date for which statistics will be displayed.
 * - $statistics: user creation statistics for the given date.
 *   An associative array containing:
 *   - users: An array of various user related statistics.
 *     - creation: An array of user creation statistics, keyed by date.
 *       - count: Total count of all created users.
 *       - dates: An array of user creation statistics for a given date.
 *         - {date}:
 *           - title: A string to use for front-end display.
 *           - count: Total count of new users created on the given date.
 *           - hours: An array of user creation statistics, keyed by hour.
 *             - {hour}:
 *               - title: A string to use for front-end display.
 *               - count: Total count of new users created during the hour.
 *               - percentage: The percentage of users created during this hour
 *                 relative to the entire day.
 */
?>
<h3><?php print $date; ?></h3>
<div class="stats-block">
  <p>Users created, by hour:</p>
  <ul class="bar-graph">
    <?php foreach ($statistics['users']['creation']['dates'] as $day) : ?>
      <?php foreach ($day['hours'] as $hour): ?>
          <li>
            <em><?php print $hour['count']; ?> users, <?php print $hour['percentage']; ?>%</em>
            <span style="width: <?php print $hour['percentage']; ?>%;"><?php print $hour['title']; ?>:00</span>
          </li>
      <?php endforeach; ?>
      <li><em><?php print $day['count']; ?> total</em></li>
    <?php endforeach; ?>
  </ul>
</div>
