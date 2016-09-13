<?php

/**
 * @file
 * Contains SignaturesQueueModuleTest.
 */

require_once dirname(__FILE__) . '/../signatures_queue.module';

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
    $this->assertEquals($expected, disposable_email_is_subaddressed_email($address), $message);
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

  /**
   * Tests unsubaddress email address manipulation.
   *
   * @param mixed $address
   *   The email address to test.
   * @param mixed $expected
   *   The expected output from the function.
   *
   * @dataProvider providerSignaturesQueueUnsubaddressEmail
   */
  public function testSignaturesQueueUnsubaddressEmail($address, $expected) {
    $this->assertEquals($expected, disposable_email_unsubaddress_email($address));
  }
  /**
   * Data provider for testSignaturesQueueUnsubaddressEmail().
   *
   * @see testSignaturesQueueUnsubaddressEmail()
   */
  public function providerSignaturesQueueUnsubaddressEmail() {
    // Subaddressed email addresses.
    $tests[] = array('username+tag@example.com', 'username@example.com');
    $tests[] = array('username+tag+@example.com', 'username@example.com');
    $tests[] = array('username++tag@example.com', 'username@example.com');
    $tests[] = array('username+@example.com', 'username@example.com');
    $tests[] = array('username++@example.com', 'username@example.com');
    $tests[] = array('username@example.com', 'username@example.com');
    $tests[] = array('+tag@example.com', '+tag@example.com');
    $tests[] = array('username@sub+domain.example.com', 'username@sub+domain.example.com');
    // Dot email addresses.
    $tests[] = array('user.name@example.com', 'username@example.com');
    $tests[] = array('user.name.@example.com', 'username@example.com');
    $tests[] = array('.user.name@example.com', 'username@example.com');
    $tests[] = array('user..name@example.com', 'username@example.com');
    $tests[] = array('user.name+tag@example.com', 'username@example.com');
    $tests[] = array('user.name+tag.tag@example.com', 'username@example.com');
    return $tests;
  }

}
