<?php
/**
 * @file
 * What hooks does the signature module provide.
 */

/**
 * Information about other signatures.
 *
 * @return array
 *   The information to find the class that will set up your signature
 */
function hook_signature_info() {
  $info = array();

  $info['mail'] = array(
    'module' => 'signature',
    // The structure of this array matches the params required to use the
    // function module_load_include().
    'file' => array('php', "signature", "SignatureMail"),
    'class' => "SignatureMail",
  );

  return $info;
}
