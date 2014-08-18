<?php

/**
 * @file
 * Hooks provided by the Petitions Log module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Log a system event.
 *
 * This hook allows modules to log events to custom facilities, such as StatsD.
 *
 * @param string $name
 *   The name of the event to log.
 * @param string $type
 *   The type of metric to log--one of the following values corresponding to the
 *   @link https://github.com/etsy/statsd/blob/master/docs/metric_types.md StatsD Metric Types @endlink
 *   :
 *   - count: The corresponding value is a number by which to increment (or
 *     decrement, if negative) a simple counter.
 *   - gauge: The corresponding value is a single datum, which remains constant
 *     until explicitly changed.
 *   - set: The corresponding value is a value to add to a set of unique values.
 *   - time: The corresponding value is a duration in milliseconds.
 * @param int|null $value
 *   The numeric value you wish to log. Defaults to NULL.
 *
 * @see petitionslog_event()
 */
function hook_petitionslog_event($name, $type, $value) {
  // Send the metric to StatsD.
  switch ($type) {
    case 'count':
      StatsD::updateStats($name, $value);
      break;

    case 'gauge':
      StatsD::gauge($name, $value);
      break;

    case 'time':
      StatsD::timing($name, $value);
      break;
  }
}

/**
 * @} End of "addtogroup hooks".
 */
