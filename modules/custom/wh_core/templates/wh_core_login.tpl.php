<?php

/**
 * @file
 * Core login form.
 *
 * Variables provided by wh_core:
 *  - $wh_core_login,
 *  - $options, array, displays options required for forgot my password
 *    destination re-direct and attributes.
 *
 * @see l()
 */
?>

<div id="user-login-form-wrapper">
  <h1>Login to Your Account</h1>
  <p class="password">
    <?php print l(t('Forgot password?'), 'user/password', $options); ?>
  </p> <!--/password -->
</div><!--/user register form wrapper -->
