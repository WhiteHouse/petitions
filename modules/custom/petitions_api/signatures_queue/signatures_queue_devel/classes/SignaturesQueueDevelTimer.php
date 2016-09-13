<?php
/**
 * @file
 * Defines SignaturesQueueDevelTimer.
 */

/**
 * A timer for the signatures_queue_devel module.
 */
class SignaturesQueueDevelTimer {

  const TIMER_STOPPED = 0;
  const TIMER_RUNNING = 1;
  const TIMER_PAUSED = 2;
  const TIMER_STATUS_INVALID_READ_STATE = 0;

  protected $timer;
  protected $duration = 0;
  protected $state;

  /**
   * Create a new timer object.
   *
   * @param string $name
   *   (Optional) name for the timer. Defaults to 'default'.
   */
  public function __construct($name = 'default') {
    $this->timer = SignaturesQueueDevelTimer::timerName($name);
    $this->state = TIMER_STOPPED;
  }

  /**
   * Start the timer or resume a running (paused) timer.
   */
  public function start() {
    if ($this->state !== TIMER_PAUSED) {
      $this->stop();
      $this->destroy();
    }

    $this->state = TIMER_RUNNING;
    timer_start($this->timer);
  }

  /**
   * Pause the timer without destroying it.
   */
  public function pause() {
    if ($this->state !== TIMER_RUNNING) {
      return;
    }
    $this->state = TIMER_PAUSED;
    $this->stopTimer();
  }

  /**
   * Stop the timer and destroy it.
   */
  public function stop() {
    $this->state = TIMER_STOPPED;
    $this->stopTimer();
    $this->destroy();
  }

  /**
   * Stop a running timer.
   */
  protected function stopTimer() {
    timer_stop($this->timer);
    $this->duration = timer_read($this->timer);
  }

  /**
   * Read the current timer duration.
   *
   * If the timer is stopped or paused, returns the current duration.  A running
   * timer cannot be read.
   *
   * @return int
   *   The length (in milliseconds) the timer has run.
   *
   * @throws Exception
   *   If the timer is still running.
   */
  public function read() {
    if ($this->state == TIMER_RUNNING) {
      $message = "";
      throw new Exception($message, TIMER_STATUS_INVALID_READ_STATE);
    }
    return $this->duration;
  }

  /**
   * Destroy a timer.
   */
  protected function destroy() {
    unset($GLOBALS['timers'][$this->timer]);
  }

  /**
   * Generates a namespaced timer name.
   *
   * @param string $name
   *   The given timer name.
   *
   * @return string
   *   The timer name prefixed with the signatures_queue_devel_ namespace.
   */
  public static function timerName($name) {
    return "signatures_queue_devel_process_timer_{$name}";
  }

}
