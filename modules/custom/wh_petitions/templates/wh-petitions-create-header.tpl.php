<div id="steps-wrapper" class="class clearfix">
  <div class="step <?php if ($step == 1): ?>step-1-expand step-expand<?php else: ?>step-1-collapse<?php endif; ?> <?php if ($step > 1): ?>step-complete<?php endif; ?>">
    <a><?php if ($step == 1): ?><?php print t('Step 1: Basic Petition Information'); ?><?php else: ?><?php print t('Step 1'); ?><?php endif; ?></a>
  </div><!--/step 1-->
  <div class="step <?php if ($step == 2): ?>step-2-expand step-expand<?php else: ?>step-2-collapse<?php endif; ?> <?php if ($step > 2): ?>step-complete<?php endif; ?>">
    <a><?php if ($step == 2): ?><?php print t('Step 2: Sign Similar Petitions'); ?><?php else: ?><?php print t('Step 2'); ?><?php endif; ?></a>
  </div><!--/step 2-->
  <div class="step <?php if ($step == 3): ?>step-3-expand step-expand<?php else: ?>step-3-collapse<?php endif; ?> <?php if ($step > 3): ?>step-complete<?php endif; ?>">
    <a><?php if ($step == 3): ?><?php print t('Step 3: Add Petition Details'); ?><?php else: ?><?php print t('Step 3'); ?><?php endif; ?></a>
  </div><!--/step 3 -->
  <div class="step <?php if ($step == 4): ?>step-4-expand step-expand<?php else: ?>step-4-collapse<?php endif; ?>">
    <a><?php if ($step == 4): ?><?php print t('Step 4: Preview & Publish Your Petition'); ?><?php else: ?><?php print t('Step 4'); ?><?php endif; ?></a>
  </div><!--/step 4-->
</div><!--/steps wrapper-->