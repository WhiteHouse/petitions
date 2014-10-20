<?php

/**
 * @file
 * Contains SignaturesQueueModuleTest.
 */

require_once dirname(__FILE__) . '/../../signatures_queue.module';

/**
 * Tests the functions in signatures_queue.module.
 */
class SignaturesQueueModuleTest extends PHPUnit_Framework_TestCase {

  /**
   * Tests subaddressed email address detection.
   *
   * @dataProvider providerSignaturesQueueIsSubaddressedEmail
   *
   * @param bool $expected
   *   The expected output from the function.
   * @param mixed $address
   *   The email address to test.
   */
  public function testSignaturesQueueIsSubaddressedEmail($expected, $address) {
    $message = ($expected) ? 'Failed to identify a subaddressed email' : 'Falsely identified a non-subaddressed email.';
    $this->assertEquals($expected, signatures_queue_is_subaddressed_email($address), $message);
  }

  /**
   * Data provider for testSignaturesQueueIsSubaddressedEmail().
   *
   * @see testSignaturesQueueIsSubaddressedEmail()
   */
  public function providerSignaturesQueueIsSubaddressedEmail() {
    // Subaddressed email addresses.
    $tests[] = array(TRUE, 'username+tag@example.com');
    $tests[] = array(TRUE, 'username+tag+@example.com');
    $tests[] = array(TRUE, 'username++tag@example.com');
    $tests[] = array(TRUE, 'username+@example.com');
    $tests[] = array(TRUE, 'username++@example.com');

    // Non-subaddressed email addresses.
    $tests[] = array(FALSE, 'username@example.com');
    $tests[] = array(FALSE, '+tag@example.com');
    $tests[] = array(FALSE, 'username@sub+domain.example.com');
    $tests[] = array(FALSE, '');

    return $tests;
  }

}
