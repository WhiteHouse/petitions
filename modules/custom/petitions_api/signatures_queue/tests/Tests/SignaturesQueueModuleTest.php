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
   * Tests email address domain extraction.
   *
   * @param string $expected
   *   The expected output from the function.
   * @param mixed $address
   *   The email address to test.
   *
   * @dataProvider providerSignaturesQueueGetDomainFromEmail
   */
  public function testSignaturesQueueGetDomainFromEmail($expected, $address) {
    $this->assertEquals($expected, signatures_queue_get_domain_from_email($address));
  }

  /**
   * Data provider for testSignaturesQueueGetDomainFromEmail().
   *
   * @see testSignaturesQueueGetDomainFromEmail()
   */
  public function providerSignaturesQueueGetDomainFromEmail() {
    $tests[] = array('example.com', 'username@example.com');
    $tests[] = array('subdomain.example.com', 'username@subdomain.example.com');
    $tests[] = array(FALSE, 'invalid address');
    return $tests;
  }

  /**
   * Tests subaddressed email address detection.
   *
   * @param bool $expected
   *   The expected output from the function.
   * @param mixed $address
   *   The email address to test.
   *
   * @dataProvider providerSignaturesQueueIsSubaddressedEmail
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
