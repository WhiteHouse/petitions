<?php
/**
 * @file
 * Template file for theming the documetnation for a given Services method.
 *
 * Available custom variables:
 * - $name:
 * - $description:
 * - $request:
 * - $response:
 * - $method: the method array defined in hook_services_resources().
 */
?>
<!-- services-documentation-method -->
<div class="services-documentation-method">
  <?php if (!empty($name)): ?>
    <h4 class="method-name"><?php print $name; ?></h4>
  <?php endif; ?>

  <?php if (!empty($description)): ?>
    <p class="method-description"><?php print $description; ?></p>
  <?php endif; ?>

  <div class="method-arguments">
    <?php if (!empty($method) && !empty($method['args'])): ?>
      <span class="title">Arguments</span>
      <ul>
        <?php foreach ($method['args'] as $argument): ?>
          <li class="method-argument">

            <div class="method-argument-title">
              <em><?php print $argument['type']; ?></em>
              <strong><?php print $argument['name']; ?></strong>

              <span class="method-argument-source">,
                <?php if (is_array($argument['source'])): ?>
                  <?php //print key($argument['source']) ?>
                  <?php print $argument['http_method'] ?>
                <?php else: ?>
                  <?php print $argument['source']; ?>
                <?php endif; ?>
              </span
            </div>

            <div class="method-argument-description">
              <?php if ($argument['optional']): ?>
                (optional)
              <?php endif; ?>
              <?php print $argument['description']; ?>
            </div>

          </li>
        <?php endforeach; ?>
      </ul>
    <?php endif; ?>
  </div>

  <?php if (!empty($request)): ?>
    <h5>Request Example</h5>
    <pre class="request-example"><?php print $request; ?></pre>
  <?php endif; ?>

  <?php if (!empty($response)): ?>
    <h5>Response Example</h5>
    <pre class="response-example"><?php print $response; ?></pre>
  <?php endif; ?>

</div>
<!-- /services-documentation-method -->
